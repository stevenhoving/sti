#pragma once

#include <sti/image.hpp>
#include <sti/color_image.hpp>
#include <sti/pixel_values.hpp>
#include <sti/utility.hpp>

namespace sti
{

template <typename pixel_type_t, int slice_count_t>
class convert_image
{
public:
    static auto from_color_image(const color_image &img) -> image<pixel_type_t, slice_count_t>;
    static auto to_color_image(const image<pixel_type_t, 3> &image);
    static auto to_color_image(const image<pixel_type_t, 4> &image);

private:
    static auto from_color_image_rgb(const color_image &img) -> image<pixel_type_t, slice_count_t>;
    static auto from_color_image_rgba(const color_image &img) -> image<pixel_type_t, slice_count_t>;
};

template <typename pixel_type_t, int slice_count_t>
auto convert_image<pixel_type_t, slice_count_t>::from_color_image(const color_image &img)
    -> image<pixel_type_t, slice_count_t>
{
    switch (img.info().format)
    {
        case color_image_format::RGB:
            return from_color_image_rgb(img);
        case color_image_format::RGBA:
            return from_color_image_rgba(img);
        case color_image_format::BGR:
        case color_image_format::YUV420:
        case color_image_format::invalid:
        default:
            throw std::runtime_error("Unsupported image format.");
    }
}

template <typename pixel_type_t, int slice_count_t>
auto convert_image<pixel_type_t, slice_count_t>::to_color_image(const image<pixel_type_t, 3> &image)
{
    constexpr auto bits = (sizeof(color_image::pixel_type_t) << 3);
    auto info = image_info(image.width(), image.height(), bits, image.stride() * slice_count_t, color_image_format::RGB);
    auto result = color_image(info);

    auto destination = static_cast<pixel_type_t *>(result.data());
    const auto destination_size = info.height * info.stride;

    auto src1 = image.get_slice(0).data();
    auto src2 = image.get_slice(1).data();
    auto src3 = image.get_slice(2).data();

    constexpr auto max_src_value = pixel_values<pixel_type_t>::max_value();
    constexpr auto min_dst_value = static_cast<double>(pixel_values<color_image::pixel_type_t>::min_value());
    constexpr auto max_dst_value = static_cast<double>(pixel_values<color_image::pixel_type_t>::max_value());
    const auto factor = max_dst_value / max_src_value;

    auto index = 0;
    for (auto offset = 0; offset < destination_size; offset += slice_count_t)
    {
        const auto pxl1 = utility::clamp(src1[index] * factor, min_dst_value, max_dst_value);
        const auto pxl2 = utility::clamp(src2[index] * factor, min_dst_value, max_dst_value);
        const auto pxl3 = utility::clamp(src3[index] * factor, min_dst_value, max_dst_value);

        destination[offset + 0] = static_cast<color_image::pixel_type_t>(pxl1);
        destination[offset + 1] = static_cast<color_image::pixel_type_t>(pxl2);
        destination[offset + 2] = static_cast<color_image::pixel_type_t>(pxl3);

        ++index;
    }

    return result;
}

template <typename pixel_type_t, int slice_count_t>
auto convert_image<pixel_type_t, slice_count_t>::to_color_image(const image<pixel_type_t, 4> &image)
{
    constexpr auto bits = (sizeof(color_image::pixel_type_t) << 3);
    auto info =
        image_info(image.width(), image.height(), bits, image.stride() * slice_count_t, color_image_format::RGBA);
    auto result = color_image(info);

    auto destination = result.data();
    const auto destination_size = info.height * info.stride;

    auto src1 = image.get_slice(0).data();
    auto src2 = image.get_slice(1).data();
    auto src3 = image.get_slice(2).data();
    auto src4 = image.get_slice(3).data();

    constexpr auto max_src_value = pixel_values<pixel_type_t>::max_value();
    constexpr auto min_dst_value = static_cast<double>(pixel_values<color_image::pixel_type_t>::min_value());
    constexpr auto max_dst_value = static_cast<double>(pixel_values<color_image::pixel_type_t>::max_value());
    const auto factor = max_dst_value / max_src_value;

    auto index = 0;
    for (auto offset = 0; offset < destination_size; offset += slice_count_t)
    {
        const auto pxl1 = utility::clamp(src1[index] * factor, min_dst_value, max_dst_value);
        const auto pxl2 = utility::clamp(src2[index] * factor, min_dst_value, max_dst_value);
        const auto pxl3 = utility::clamp(src3[index] * factor, min_dst_value, max_dst_value);
        const auto pxl4 = utility::clamp(src4[index] * factor, min_dst_value, max_dst_value);

        destination[offset + 0] = static_cast<color_image::pixel_type_t>(pxl1);
        destination[offset + 1] = static_cast<color_image::pixel_type_t>(pxl2);
        destination[offset + 2] = static_cast<color_image::pixel_type_t>(pxl3);
        destination[offset + 3] = static_cast<color_image::pixel_type_t>(pxl4);

        ++index;
    }

    return result;
}

template <typename pixel_type_t, int slice_count_t>
auto convert_image<pixel_type_t, slice_count_t>::from_color_image_rgb(const color_image &img)
    -> image<pixel_type_t, slice_count_t>
{
    auto &info = img.info();
    auto result = image<pixel_type_t, slice_count_t>(info.width, info.height, info.stride / 3);

    const auto source = img.data();
    const auto source_size = info.height * info.stride;

    auto dest1 = result.get_slice(0).data();
    auto dest2 = result.get_slice(1).data();
    auto dest3 = result.get_slice(2).data();

    constexpr auto max_src_value = pixel_values<color_image::pixel_type_t>::max_value();
    constexpr auto max_dst_value = pixel_values<pixel_type_t>::max_value();
    const auto factor = double(max_dst_value) / max_src_value;

    auto index = 0;
    for (auto offset = 0; offset < source_size; offset += 3)
    {
        dest1[index] = static_cast<pixel_type_t>(static_cast<double>(source[offset + 0]) * factor);
        dest2[index] = static_cast<pixel_type_t>(static_cast<double>(source[offset + 1]) * factor);
        dest3[index] = static_cast<pixel_type_t>(static_cast<double>(source[offset + 2]) * factor);
    }

    return result;
}

template <typename pixel_type_t, int slice_count_t>
auto convert_image<pixel_type_t, slice_count_t>::from_color_image_rgba(const color_image &img)
    -> image<pixel_type_t, slice_count_t>
{
    auto &info = img.info();
    auto result = image<pixel_type_t, slice_count_t>(info.width, info.height, info.stride / 4);

    const auto source = img.data();
    const auto source_size = info.height * info.stride;

    auto dest1 = result.get_slice(0).data();
    auto dest2 = result.get_slice(1).data();
    auto dest3 = result.get_slice(2).data();
    auto dest4 = result.get_slice(3).data();

    constexpr auto max_src_value = pixel_values<color_image::pixel_type_t>::max_value();
    constexpr auto max_dst_value = pixel_values<pixel_type_t>::max_value();
    const auto factor = double(max_dst_value) / max_src_value;

    auto index = 0;
    for (auto offset = 0; offset < source_size; offset += 4)
    {
        dest1[index] = static_cast<pixel_type_t>(static_cast<double>(source[offset + 0]) * factor);
        dest2[index] = static_cast<pixel_type_t>(static_cast<double>(source[offset + 1]) * factor);
        dest3[index] = static_cast<pixel_type_t>(static_cast<double>(source[offset + 2]) * factor);
        dest4[index] = static_cast<pixel_type_t>(static_cast<double>(source[offset + 3]) * factor);
        ++index;
    }

    return result;
}

} // namespace sti
