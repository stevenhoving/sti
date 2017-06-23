#pragma once

#include <sti/interleaved_image.hpp>
#include <aeon/streams/stream.h>

namespace sti
{
namespace codecs
{

struct png
{
    static auto decode(aeon::streams::stream &stream) -> interleaved_image;
    static void encode(const interleaved_image &image, aeon::streams::stream &stream);
};

} // namespace codecs
} // namespace sti
