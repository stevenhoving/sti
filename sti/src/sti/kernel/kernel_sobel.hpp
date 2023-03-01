#pragma once

#include <sti/kernel/kernel.hpp>

/*!

\todo actually read the stackoverflow post about how a sobel kernel is constructed.
      The main trick that is described (I think) in the post about how to create a sobel
      kernel based on parameters have todo with creating a smoothing and gradient kernel
      based on parameters and then multiply them (or at least that would be useful for us).
\see https://stackoverflow.com/questions/9567882/sobel-filter-kernel-of-large-size

3x3
sx = 1  0 -1
     2  0 -2
     1  0 -1

sy =-1 -2 -1
     0  0  0
     1  2  1

5x5
sx = 1   2   0  -2  -1
     4   8   0  -8  -4
     6  12   0 -12  -6
     4   8   0  -8  -4
     1   2   0  -2  -1

sy =-1  -4  -6  -4  -1
    -2  -8 -12  -8  -2
     0   0   0   0   0
     2   8  12   8   2
     1   4   6   4   1
*/

namespace sti
{
namespace kernel
{
namespace sobel_horizontal
{
template <typename T, int Size>
kernel<T, Size> make_kernel()
{
    static_assert(Size == 3, "We only support horizontal sobel kernels of 3x3 for now");

    kernelv<T, Size> v{{1, 2, 1}};  // smoothing kernel
    //kernelh<T, Size> h{{0, 0, 0}};
    kernelh<T, Size> h;;

    // gradient kernel
    for (int i = 0; i < Size; ++i)
        h[i] = static_cast<float>((-Size / 2) + i);

    auto kern = v * h;
    //kern.factor = 1.f / 8.f;

    return kern;
}

} // namespace sobel_horizontal
} // namespace kernel
} // namespace sti
