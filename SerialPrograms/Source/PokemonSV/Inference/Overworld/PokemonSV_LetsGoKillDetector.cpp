/*  Let's Go Kill Detector
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#include "Common/Cpp/AbstractLogger.h"
#include "Kernels/Waterfill/Kernels_Waterfill_Session.h"
#include "CommonFramework/Globals.h"
#include "CommonFramework/ImageTools/BinaryImage_FilterRgb32.h"
#include "CommonFramework/ImageMatch/ExactImageMatcher.h"
#include "CommonFramework/VideoPipeline/VideoOverlayScopes.h"
#include "PokemonSV_LetsGoKillDetector.h"

//#include <iostream>
//using std::cout;
//using std::endl;

namespace PokemonAutomation{
namespace NintendoSwitch{
namespace PokemonSV{

using namespace Kernels::Waterfill;


const ImageMatch::ExactImageMatcher& LETS_GO_KILL_CROPPED(){
    static ImageMatch::ExactImageMatcher matcher(RESOURCE_PATH() + "PokemonSV/LetsGoKill-Cropped.png");
    return matcher;
}


bool is_kill_icon(
    const ImageViewRGB32& image,
    WaterfillObject& object,
    const WaterfillObject& red, const WaterfillObject& white
){
    if (red.max_y >= white.min_y){
        return false;
    }
    if (red.min_x < white.min_x){
        return false;
    }
    if (red.max_x < white.max_x){
        return false;
    }

    object = red;
    object.merge_assume_no_overlap(white);

    extract_box_reference(image, object).save("test.png");

    if (object.width() < 20 || object.height() < 20){
//        cout << "too small: " << object.width() << endl;
        return false;
    }
    double aspect_ratio = object.aspect_ratio();
    if (aspect_ratio < 0.8 || aspect_ratio > 1.2){
//        cout << aspect_ratio << endl;
        return false;
    }

    ImageViewRGB32 cropped = extract_box_reference(image, object);
//    cropped.save("test.png");
    double rmsd = LETS_GO_KILL_CROPPED().rmsd(cropped);
//    cout << rmsd << endl;
    return rmsd < 120;
}





LetsGoKillDetector::LetsGoKillDetector(
    Color color,
    const ImageFloatBox& box
)
    : m_color(color)
    , m_box(box)
{}
void LetsGoKillDetector::make_overlays(VideoOverlaySet& items) const{
    items.add(m_color, m_box);
}
bool LetsGoKillDetector::detect(const ImageViewRGB32& screen) const{
    using namespace Kernels::Waterfill;

    ImageViewRGB32 region = extract_box_reference(screen, m_box);
    std::vector<WaterfillObject> reds;
    std::vector<WaterfillObject> whites;
    std::unique_ptr<WaterfillSession> session = make_WaterfillSession();
    {
        std::vector<PackedBinaryMatrix> matrices = compress_rgb32_to_binary_range(
            region,
            {
                {0xffc02020, 0xffff7f7f},
            }
        );
        size_t c = 0;
        for (PackedBinaryMatrix& matrix : matrices){
            session->set_source(matrix);
            auto iter = session->make_iterator(10);
            WaterfillObject object;
            while (iter->find_next(object, false)){
                double aspect_ratio = object.aspect_ratio();
                if (aspect_ratio < 1.5 || aspect_ratio > 2.5){
                    continue;
                }
//                cout << object.area << endl;
                extract_box_reference(region, object).save("red-" + std::to_string(c++) + ".png");
                reds.emplace_back(std::move(object));
            }
        }
    }
    {
        std::vector<PackedBinaryMatrix> matrices = compress_rgb32_to_binary_range(
            region,
            {
                {0xffc0c0c0, 0xffffffff},
            }
        );
        size_t c = 0;
        for (PackedBinaryMatrix& matrix : matrices){
            session->set_source(matrix);
            auto iter = session->make_iterator(10);
            WaterfillObject object;
            while (iter->find_next(object, false)){
                double aspect_ratio = object.aspect_ratio();
                if (aspect_ratio < 1.5 || aspect_ratio > 2.5){
                    continue;
                }
//                cout << object.area << endl;
                extract_box_reference(region, object).save("white-" + std::to_string(c++) + ".png");
                whites.emplace_back(std::move(object));
            }
        }
    }
    for (WaterfillObject& red : reds){
        for (WaterfillObject& white : whites){
            WaterfillObject object;
            if (is_kill_icon(region, object, red, white)){
                return true;
            }
        }
    }

    return false;
}

bool LetsGoKillWatcher::process_frame(const ImageViewRGB32& frame, WallClock timestamp){
    bool result = DetectorToFinder::process_frame(frame, timestamp);
    if (!result){
        return false;
    }

    if (!m_last_detected){
        m_logger.log("Detected Let's Go kill icon.");
    }
    m_last_detected = true;

    return m_trigger_if_detected;
}





}
}
}
