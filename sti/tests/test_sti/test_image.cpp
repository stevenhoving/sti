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
    auto result = sti::convert_image<std::uint8_t>::from_interleaved_image(loaded_image);

    result.swap_planes<0, 1, 2>();

    auto new_interleaved_image = sti::convert_image<std::uint8_t>::to_interleaved_image(result);

    write_result("DSC_7000_reencoded_remove_plane_3_swap.png", new_interleaved_image);
}

TEST_F(load_file_fixture, convert_to_grey_rearrange)
{
    auto result = sti::convert_image<std::uint8_t>::from_interleaved_image(loaded_image);

    result.rearrange_planes<0, 0, 0>();

    auto new_interleaved_image = sti::convert_image<std::uint8_t>::to_interleaved_image(result);

    write_result("DSC_7000_rearrange.png", new_interleaved_image);
}

TEST_F(load_file_fixture, split_rgba)
{
    auto result = sti::convert_image<std::uint8_t>::from_interleaved_image(loaded_image);

    auto red_image = sti::image<std::uint8_t>(result.width(), result.height(), result.stride(), {result.get_plane(0)});
    auto green_image =
        sti::image<std::uint8_t>(result.width(), result.height(), result.stride(), {result.get_plane(1)});
    auto blue_image = sti::image<std::uint8_t>(result.width(), result.height(), result.stride(), {result.get_plane(2)});

    red_image.rearrange_planes<0, 0, 0>();
    green_image.rearrange_planes<0, 0, 0>();
    blue_image.rearrange_planes<0, 0, 0>();

    write_result("DSC_7000_red.png", sti::convert_image<std::uint8_t>::to_interleaved_image(red_image));
    write_result("DSC_7000_green.png", sti::convert_image<std::uint8_t>::to_interleaved_image(green_image));
    write_result("DSC_7000_blue.png", sti::convert_image<std::uint8_t>::to_interleaved_image(blue_image));
}

TEST_F(load_file_fixture, image_clone)
{
    auto result = sti::convert_image<std::uint8_t>::from_interleaved_image(loaded_image);
    auto new_image = result.clone();
    auto new_interleaved_image = sti::convert_image<std::uint8_t>::to_interleaved_image(new_image);
    write_result("DSC_7000_clone.png", new_interleaved_image);
}

TEST_F(load_file_fixture, blit_on_cloned_image)
{
    auto result = sti::convert_image<std::uint8_t>::from_interleaved_image(loaded_image);
    auto new_image = result.clone();

    sti::detail<std::uint8_t>::blit(new_image, 30, 30, 50, 50, result, 100, 90);

    auto new_interleaved_image = sti::convert_image<std::uint8_t>::to_interleaved_image(new_image);

    write_result("DSC_7000_blit_on_cloned.png", new_interleaved_image);
}

TEST_F(load_file_fixture, crop_image)
{
    auto result = sti::convert_image<std::uint8_t>::from_interleaved_image(loaded_image);

    auto new_image = sti::detail<std::uint8_t>::crop(result, 100, 90, 50, 50);

    auto new_interleaved_image = sti::convert_image<std::uint8_t>::to_interleaved_image(new_image);

    write_result("DSC_7000_crop_100_90_50_50.png", new_interleaved_image);
}

TEST_F(load_file_fixture, blit_from_cropped_image)
{
    auto result = sti::convert_image<std::uint8_t>::from_interleaved_image(loaded_image);
    auto cropped_image = sti::detail<std::uint8_t>::crop(result, 100, 90, 50, 50);

    sti::detail<std::uint8_t>::blit(result, 30, 30, cropped_image);

    auto new_interleaved_image = sti::convert_image<std::uint8_t>::to_interleaved_image(result);

    write_result("DSC_7000_blit_from_cropped.png", new_interleaved_image);
}
