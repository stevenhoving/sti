#pragma once

namespace sti
{
namespace high_pass
{

template<typename T, int Size>
kernel<T, Size> make_kernel()
{
    kernel<T, Size> kern;
    for (auto &itr : kern.data)
        itr = static_cast<T>(-1.0);

    kern[Size / 2][Size / 2] = (Size * Size) - 1;
    return kern;
}

} // namespace high_pass
} // namespace sti