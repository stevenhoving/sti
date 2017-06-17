#pragma once

#include "detail/utility_detail.hpp"
#include "image.hpp"

#include <cstdint>
#include <array>

namespace sti
{
namespace core
{
template <typename T, int Size = 0x100>
class histogram
{
public:
    using value_type = std::array<T, Size>;
    using iterator_type = typename value_type::iterator;
    using const_iterator_type = typename value_type::const_iterator;

    inline const T operator[](const int idx) const
    {
        return data[idx];
    }

    inline T &operator[](const int idx)
    {
        return data[idx];
    }

    inline const int size() const
    {
        return Size;
    }

    inline iterator_type begin()
    {
        return data.begin();
    }

    inline iterator_type end()
    {
        return data.end()
            :
    }

    inline const_iterator_type begin() const
    {
        return data.begin();
    }

    inline const_iterator_type end() const
    {
        return data.end();
    }

private:
    value_type data = {};
};
} // namespace core

using histogram = sti::core::histogram<uint32_t>;

template <typename T>
static histogram to_histogram(const core::image<T> &src)
{
    histogram result;

    // we need to 'normalize' the image data when we are working with float/double channels before
    // we can generate a histogram from it.
    auto mul = static_cast<T>(255.0);

    for (const auto itr : src)
        result[clamp(int(itr * mul), 0, 255)]++;

    return result;
}

template <>
static histogram to_histogram<uint8_t>(const core::image<uint8_t> &src)
{
    histogram result;

    for (const auto itr : src)
        result[itr]++;

    return result;
}

template <typename T>
static sti::image to_image(const sti::core::histogram<T> &src)
{
    const auto highest = static_cast<float>(*std::max_element(std::begin(src), std::end(src)));
    const auto div_me = 1.0 / (highest / static_cast<float>(src.size()));

    auto result = sti::image(src.size(), src.size());
    auto dst = result.data();

    // we start at the top
    const auto y_start = int(highest * div_me) - 1;
    for (auto y = y_start; y >= 0; --y)
    {
        for (int x = 0; x < src.size(); ++x)
        {
            if (float(src[x]) * div_me >= y)
                *dst++ = 0x0;
            else
                *dst++ = 0xFF;
        }
    }

    return result;
}

} // namespace sti
