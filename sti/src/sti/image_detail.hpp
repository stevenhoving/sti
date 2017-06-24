#pragma once

#include <sti/image.hpp>
#include <sti/pixel_values.hpp>
#include <functional>
#include <cassert>
#include <cstring>

namespace sti
{

template <typename pixel_type_t>
struct blit_operator
{
    static auto add(const pixel_type_t source, const pixel_type_t source_alpha, const pixel_type_t destination,
                    const pixel_type_t destination_alpha) -> pixel_type_t;
    static auto multiply(const pixel_type_t source, const pixel_type_t source_alpha, const pixel_type_t destination,
                         const pixel_type_t destination_alpha) -> pixel_type_t;
    static auto overlay(const pixel_type_t source, const pixel_type_t source_alpha, const pixel_type_t destination,
                        const pixel_type_t destination_alpha) -> pixel_type_t;
    static auto screen(const pixel_type_t source, const pixel_type_t source_alpha, const pixel_type_t destination,
                       const pixel_type_t destination_alpha) -> pixel_type_t;
    static auto blend_alpha(const pixel_type_t source, const pixel_type_t source_alpha, const pixel_type_t destination,
                            const pixel_type_t destination_alpha) -> pixel_type_t;
    static auto blend_one_minus_alpha(const pixel_type_t source, const pixel_type_t source_alpha,
                                      const pixel_type_t destination, const pixel_type_t destination_alpha)
        -> pixel_type_t;
    static auto blend_alpha_ratio(const pixel_type_t source, const pixel_type_t source_alpha,
                                  const pixel_type_t destination, const pixel_type_t destination_alpha) -> pixel_type_t;
};

template <typename pixel_type_t>
auto blit_operator<pixel_type_t>::add(const pixel_type_t source, const pixel_type_t source_alpha,
                                      const pixel_type_t destination, const pixel_type_t destination_alpha)
    -> pixel_type_t
{
    return destination + source;
}

template <typename pixel_type_t>
auto blit_operator<pixel_type_t>::multiply(const pixel_type_t source, const pixel_type_t source_alpha,
                                           const pixel_type_t destination, const pixel_type_t destination_alpha)
    -> pixel_type_t
{
    return destination * source;
}

template <typename pixel_type_t>
auto blit_operator<pixel_type_t>::overlay(const pixel_type_t source, const pixel_type_t source_alpha,
                                          const pixel_type_t destination, const pixel_type_t destination_alpha)
    -> pixel_type_t
{
    return destination;
}

template <typename pixel_type_t>
auto blit_operator<pixel_type_t>::screen(const pixel_type_t source, const pixel_type_t source_alpha,
                                         const pixel_type_t destination, const pixel_type_t destination_alpha)
    -> pixel_type_t
{
    const auto max = pixel_values<pixel_type_t>::max_value();
    return max - ((max - destination) * (max - source));
}

template <typename pixel_type_t>
auto blit_operator<pixel_type_t>::blend_alpha(const pixel_type_t source, const pixel_type_t source_alpha,
                                              const pixel_type_t destination, const pixel_type_t destination_alpha)
    -> pixel_type_t
{
    return destination + (source * source_alpha);
}

template <typename pixel_type_t>
auto blit_operator<pixel_type_t>::blend_one_minus_alpha(const pixel_type_t source, const pixel_type_t source_alpha,
                                                        const pixel_type_t destination,
                                                        const pixel_type_t destination_alpha) -> pixel_type_t
{
    const auto max = pixel_values<pixel_type_t>::max_value();
    return destination + (source * (max - source_alpha));
}

template <typename pixel_type_t>
auto blit_operator<pixel_type_t>::blend_alpha_ratio(const pixel_type_t source, const pixel_type_t source_alpha,
                                                    const pixel_type_t destination,
                                                    const pixel_type_t destination_alpha) -> pixel_type_t
{
    const auto min = pixel_values<pixel_type_t>::min_value();
    const auto max = pixel_values<pixel_type_t>::max_value();
    const auto range = max - min;
    const auto ratio = source_alpha / range;
    const auto ratio_min_one = max - ratio;
    return (source * ratio) + (destination * ratio_min_one);
}

template <typename pixel_type_t>
struct detail
{
    static void blit(sti::image<pixel_type_t> &destination, const int dest_x, const int dest_y,
                     const sti::image<pixel_type_t> &source);
    static void blit(sti::image<pixel_type_t> &destination, const int dest_x, const int dest_y, const int width,
                     const int height, const sti::image<pixel_type_t> &source, const int source_x, const int source_y);

    static void blit(sti::image<pixel_type_t> &destination, const int dest_x, const int dest_y,
                     const sti::image<pixel_type_t> &source,
                     std::function<pixel_type_t(pixel_type_t, pixel_type_t, pixel_type_t, pixel_type_t)> operation);

