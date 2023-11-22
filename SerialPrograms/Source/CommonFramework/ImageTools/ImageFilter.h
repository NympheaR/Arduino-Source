/*  Image Filter
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#ifndef PokemonAutomation_CommonFramework_ImageFilter_H
#define PokemonAutomation_CommonFramework_ImageFilter_H

#include <cstddef>
#include <vector>
#include "Common/Cpp/Color.h"

namespace PokemonAutomation{

class ImageViewRGB32;
class ImageRGB32;


//  If `replace_color_within_range` is true, replace the color range [mins, maxs] with the color `replacement_color`.
//  If `replace_color_within_range` is false, replace the color outside of the range [mins, maxs] with the color
//    `replacement_color`.
ImageRGB32 filter_rgb32_range(
    const ImageViewRGB32& image,
    uint32_t mins, uint32_t maxs,
    Color replacement_color, bool replace_color_within_range
);
//  If `replace_color_within_range` is true, replace the color range [mins, maxs] with the color `replacement_color`.
//  If `replace_color_within_range` is false, replace the color outside of the range [mins, maxs] with the color
//    `replacement_color`.
//  Returns the # of pixels inside the range [mins, maxs] as `pixels_in_range`
ImageRGB32 filter_rgb32_range(
    size_t& pixels_in_range,
    const ImageViewRGB32& image,
    uint32_t mins, uint32_t maxs,
    Color replacement_color, bool replace_color_within_range
);


struct FilterRgb32Range{
    uint32_t mins;
    uint32_t maxs;
    Color replacement_color;
    bool replace_color_within_range;
};
//  Run multiple filters at once. This is more memory efficient than making
//  multiple calls to one filter at a time.
//  For each filter:
//  If `replace_color_within_range` is true, replace the color range [mins, maxs] with the color `replacement_color`.
//  If `replace_color_within_range` is false, replace the color outside of the range [mins, maxs] with the color
//    `replacement_color`.
//  For each filter, return the filtered image and the # of pixels inside the [mins, maxs] range of the filter.
std::vector<std::pair<ImageRGB32, size_t>> filter_rgb32_range(
    const ImageViewRGB32& image,
    const std::vector<FilterRgb32Range>& filters
);


//  If `replace_color_within_range` is true, replace the color within (<=) `max_euclidean_distance` of the
//    `expected_color` with `replacement_color`.
//  If `replace_color_within_range` is false, replace the color outside of the distance with the color `replacement_color`.
//  Returns the # of pixels inside the distance.
ImageRGB32 filter_rgb32_euclidean(
    const ImageViewRGB32& image,
    uint32_t expected_color, double max_euclidean_distance,
    Color replacement_color, bool replace_color_within_range
);
ImageRGB32 filter_rgb32_euclidean(
    size_t& pixels_in_range,
    const ImageViewRGB32& image,
    uint32_t expected_color, double max_euclidean_distance,
    Color replacement_color, bool replace_color_within_range
);





//  Convert the image to black and white.
//  Inside [mins, maxs] is white, otherwise it's black.
//  Set "in_range_black" to true to invert the colors.
ImageRGB32 to_blackwhite_rgb32_range(
    const ImageViewRGB32& image,
    uint32_t mins, uint32_t maxs, bool in_range_black
);
ImageRGB32 to_blackwhite_rgb32_range(
    size_t& pixels_in_range,
    const ImageViewRGB32& image,
    uint32_t mins, uint32_t maxs, bool in_range_black
);



//  Run multiple filters at once. This is more memory efficient than making
//  multiple calls to one filter at a time.
struct BlackWhiteRgb32Range{
    uint32_t mins;
    uint32_t maxs;
    bool in_range_black;
};
std::vector<std::pair<ImageRGB32, size_t>> to_blackwhite_rgb32_range(
    const ImageViewRGB32& image,
    const std::vector<BlackWhiteRgb32Range>& filters
);




}
#endif
