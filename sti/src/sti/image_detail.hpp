#pragma once

#include <cstdint>
#include <type_traits>
#include <memory>

namespace sti
{
namespace detail
{

template <typename pixel_type_t>
static void normalize_image(const uint8_t *src, pixel_type_t *dst, const int width, const int height)
{
    const auto henk = static_cast<pixel_type_t>(1.0) / static_cast<pixel_type_t>(255.0);
    for (auto i = 0; i < width * height; ++i)
        dst[i] = src[i] * henk;
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
    const auto henk = 255.f;
    for (auto i = 0; i < width * height; ++i)
        // \todo fix missing clamp here
        dst[i] = static_cast<uint8_t>(src[i] * henk);
}

template <>
static void denormalize_image<double>(const double *src, uint8_t *dst, const int width, const int height)
{
    const auto henk = 255.0;
    for (auto i = 0; i < width * height; ++i)
        // \todo fix missing clamp here
        dst[i] = static_cast<uint8_t>(src[i] * henk);
}

} // namespace detail
} // namespace sti
