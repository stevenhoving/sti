#pragma once

#include <sti/kernel/kernel.hpp>

namespace sti
{
namespace kernel
{
namespace sharpen
{

template <typename T, int Size>
kernel<T, Size> make_kernel()
{
    kernel<T, Size> kern;
    std::fill(kern.begin(), kern.end(), static_cast<T>(-1.0));
    kern[Size / 2][Size / 2] = Size * Size;
    return kern;
}

} // namespace sharpen
} // namespace kernel
} // namespace sti
