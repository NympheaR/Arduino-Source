/*  Sweat Bubble Detector
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#ifndef PokemonAutomation_PokemonSV_SweatBubbleDetector_H
#define PokemonAutomation_PokemonSV_SweatBubbleDetector_H

#include "CommonFramework/ImageTools/ImageBoxes.h"
#include "CommonFramework/VideoPipeline/VideoOverlayScopes.h"
#include "CommonFramework/InferenceInfra/VisualInferenceCallback.h"
#include "CommonFramework/Inference/VisualDetector.h"

namespace PokemonAutomation{
namespace NintendoSwitch{
namespace PokemonSV{



class SweatBubbleDetector : public StaticScreenDetector{
public:
    SweatBubbleDetector(Color color, const ImageFloatBox& box = {0.11, 0.81, 0.06, 0.10});

    virtual void make_overlays(VideoOverlaySet& items) const override;
    virtual bool detect(const ImageViewRGB32& screen) const override;

protected:
    Color m_color;
    ImageFloatBox m_box;
};
class SweatBubbleWatcher : public DetectorToFinder<SweatBubbleDetector>{
public:
    SweatBubbleWatcher(Color color = COLOR_RED)
         : DetectorToFinder("SweatBubbleWatcher", std::chrono::milliseconds(100), color)
    {}
};




}
}
}
#endif
