#pragma once

#include <cfloat>
#include <cmath>

#include <vector>

namespace sti
{
namespace gaussian
{

// https://www.wolframalpha.com/input/?i=gaussian
template <typename T, int Size>
kernel<T, Size> make_kernel(T Sigma = static_cast<T>(1.0))
{
    T sigma = Sigma;
    T r, s = static_cast<T>(2.0) * sigma * sigma;

    // sum is for normalization
    T sum = 0.0;

    kernel<T, Size> kern;
    for (int i = 0; i < Size * Size; ++i)
    {
        int x = (i % Size) - (Size / 2);
        int y = (i / Size) - (Size / 2);

        // r = static_cast<T>(std::sqrt(x*x + y*y)); // distance from center
        // T value = static_cast<T>((std::exp(-(r*r) / s)) / (M_PI * s));

        r = static_cast<T>(x * x + y * y);
        T value = static_cast<T>((std::exp(-r / s)) / (M_PI * s));

        kern.data[i] = value;
        sum += value;
    }

    for (int i = 0; i < Size * Size; ++i)
        kern.data[i] /= static_cast<T>(sum);

    return kern;
}
} // namespace gaussian
} // namespace sti
