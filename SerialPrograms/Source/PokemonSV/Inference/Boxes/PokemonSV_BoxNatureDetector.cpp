/*  Box Nature Detector
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#include "CommonFramework/ImageTypes/ImageViewRGB32.h"
#include "PokemonSV_BoxNatureDetector.h"

namespace PokemonAutomation{
namespace NintendoSwitch{
namespace PokemonSV{

const NatureReader& NATURE_READER(){
    const static Pokemon::NatureReader reader("PokemonSV/NatureCheckerOCR.json");
    return reader;
}

BoxNatureDetector::BoxNatureDetector(VideoOverlay& overlay, Language language)
    : m_language(language)
    , m_box_nature(overlay, {0.757, 0.529, 0.141, 0.050})
{}

NatureCheckerValue BoxNatureDetector::read(Logger& logger, const ImageViewRGB32& frame, const OverlayBoxScope& box){
    ImageViewRGB32 image = extract_box_reference(frame, box);
    OCR::StringMatchResult result = NATURE_READER().read_substring(
        logger, m_language, image,
        OCR::WHITE_TEXT_FILTERS()
    );
    result.clear_beyond_log10p(-1.40);
    if (result.results.size() != 1){
        return NatureCheckerValue::UnableToDetect;
    }
    return NatureCheckerValue_string_to_enum(result.results.begin()->second.token);
}
NatureReader::Results BoxNatureDetector::read(Logger& logger, const ImageViewRGB32& frame){
    NatureReader::Results results;
    if (m_language != Language::None){
        results.nature = read(logger, frame, m_box_nature);
    }
    return results;
}



}
}
}
