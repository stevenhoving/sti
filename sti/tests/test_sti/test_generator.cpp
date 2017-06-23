#include <sti/generator.hpp>
#include <sti/convert/convert_image.hpp>
#include "write_file_fixture.h"
#include <aeon/utility/container.h>
#include <cstdint>

static constexpr auto white = aeon::utility::container::make_array2(255.0f, 255.0f, 255.0f);
static constexpr auto black = aeon::utility::container::make_array2(0.0f, 0.0f, 0.0f);
static constexpr auto red = aeon::utility::container::make_array2(255.0f, 0.0f, 0.0f);
static constexpr auto green = aeon::utility::container::make_array2(0.0f, 255.0f, 0.0f);
static constexpr auto blue = aeon::utility::container::make_array2(0.0f, 0.0f, 255.0f);
static constexpr auto purple = aeon::utility::container::make_array2(255.0f, 0.0f, 255.0f);

TEST_F(write_file_fixture, generate_black_white_color_checker_image_3_planes)
{
    auto result = sti::generator<float>::generate_checker_board_image<3>(128, 128, 16, 16, white, black);
    auto new_interleaved_image = sti::convert_image<float>::to_interleaved_image(result);
    write_result("checker_black_white_3_planes.png", new_interleaved_image);
}

TEST_F(write_file_fixture, generate_red_green_color_checker_image_3_planes)
{
    auto result = sti::generator<float>::generate_checker_board_image<3>(128, 128, 16, 16, red, green);
    auto new_interleaved_image = sti::convert_image<float>::to_interleaved_image(result);
    write_result("checker_red_green_3_planes.png", new_interleaved_image);
}

TEST_F(write_file_fixture, generate_green_blue_color_checker_image_3_planes)
{
    auto result = sti::generator<float>::generate_checker_board_image<3>(128, 128, 16, 16, green, blue);
    auto new_interleaved_image = sti::convert_image<float>::to_interleaved_image(result);
    write_result("checker_green_blue_3_planes.png", new_interleaved_image);
}

TEST_F(write_file_fixture, generate_purple_black_color_checker_image_3_planes)
{
    auto result = sti::generator<float>::generate_checker_board_image<3>(512, 512, 64, 64, black, purple);
    auto new_interleaved_image = sti::convert_image<float>::to_interleaved_image(result);
    write_result("checker_purple_black_3_planes.png", new_interleaved_image);
}
