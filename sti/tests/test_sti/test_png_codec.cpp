#include <sti/codecs/codec_png.h>
#include <sti/color_image.hpp>
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
    EXPECT_EQ(4, info.bytes_per_pixel);
    EXPECT_EQ(info.width, info.stride);
}
