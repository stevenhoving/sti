#include "tests.hpp"

#include "../image.hpp"

#include <string>
#include <cassert>

#define test_assert(xs)

using namespace std::string_literals;

void test_image_read_image()
{
    auto path = "D:/dev/sti/data/DSC_7000.jpg"s;
    auto image = sti::read_image(path);
}

void test_image_default_color_float()
{
    auto image = sti::core::image<float>(10, 10);
    test_assert(image[0][0] == 1.f);
}

void test_image_default_color_double()
{
    auto image = sti::core::image<double>(10, 10);
    test_assert(image[0][0] == 1.0);
}

void test_image_default_color_uint8()
{
    auto image = sti::core::image<uint8_t>(10, 10);
    test_assert(image[0][0] == 0xFF);
}

void test_image_default_color_uint16()
{
    auto image = sti::core::image<uint16_t>(10, 10);
    test_assert(image[0][0] == 0xFFFF);
}

void test_image_default_color_uint32()
{
    auto image = sti::core::image<uint32_t>(10, 10);
    test_assert(image[0][0] == 0xFFFFFFFF);
}

void test_image()
{
    test_image_read_image();
    test_image_default_color_float();
    test_image_default_color_double();
    test_image_default_color_uint8();
    test_image_default_color_uint16();
    test_image_default_color_uint32();
}