#include <gtest/gtest.h>

#include <core/image.hpp>
#include <core/histogram.hpp>
#include <core/filter.hpp>
#include <core/filter/filter_mean_shift.hpp>
#include <core/filter/filter_threshold_adaptive_bradley.hpp>
#include <core/filter/filter_lowpass.hpp>

#include <string>

using namespace std::string_literals;

TEST(test_filter, test_apply_filter)
{
    auto path = "D:/dev/sti/data/DSC_7000.jpg"s;
    auto image = sti::read_image(path);

    //auto kernel = sti::filter::kernel::lowpass::make_kernel();
    //auto kernel = sti::make_filter<sti::filter::kernel::lowpass<3, float>>();

    //auto result = sti::apply_filter(image, kernel);
    //auto filter = sti::make_filter(sti::make_kernel());

    //auto result = sti::apply_filter(image, sti::make_kernel());
    //sti::write_image(image, "D:/dev/sti/data/DSC_7000_gray.bmp");
}

TEST(test_filter, test_mean_shift_filter)
{
    auto path = "D:/dev/sti/data/DSC_7000.jpg"s;
    auto image = sti::read_image(path);

    auto dst = sti::image(image.width(), image.height());
    auto dst_orig = sti::image(image.width(), image.height());

    const int spatial_radius = 4;
    const float color_distance = 25.f;

    sti::core::filter_mean_shift(image, dst, spatial_radius, color_distance);
    sti::write_image(dst, "D:/dev/sti/data/mean-shift.bmp");
}

TEST(test_filter, test_threshold_adaptive_bradley)
{
    auto path = "D:/dev/sti/data/DSC_7000.jpg"s;
    auto image = sti::read_image(path);
    image = sti::core::filter_lowpass_copy(image);
    image = sti::core::filter_mean_shift_copy(image, 4, 25.f);
    image = sti::core::filter_threshold_adaptive_bradley_copy(image, 25, 0.15f);
    sti::write_image(image, "D:/dev/sti/data/threshold-adaptive-bradley.bmp");
}
