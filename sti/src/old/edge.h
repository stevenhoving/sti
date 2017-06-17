#pragma once

namespace sti
{
namespace edge
{
template <typename T, int Size>
kernel<T, Size> make_kernel()
{
    kernel<T, Size> kern;
    for (auto &itr : kern.data)
        itr = static_cast<T>(-1.0);

    kern[Size / 2][Size / 2] = static_cast<T>(Size * Size) - static_cast<T>(1.0);
    return kern;
}

} // namespace edge
} // namespace sti
