#pragma once

#include <cstdint>
#include <memory>

namespace sti
{
namespace detail
{
namespace image
{

/* function we use to 'normalize' a image into the domain we do our processing
 * on.
 * Currently we only support:
 * - types 'byte -> float'
 * - types 'byte -> double'
 * - 'byte pass through'.
 */
template <typename T>
static void normalize_image(const uint8_t *src, T *dst, const int width, const int height)
{
    const T factor = static_cast<T>(1.0) / static_cast<T>(255.0);
    for (auto i = 0; i < width * height; ++i)
        dst[i] = src[i] * factor;
}

/* template specialization for the 'byte pass through mode */
template <>
static void normalize_image<uint8_t>(const uint8_t *src, uint8_t *dst, const int width, const int height)
{
    std::memmove(dst, src, width * height);
}

/* function to 'denormalize' the image back into the domain we use todo
 * io from.
 *
 * \todo we are currently doing specialization on float and double. But we could
 * get the normalization factor using T. Practically making it similar to the
 * normalization functions.
 */
template <typename T>
static void denormalize_image(const T *src, uint8_t *dst, const int width, const int height)
{
    std::memmove(dst, src, width * height);
}

/* specialization for float -> byte */
template <>
static void denormalize_image<float>(const float *src, uint8_t *dst, const int width, const int height)
{
    const auto factor = 255.f;
    for (auto i = 0; i < width * height; ++i)
        dst[i] = static_cast<uint8_t>(sti::detail::clamp<float>(src[i] * factor, 0.f, 255.f));
}

/* specialization for double -> byte */
template <>
static void denormalize_image<double>(const double *src, uint8_t *dst, const int width, const int height)
{
    const auto factor = 255.0;
    for (auto i = 0; i < width * height; ++i)
        dst[i] = static_cast<uint8_t>(sti::detail::clamp<double>(src[i] * factor, 0.0, 255.0));
}

} // namespace image
} // namespace detail
} // namespace sti
