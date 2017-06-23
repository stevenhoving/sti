#pragma once

#include <sti/image.hpp>
#include <sti/color_image.hpp>
#include <sti/pixel_values.hpp>
#include <sti/utility.hpp>

namespace sti
{

template <typename pixel_type_t>
class convert_image
{
public:
    static auto from_color_image(const color_image &img) -> image<pixel_type_t>;
    static auto to_color_image(const image<pixel_type_t> &image);

private:
    static auto from_color_image_internal(const color_image &img, const int plane_count) -> image<pixel_type_t>;
    static auto to_color_image_internal(const image<pixel_type_t> &image, const color_image_format format);
};

template <typename pixel_type_t>
auto convert_image<pixel_type_t>::from_color_image(const color_image &img) -> image<pixel_type_t>
{
    switch (img.info().format)
    {
        case color_image_format::RGB:
            return from_color_image_internal(img, 3);
        case color_image_format::RGBA:
            return from_color_image_internal(img, 4);
        case color_image_format::BGR:
        case color_image_format::YUV420:
        case color_image_format::invalid:
        default:
            throw std::runtime_error("Unsupported image format.");
    }
}

template <typename pixel_type_t>
auto convert_image<pixel_type_t>::to_color_image(const image<pixel_type_t> &image)
{
    switch (image.plane_count())
    {
        case 3:
            return to_color_image_internal(image, color_image_format::RGB);
        case 4:
            return to_color_image_internal(image, color_image_format::RGBA);
        default:
            throw std::runtime_error("Unsupported image format.");
    }
}

template <typename pixel_type_t>
auto convert_image<pixel_type_t>::from_color_image_internal(const color_image &img, const int plane_count)
    -> image<pixel_type_t>
{
    auto &info = img.info();
    auto result = image<pixel_type_t>(info.width, info.height, info.stride / plane_count, plane_count);

    const auto source = img.data();
    const auto source_size = info.height * info.stride;

    auto &planes = result.get_planes();

    constexpr auto max_src_value = pixel_values<color_image::pixel_type_t>::max_value();
    constexpr auto max_dst_value = pixel_values<pixel_type_t>::max_value();
    const auto factor = double(max_dst_value) / max_src_value;

    auto index = 0;
    for (auto offset = 0; offset < source_size; offset += plane_count)
    {
        for (auto plane = 0; plane < plane_count; ++plane)
        {
            planes[plane][index] = static_cast<pixel_type_t>(static_cast<double>(source[offset + plane]) * factor);
        }
        ++index;
    }

    return result;
}

template <typename pixel_type_t>
auto convert_image<pixel_type_t>::to_color_image_internal(const image<pixel_type_t> &image,
                                                          const color_image_format format)
{
    constexpr auto bits = (sizeof(color_image::pixel_type_t) << 3);
    const auto plane_count = image.plane_count();

    auto info = image_info(image.width(), image.height(), bits, image.stride() * plane_count, format);
    auto result = color_image(info);

    auto destination = result.data();
    const auto destination_size = info.height * info.stride;

    auto &planes = image.get_planes();

    constexpr auto max_src_value = pixel_values<pixel_type_t>::max_value();
    constexpr auto min_dst_value = static_cast<double>(pixel_values<color_image::pixel_type_t>::min_value());
    constexpr auto max_dst_value = static_cast<double>(pixel_values<color_image::pixel_type_t>::max_value());
    const auto factor = max_dst_value / max_src_value;

    auto index = 0;
    for (auto offset = 0; offset < destination_size; offset += plane_count)
    {
        for (auto plane = 0; plane < plane_count; ++plane)
        {
            const auto plx = utility::clamp(planes[plane][index] * factor, min_dst_value, max_dst_value);
            destination[offset + plane] = static_cast<color_image::pixel_type_t>(plx);
        }

        ++index;
    }

    return result;
}

} // namespace sti
