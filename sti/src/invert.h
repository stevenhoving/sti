#pragma once

#include "image.h"

namespace sti
{
namespace invert
{
template <typename T>
void invert_image(sti::image<T> &src, sti::image<T> &dst)
{
    for (int i = 0; i < src.width() * src.height(); ++i)
        dst.data()[i] = sti::pixel_max<T>() - src.data()[i];
}

} // namespace invert
} // namespace sti
