#include <gtest/gtest.h>


#if 0

#include <core/image.hpp>
#include <core/histogram.hpp>
#include <string>

using namespace std::string_literals;

TEST(test_histogram, test_write_histogram_image)
{
    auto path = "D:/dev/sti/data/DSC_7000.jpg"s;
    auto image = sti::read_image(path);
    auto histogram = sti::to_histogram(image);
    auto histogram_image = sti::to_image(histogram);

    sti::write_image(image, "D:/dev/sti/data/DSC_7000_gray.bmp");
    sti::write_image(histogram_image, "D:/dev/sti/data/histogram.bmp");
}

#endif