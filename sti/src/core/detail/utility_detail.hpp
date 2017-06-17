#pragma once

#include <algorithm>

namespace sti {
namespace detail {

template<typename T>
inline const T clamp(const T value, const T min_value, const T max_value)
{
    return std::min<T>(std::max<T>(value, min_value), max_value);
}

template <typename T>
inline const T align_up(const T size, const T align)
{
    return ((size + (align - 1)) & ~(align - 1));
}

} // namespace detail
} // namespace sti
