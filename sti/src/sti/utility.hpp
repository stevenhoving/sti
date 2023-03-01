#pragma once

#include <algorithm>

namespace sti
{

struct utility
{
    template <typename T>
    static constexpr auto clamp(const T value, const T min_value, const T max_value) noexcept
    {
        return std::min<T>(std::max<T>(value, min_value), max_value);
    }

    template <typename T>
    static constexpr auto align_up(const T size, const T align) noexcept
    {
        return ((size + (align - 1)) & ~(align - 1));
    }
};

} // namespace sti
