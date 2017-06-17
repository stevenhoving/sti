#pragma once

#include <sti/color_image.hpp>
#include <aeon/streams/stream.h>

namespace sti
{
namespace codecs
{

struct png
{
    static auto decode(aeon::streams::stream &stream) -> color_image;
    static void encode(const color_image &image, aeon::streams::stream &stream);
};

} // namespace codecs
} // namespace sti
