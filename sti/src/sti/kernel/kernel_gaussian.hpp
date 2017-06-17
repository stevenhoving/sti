#pragma once

#include <sti/kernel/kernel.hpp>
#include <sti/math/math_constants.hpp>
#include <cmath>

namespace sti
{
namespace kernel
{
namespace gaussian
{

// https://www.wolframalpha.com/input/?i=gaussian
template <typename T, int Size>
kernel<T, Size> make_kernel(T Sigma = static_cast<T>(1.0))
{
    T sigma = Sigma;
    T s = static_cast<T>(2.0) * sigma * sigma;

    // sum is for normalization
    T sum = 0.0;

    kernel<T, Size> kern;
    for (int i = 0; i < Size * Size; ++i)
    {
        int x = (i % Size) - (Size / 2);
        int y = (i / Size) - (Size / 2);

        // r = static_cast<T>(std::sqrt(x*x + y*y)); // distance from center
        // T value = static_cast<T>((std::exp(-(r*r) / s)) / (M_PI * s));
        const auto r = static_cast<T>(x * x + y * y);
        T value = static_cast<T>((std::exp(-r / s)) / (sti::math::pi * s));

        kern[y + (Size/2)][x + (Size / 2)] = value;
        sum += value;
    }

    for (auto &itr : kern)
        itr /= static_cast<T>(sum);

    return kern;
}

} // namespace gaussian
} // namespace kernel
} // namespace sti
