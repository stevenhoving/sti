#include "load_file_fixture.h"
#include <sti/convert/convert_image.hpp>
#include <gtest/gtest.h>
#include <build_config.h>

TEST_F(load_file_fixture, decode_image)
{
    auto &info = loaded_image.info();

    EXPECT_EQ(256, info.width);
    EXPECT_EQ(171, info.height);
    EXPECT_EQ(sti::interleaved_image_format::RGBA, info.format);
    EXPECT_EQ(8, info.bits);
    EXPECT_EQ(info.width * (info.bits >> 3) * 4, info.stride);
}

TEST_F(load_file_fixture, decode_encode_image)
{
    write_result("DSC_7000_encoded.png", loaded_image);
}

TEST_F(load_file_fixture, decode_and_convert_to_image)
{
    auto result = sti::convert_image<std::uint8_t>::from_interleaved_image(loaded_image);
    EXPECT_EQ(256, result.width());
    EXPECT_EQ(171, result.height());
}

TEST_F(load_file_fixture, decode_and_convert_to_image_back_and_export)
{
    auto result = sti::convert_image<std::uint8_t>::from_interleaved_image(loaded_image);
    auto new_interleaved_image = sti::convert_image<std::uint8_t>::to_interleaved_image(result);

    write_result("DSC_7000_reencoded.png", new_interleaved_image);
}

TEST_F(load_file_fixture, decode_and_convert_to_image_back_and_export_remove_plane)
{
    auto result = sti::convert_image<std::uint8_t>::from_interleaved_image(loaded_image);

    result.remove_plane(3);

    auto new_interleaved_image = sti::convert_image<std::uint8_t>::to_interleaved_image(result);

    write_result("DSC_7000_reencoded_remove_plane_3.png", new_interleaved_image);
}

TEST_F(load_file_fixture, decode_and_convert_to_float_image_back_and_export)
{
    auto result = sti::convert_image<float>::from_interleaved_image(loaded_image);

    auto new_interleaved_image = sti::convert_image<float>::to_interleaved_image(result);

    write_result("DSC_7000_reencoded_domain.png", new_interleaved_image);
}
