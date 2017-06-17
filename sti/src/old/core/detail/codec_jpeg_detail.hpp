#pragma once

#include "image_color_detail.hpp"
#include "memory_file.hpp"

#if 0
#include <ujpeg/ujpeg.h>

namespace sti {
namespace detail {
namespace codec {
namespace jpeg {

static sti::detail::image_color decompress_YUV(sti::detail::memory_file &src)
{
    uJPEG uj;
    uj.decode(src.data(), src.size());

    // yuv420 -> y = 1 byte per pixel, u = 1/2 byte per pixel v = 1/2 byte per
    // pixel.
    auto info = sti::detail::image_color::image_info(uj.getWidth(),
        uj.getHeight(), 2,
        uj.getPlane(0)->stride,
        sti::detail::image_color::image_format::YUV420);
    auto image = sti::detail::image_color(info);

    // \todo crosses our fingers I allocated enough memory.
    auto buffer_size = uj.getPlane(0)->height * uj.getPlane(0)->stride;
    memmove(image.data(), uj.getPlane(0)->pixels, buffer_size);

    return image;
}

} // namespace jpeg
} // namespace codec
} // namespace detail
} // namespace sti

#endif