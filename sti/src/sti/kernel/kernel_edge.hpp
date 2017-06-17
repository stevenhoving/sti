#pragma once

#include <sti/kernel/kernel.hpp>

namespace sti
{
namespace kernel
{
namespace edge
{
// \see https://en.wikipedia.org/wiki/Kernel_(image_processing)
// This kernel detects both the horizontal and vertical edges.
template <typename T, int Size>
kernel<T, Size> make_kernel()
{
    kernel<T, Size> kern;
    std::fill(kern.begin(), kern.end(), static_cast<T>(-1.0));
    kern[Size / 2][Size / 2] = static_cast<T>(Size * Size) - static_cast<T>(1.0);
    return kern;
}

} // namespace edge
} // namespace kernel
} // namespace sti
