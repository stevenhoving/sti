#pragma once

#include <sti/image.hpp>
#include <sti/color_image.hpp>

namespace sti
{

template <typename pixel_type_t, int slice_count_t>
class convert_image
{
public:
    static auto from_color_image(const color_image &img) -> image<pixel_type_t, slice_count_t>;

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
auto convert_image<pixel_type_t, slice_count_t>::from_color_image_rgb(const color_image &img)
    -> image<pixel_type_t, slice_count_t>
{
    auto &info = img.info();
    auto result = image<pixel_type_t, slice_count_t>(info.width, info.height, info.stride);

    const auto source = img.data();
    const auto source_size = img.size();

    auto dest1 = result.get_slice(0).data();
    auto dest2 = result.get_slice(1).data();
    auto dest3 = result.get_slice(2).data();

    auto index = 0;
    for (auto offset = 0; offset < source_size; offset += 3)
    {
        dest1[index] = source[offset];
        dest2[index] = source[offset + 1];
        dest3[index] = source[offset + 2];
    }

    return result;
}

template <typename pixel_type_t, int slice_count_t>
auto convert_image<pixel_type_t, slice_count_t>::from_color_image_rgba(const color_image &img)
    -> image<pixel_type_t, slice_count_t>
{
    auto &info = img.info();
    auto result = image<pixel_type_t, slice_count_t>(info.width, info.height, info.stride);

    const auto source = img.data();
    const auto source_size = img.size();

    auto dest1 = result.get_slice(0).data();
    auto dest2 = result.get_slice(1).data();
    auto dest3 = result.get_slice(2).data();
    auto dest4 = result.get_slice(3).data();

    auto index = 0;
    for (auto offset = 0; offset < source_size; offset += 4)
    {
        dest1[index] = source[offset];
        dest2[index] = source[offset + 1];
        dest3[index] = source[offset + 2];
        dest4[index] = source[offset + 3];
        ++index;
    }

    return result;
}

} // namespace sti
