#pragma once

#include <sti/plane.hpp>

namespace sti
{
template <typename pixel_type_t>
void fill(sti::plane<pixel_type_t> &plane, const pixel_type_t color)
{
    std::fill(std::begin(plane), std::end(plane), color);
}

} // namespace sti
