#pragma once

#include <sti/utility.hpp>
#include <sti/pixel_values.hpp>
#include <cstdint>
#include <memory>

namespace sti
{
namespace detail
{

template <typename pixel_type_t>
static void normalize_image(const uint8_t *src, pixel_type_t *dst, const int width, const int height)
{
    constexpr auto max_src_value = pixel_values<std::uint8_t>::max_value();
    constexpr auto max_dst_value = pixel_values<pixel_type_t>::max_value();

    constexpr auto factor = max_dst_value / max_src_value;
    for (auto i = 0; i < width * height; ++i)
        dst[i] = src[i] * factor;
}

template <>
static void normalize_image<uint8_t>(const uint8_t *src, uint8_t *dst, const int width, const int height)
{
    std::memmove(dst, src, width * height);
}

template <typename T>
static void denormalize_image(const T *src, uint8_t *dst, const int width, const int height)
{
    std::memmove(dst, src, width * height);
}

template <>
static void denormalize_image<float>(const float *src, uint8_t *dst, const int width, const int height)
{
    constexpr auto factor = 255.0f;
    constexpr auto min_value = static_cast<float>(pixel_values<uint8_t>::min_value());
    constexpr auto max_value = static_cast<float>(pixel_values<uint8_t>::max_value());
    for (auto i = 0; i < width * height; ++i)
        dst[i] = static_cast<uint8_t>(utility::clamp<float>(src[i] * factor, min_value, max_value));
}

template <>
static void denormalize_image<double>(const double *src, uint8_t *dst, const int width, const int height)
{
    constexpr auto factor = 255.0;
    constexpr auto min_value = static_cast<float>(pixel_values<uint8_t>::min_value());
    constexpr auto max_value = static_cast<float>(pixel_values<uint8_t>::max_value());
    for (auto i = 0; i < width * height; ++i)
        dst[i] = static_cast<uint8_t>(sti::utility::clamp<double>(src[i] * factor, min_value, max_value));
}

} // namespace detail
} // namespace sti