    static void blit(sti::image<pixel_type_t> &destination, const int dest_x, const int dest_y, const int width,
                     const int height, const sti::image<pixel_type_t> &source, const int source_x, const int source_y,
                     std::function<pixel_type_t(pixel_type_t, pixel_type_t, pixel_type_t, pixel_type_t)> operation);
    static auto crop(const sti::image<pixel_type_t> &image, const int x, const int y, const int width, const int height)
        -> sti::image<pixel_type_t>;
};

template <typename pixel_type_t>
void detail<pixel_type_t>::blit(sti::image<pixel_type_t> &destination, const int dest_x, const int dest_y,
                                const sti::image<pixel_type_t> &source)
{
    blit(destination, dest_x, dest_y, source.width(), source.height(), source, 0, 0);
}

template <typename pixel_type_t>
void detail<pixel_type_t>::blit(sti::image<pixel_type_t> &destination, const int dest_x, const int dest_y,
                                const int width, const int height, const sti::image<pixel_type_t> &source,
                                const int source_x, const int source_y)
{
    assert(source_x + width <= source.width());
    assert(source_y + height <= source.height());
    assert(dest_x + width <= destination.width());
    assert(dest_y + height <= destination.height());
    assert(source.plane_count() == destination.plane_count());

    const auto plane_count = destination.plane_count();
    const auto source_stride = source.stride();
    const auto dest_stride = destination.stride();

    for (auto scanline = 0; scanline < height; ++scanline)
    {
        const int source_y_offset = source_y + scanline;
        const int dest_y_offset = dest_y + scanline;

        for (auto plane = 0; plane < plane_count; ++plane)
        {
            const auto dest_offset = (dest_y_offset * dest_stride) + dest_x;
            auto dest_data = &destination.get_plane(plane).data()[dest_offset];
            const auto source_offset = (source_y_offset * source_stride) + source_x;
            const auto source_data = &source.get_plane(plane).data()[source_offset];

            std::memcpy(dest_data, source_data, width * sizeof(pixel_type_t));
        }
    }
}

template <typename pixel_type_t>
void detail<pixel_type_t>::blit(
    sti::image<pixel_type_t> &destination, const int dest_x, const int dest_y, const sti::image<pixel_type_t> &source,
    std::function<pixel_type_t(pixel_type_t, pixel_type_t, pixel_type_t, pixel_type_t)> operation)
{
    blit(destination, dest_x, dest_y, source.width(), source.height(), source, 0, 0, operation);
}

template <typename pixel_type_t>
void detail<pixel_type_t>::blit(
    sti::image<pixel_type_t> &destination, const int dest_x, const int dest_y, const int width, const int height,
    const sti::image<pixel_type_t> &source, const int source_x, const int source_y,
    std::function<pixel_type_t(pixel_type_t, pixel_type_t, pixel_type_t, pixel_type_t)> operation)
{
    assert(source_x + width <= source.width());
    assert(source_y + height <= source.height());
    assert(dest_x + width <= destination.width());
    assert(dest_y + height <= destination.height());
    assert(source.plane_count() == destination.plane_count());
    assert(source.plane_count() == 4); // blitting with an operation requires an alpha channel.

    const auto plane_count = destination.plane_count() - 1;
    const auto alpha_plane_index = 3;
    const auto source_stride = source.stride();
    const auto dest_stride = destination.stride();

    for (auto scanline = 0; scanline < height; ++scanline)
    {
        const int source_y_offset = source_y + scanline;
        const int dest_y_offset = dest_y + scanline;

        const auto dest_offset = (dest_y_offset * dest_stride) + dest_x;
        auto dest_alpha_data = &destination.get_plane(alpha_plane_index).data()[dest_offset];
        const auto source_offset = (source_y_offset * source_stride) + source_x;
        const auto source_alpha_data = &source.get_plane(alpha_plane_index).data()[source_offset];

        for (auto plane = 0; plane < plane_count; ++plane)
        {
            auto dest_data = &destination.get_plane(plane).data()[dest_offset];
            const auto source_data = &source.get_plane(plane).data()[source_offset];

            for (auto x = 0; x < width; ++x)
            {
                dest_data[x] = operation(source_data[x], source_alpha_data[x], dest_data[x], dest_alpha_data[x]);
            }
        }
    }
}

template <typename pixel_type_t>
auto detail<pixel_type_t>::crop(const sti::image<pixel_type_t> &image, const int x, const int y, const int width,
                                const int height) -> sti::image<pixel_type_t>
{
    assert(x + width <= image.width());
    assert(y + height <= image.height());

    const auto source_stride = image.stride();
    const auto plane_count = image.plane_count();
    const int x_offset = x;

    sti::image<pixel_type_t> result(width, height, plane_count);

    for (auto scanline = 0; scanline < height; ++scanline)
    {
        const int y_offset = y + scanline;
        for (auto plane = 0; plane < plane_count; ++plane)
        {
            const auto dest_offset = scanline * width;
            auto dest = &result.get_plane(plane).data()[dest_offset];
            const auto source_offset = (y_offset * source_stride) + x_offset;
            const auto source = &image.get_plane(plane).data()[source_offset];

            std::memcpy(dest, source, width * sizeof(pixel_type_t));
        }
    }

    return result;
}

// namespace detail
//{

/*
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
}*/

//} // namespace detail
} // namespace sti
