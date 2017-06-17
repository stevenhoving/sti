#pragma once

#include <limits>

namespace sti
{

// TODO: Fix me with enable_if!

template <typename pixel_type_t>
struct pixel_values
{
    static constexpr auto min_color()
    {
        return static_cast<pixel_type_t>(0);
    }

    static constexpr auto max_color()
    {
        return static_cast<pixel_type_t>(std::numeric_limits<pixel_type_t>::max());
    }

    static constexpr auto default_color()
    {
        return max_color();
    }
};

template <>
struct pixel_values<float>
{
    static constexpr auto min_color()
    {
        return static_cast<float>(0);
    }

    static constexpr auto max_color()
    {
        return static_cast<float>(1.0);
    }

    static constexpr auto default_color()
    {
        return max_color();
    }
};

} // namespace sti
