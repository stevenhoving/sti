#pragma once

#include <cstdint>

namespace sti {
namespace detail {
namespace image_resize {

// simple bilinear single channel resize
//void resize_bilinear(const sti::core::image<T> &pixels, sti::core::image<T> &temp)
static
void resize_bilinear(
    const uint8_t *src, const int src_width, const int src_height,
    uint8_t *dst, const int dst_width, const int dst_height)
{
    int A, B, C, D, index;
    const float x_ratio = ((float)(src_width - 1)) / dst_width;
    const float y_ratio = ((float)(src_height - 1)) / dst_height;
    float x_diff, y_diff;
    int offset = 0;
    for (int i = 0; i < dst_height; ++i)
    {
        for (int j = 0; j < dst_width; ++j)
        {
            auto x = (int)(x_ratio * j);
            auto y = (int)(y_ratio * i);
            x_diff = (x_ratio * j) - x;
            y_diff = (y_ratio * i) - y;
            
            index = (y*src_width) + x;

            A = src[index];
            B = src[index + 1];
            C = src[index + src_width];
            D = src[index + src_width + 1];

            // Y = A(1-w)(1-h) + B(w)(1-h) + C(h)(1-w) + Dwh
            auto gray = (int)(
                A*(1 - x_diff)*(1 - y_diff) + B*(x_diff)*(1 - y_diff) +
                C*(y_diff)*(1 - x_diff) + D*(x_diff*y_diff)
                );

            dst[offset++] = gray;
        }
    }
    //return temp;
}

} // image_resize
} // namespace detail
} // namespace sti
