#pragma once

#include "image.h"
#include "memory_view.h"

#include <cstdint>
#include <array>

namespace sti
{
template <typename T>
class histogram_core
{
public:
    using value_type = std::array<T, 0x100>;
    using iterator_type = typename value_type::const_iterator;

    inline T operator[](const int idx) const
    {
        return data[idx];
    }

    inline T &operator[](const int idx)
    {
        return data[idx];
    }

    inline iterator_type begin() const
    {
        return data.begin();
    }

    inline iterator_type end() const
    {
        return data.end();
    }
    value_type data = {};
};

using histogram = histogram_core<uint32_t>;

template <typename T>
histogram to_histogram(const image<T> &src)
{
    histogram result;

    for (auto itr : src)
        result.data[int(itr * static_cast<T>(255.0))]++;

    return result;
}

template <>
histogram to_histogram<uint8_t>(const image<uint8_t> &src)
{
    histogram result;

    for (auto itr : src)
        result.data[itr]++;

    return result;
}

#if 0
template<typename T>
class histogram_base
{
public:
    inline T operator[](const int idx) const
    {
        return data[idx];
    }

    inline T &operator[](const int idx)
    {
        return data[idx];
    }

    inline basic_memory_view<T> view() {return sti::basic_memory_view<T>(data.data(), data.size());}

    std::array<T, 0x100> data = {};
};

using histogram = sti::histogram_base<uint32_t>;

template<typename T>
histogram to_histogram(const sti::image<T> &src)
{
    histogram result;

    for (auto itr : src)
        result.data[int(itr * static_cast<T>(255.0))]++;

    return result;
}

template<>
histogram to_histogram<uint8_t>(const sti::image<uint8_t> &src)
{
    histogram result;

    for (auto itr : src)
        result.data[itr]++;

    return result;
}
#endif

} // namespace sti
