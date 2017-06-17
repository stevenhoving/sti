#pragma once

#include <limits>
#include <type_traits>

namespace sti
{

template <typename pixel_type_t>
struct pixel_values
{
    static constexpr auto min_color()
    {
        return static_cast<pixel_type_t>(0);
    }

    static constexpr auto max_color()
    {
        return static_cast<pixel_type_t>(std::numeric_limits<pixel_values>::max());
    }

    static constexpr auto default_color()
    {
        return max_color();
    }
};

template <typename pixel_type_t>
struct pixel_values<typename std::enable_if<std::is_floating_point<pixel_type_t>::value>::type>
{
    static constexpr auto min_color()
    {
        return static_cast<pixel_type_t>(0);
    }

    static constexpr auto max_color()
    {
        return static_cast<pixel_type_t>(1.0);
    }

    static constexpr auto default_color()
    {
        return max_color();
    }
};

} // namespace sti
