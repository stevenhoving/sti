#pragma once

#include <cstdint>
#include <memory>

namespace sti {
namespace detail {
namespace image {

template<typename T>
static void normalize_image(const uint8_t *src, T *dst, const int width,
    const int height)
{
    const T henk = static_cast<T>(1.0) / static_cast<T>(255.0);
    for (auto i = 0; i < width * height; ++i)
        dst[i] = src[i] * henk;
}

template<>
static void normalize_image<uint8_t>(const uint8_t *src, uint8_t *dst,
                                     const int width, const int height)
{
    std::memmove(dst, src, width * height);
}

template<typename T>
static void denormalize_image(const T *src, uint8_t *dst, const int width,
                              const int height)
{
    std::memmove(dst, src, width * height);
}

template<>
static void denormalize_image<float>(const float *src, uint8_t *dst,
                                     const int width, const int height)
{
    const auto henk = 255.f;
    for (auto i = 0; i < width * height; ++i)
        // \todo fix missing clamp here
        dst[i] = static_cast<uint8_t>(src[i] * henk);
}

template<>
static void denormalize_image<double>(const double *src, uint8_t *dst,
                                      const int width, const int height)
{
    const auto henk = 255.0;
    for (auto i = 0; i < width * height; ++i)
        // \todo fix missing clamp here
        dst[i] = static_cast<uint8_t>(src[i] * henk);
}

/* handle float/double */
template<typename T>
constexpr T pixel_default_value() { return static_cast<T>(1.0); }

/* handle 8 bit integer */
template<>
constexpr uint8_t pixel_default_value<uint8_t>() { return 255; }

/* handle 16 bit integer */
template<>
constexpr uint16_t pixel_default_value<uint16_t>() { return 65535; }

/* handle 32 bit integer */
template<>
constexpr uint32_t pixel_default_value<uint32_t>() { return 4294967295; }

/* pixel value limits */
template <typename T>
constexpr T pixel_min_value() { return static_cast<T>(0.0); }

template <>
constexpr uint8_t pixel_min_value<uint8_t>() { return 0; }

template <typename T>
constexpr T pixel_max_value() { return static_cast<T>(1.0); }

template <>
constexpr uint8_t pixel_max_value<uint8_t>() { return 0xFF; }

} // namespace image
} // namespace detail
} // namespace sti
