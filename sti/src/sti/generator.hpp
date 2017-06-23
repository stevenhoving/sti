#pragma once

#include <sti/plane.hpp>
#include <sti/image.hpp>
#include <sti/pixel_values.hpp>
#include <array>
#include <vector>

namespace sti
{

template <typename pixel_type_t>
struct generator
{
    static auto generate_checker_board_plane(const int width, const int height, const int rows, const int cols,
                                             const pixel_type_t color1 = pixel_values<pixel_type_t>::min_value(),
                                             const pixel_type_t color2 = pixel_values<pixel_type_t>::max_value())
    {
        auto result = plane<pixel_type_t>(width, height);

        const auto row_width = result.width() / rows;
        const auto col_height = result.height() / cols;
        const auto data_size = result.width() * result.height();
        const std::array<pixel_type_t, 2> colors = {color1, color2};

        for (auto i = 0; i < data_size; i += row_width)
        {
            auto chunk = &result[i];

            int vertical_change = (i / (width * col_height)) & 1;
            int horizontal_change = (i / row_width) & 1;

            int color_index = horizontal_change ^ vertical_change;
            std::fill(chunk, chunk + row_width, colors[color_index]);
        }

        return result;
    }

    template <int plane_count>
    static auto generate_checker_board_image(const int width, const int height, const int rows, const int cols,
                                             const std::array<pixel_type_t, plane_count> color1,
                                             const std::array<pixel_type_t, plane_count> color2)
    {
        std::vector<plane<pixel_type_t>> planes;

        for (auto i = 0; i < plane_count; ++i)
        {
            planes.emplace_back(generate_checker_board_plane(width, height, rows, cols, color1[i], color2[i]));
        }

        return image<pixel_type_t>(width, height, width, std::move(planes));
    }
};

} // namespace sti
