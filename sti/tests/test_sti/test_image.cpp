#include "load_file_fixture.h"
#include <sti/image_detail.hpp>
#include <sti/convert/convert_image.hpp>
#include <sti/image.hpp>
#include <aeon/streams/file_stream.h>
#include <build_config.h>
#include <gtest/gtest.h>

TEST(test_image, default_stride_value)
{
    auto image = sti::image<float>(10, 10, 3);
    EXPECT_EQ(10, image.width());
    EXPECT_EQ(10, image.width());
    EXPECT_EQ(image.width(), image.stride());
    EXPECT_EQ(3, image.plane_count());
}

TEST_F(load_file_fixture, decode_and_convert_to_image_back_and_export_remove_plane_swap)
{
    auto result = sti::convert_image<std::uint8_t>::from_color_image(loaded_image);

    result.swap_planes<0, 1, 2>();

    auto new_color_image = sti::convert_image<std::uint8_t>::to_color_image(result);

    write_result("DSC_7000_reencoded_remove_plane_3_swap.png", new_color_image);
}

TEST_F(load_file_fixture, convert_to_grey_rearrange)
{
    auto result = sti::convert_image<std::uint8_t>::from_color_image(loaded_image);

    result.rearrange_planes<0, 0, 0>();

    auto new_color_image = sti::convert_image<std::uint8_t>::to_color_image(result);

    write_result("DSC_7000_rearrange.png", new_color_image);
}

TEST_F(load_file_fixture, split_rgba)
{
    auto result = sti::convert_image<std::uint8_t>::from_color_image(loaded_image);

    auto red_image = sti::image<std::uint8_t>(result.width(), result.height(), result.stride(), {result.get_plane(0)});
    auto green_image =
        sti::image<std::uint8_t>(result.width(), result.height(), result.stride(), {result.get_plane(1)});
    auto blue_image = sti::image<std::uint8_t>(result.width(), result.height(), result.stride(), {result.get_plane(2)});

    red_image.rearrange_planes<0, 0, 0>();
    green_image.rearrange_planes<0, 0, 0>();
    blue_image.rearrange_planes<0, 0, 0>();

    write_result("DSC_7000_red.png", sti::convert_image<std::uint8_t>::to_color_image(red_image));
    write_result("DSC_7000_green.png", sti::convert_image<std::uint8_t>::to_color_image(green_image));
    write_result("DSC_7000_blue.png", sti::convert_image<std::uint8_t>::to_color_image(blue_image));
}
