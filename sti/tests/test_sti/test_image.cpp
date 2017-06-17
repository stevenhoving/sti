#include <gtest/gtest.h>

#include <core/image.hpp>

#include <string>
#include <cassert>

using namespace std::string_literals;

TEST(test_image, read_image)
{
    auto path = "D:/dev/sti/data/DSC_7000.jpg"s;
    auto image = sti::read_image(path);
}

TEST(test_image, default_color_float)
{
    auto image = sti::core::image<float>(10, 10);
    //ASSERT_FLOAT_EQ(1.0f, image[0][0]);
}

TEST(test_image, default_color_double)
{
    auto image = sti::core::image<double>(10, 10);
    //test_assert(image[0][0] == 1.0);
}

TEST(test_image, default_color_uint8)
{
    auto image = sti::core::image<uint8_t>(10, 10);
    //test_assert(image[0][0] == 0xFF);
}

TEST(test_image, default_color_uint16)
{
    auto image = sti::core::image<uint16_t>(10, 10);
    //test_assert(image[0][0] == 0xFFFF);
}

TEST(test_image, default_color_uint32)
{
    auto image = sti::core::image<uint32_t>(10, 10);
    //test_assert(image[0][0] == 0xFFFFFFFF);
}
