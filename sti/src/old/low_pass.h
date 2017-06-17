#pragma once

namespace sti
{
namespace low_pass
{

template <typename T, int Size>
kernel<T, Size> make_kernel()
{
    kernel<T, Size> kern;
    for (auto &itr : kern.data)
        itr = static_cast<T>(1.0 / static_cast<T>(Size * Size));
    return kern;
}

} // namespace low_pass
} // namespace sti
