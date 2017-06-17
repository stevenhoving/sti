#include <sti/codecs/codec_png.h>
#include <sti/color_image.hpp>
#include <sti/convert/convert_image.hpp>
#include <aeon/streams/file_stream.h>

#include <gtest/gtest.h>
#include <build_config.h>

TEST(test_png_codec, decode_image)
{
    auto stream = aeon::streams::file_stream(STI_TEST_DATA_PATH "/DSC_7000.png");
    auto image = sti::color_image();
    ASSERT_NO_THROW(image = sti::codecs::png::decode(stream));

    auto &info = image.info();

    EXPECT_EQ(256, info.width);
    EXPECT_EQ(171, info.height);
    EXPECT_EQ(sti::color_image_format::RGBA, info.format);
    EXPECT_EQ(8, info.bits);
    EXPECT_EQ(info.width * (info.bits >> 3) * 4, info.stride);
}

TEST(test_png_codec, decode_encode_image)
{
    auto stream = aeon::streams::file_stream(STI_TEST_DATA_PATH "/DSC_7000.png");
    auto image = sti::color_image();
    ASSERT_NO_THROW(image = sti::codecs::png::decode(stream));

    auto output_stream = aeon::streams::file_stream("DSC_7000_encoded.png", aeon::streams::access_mode::write |
                                                                                aeon::streams::access_mode::truncate);
    sti::codecs::png::encode(image, output_stream);
}

TEST(test_png_codec, decode_and_convert_to_image)
{
    auto stream = aeon::streams::file_stream(STI_TEST_DATA_PATH "/DSC_7000.png");
    auto image = sti::color_image();
    ASSERT_NO_THROW(image = sti::codecs::png::decode(stream));

    auto result = sti::convert_image<std::uint8_t, 4>::from_color_image(image);
    EXPECT_EQ(256, result.width());
    EXPECT_EQ(171, result.height());
}
