#include "tests.hpp"

#include "../image.hpp"
#include "../histogram.hpp"

#include <string>

using namespace std::string_literals;

void test_histogram()
{
    auto path = "D:/dev/sti/data/DSC_7000.jpg"s;
    auto image = sti::read_image(path);
    auto histogram = sti::to_histogram(image);
    auto histogram_image = sti::to_image(histogram);

    sti::write_image(image, "D:/dev/sti/data/DSC_7000_gray.bmp");
    sti::write_image(histogram_image, "D:/dev/sti/data/histogram.bmp");
}
