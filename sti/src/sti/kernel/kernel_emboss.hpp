#pragma once

#include <sti/pixel_values.hpp>

namespace sti
{
namespace kernel
{
namespace emboss
{
/* Example of a 3x3 emboss kernel
 -1 -1  0
 -1  0  1
  0  1  1

  Example of a 5x5 emboss kernel
 -1 -1 -1 -1  0
 -1 -1 -1  0  1
 -1 -1  0  1  1
 -1  0  1  1  1
  0  1  1  1  1
*/

template <typename T, int Size>
kernel<T, Size> make_kernel()
{
    kernel<T, Size> kern;
    int diagonal_line = Size - 1;
    for (auto y = 0; y < Size; ++y)
    {
        for (auto x = 0; x < Size; ++x)
        {
            if (x < diagonal_line)
                kern[x][y] = -1;
            else if (x == diagonal_line)
                kern[x][y] = 0;
            else
                kern[x][y] = 1;
        }
        diagonal_line--;
    }

    kern.factor = 1.0;
    kern.offset = pixel_values<T>::center_value();
    return kern;
}

} // namespace emboss
} // namespace kernel
} // namespace sti
