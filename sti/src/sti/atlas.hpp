#pragma once

#include <sti/image.hpp>
#include <sti/image_detail.hpp>

namespace sti
{

template <typename pixel_type_t>
struct atlas
{

    static auto split(const sti::image<pixel_type_t> &image, const int width, const int height)
        -> std::vector<sti::image<pixel_type_t>>;
};

template <typename pixel_type_t>
auto atlas<pixel_type_t>::split(const sti::image<pixel_type_t> &image, const int width, const int height)
    -> std::vector<sti::image<pixel_type_t>>
{
    std::vector<sti::image<pixel_type_t>> result;

    const auto source_width = image.width();
    const auto source_height = image.height();
    const auto source_plane_count = image.plane_count();

    const auto horizontal_count = source_width / width;
    const auto vertical_count = source_height / height;

    for (auto y = 0; y < vertical_count; ++y)
    {
        for (auto x = 0; x < horizontal_count; ++x)
        {
            result.emplace_back(detail<pixel_type_t>::crop(image, x * width, y * height, width, height));
        }
    }

    return result;
}

} // namespace sti
