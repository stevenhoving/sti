#include "tests.hpp"

#include "core/image.hpp"
#include "core/histogram.hpp"
#include "core/filter.hpp"
#include "core/filter/filter_mean_shift.hpp"
#include "core/filter/filter_threshold_adaptive_bradley.hpp"
#include "core/filter/filter_lowpass.hpp"
#include "core/filter/filter_range_compression_sse.hpp"

#include "stop_watch.h"

#include <cstdio>
#include <string>

using namespace std::string_literals;

void test_filter()
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

void test_mean_shift_filter()
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

void test_filter_mean_shift_filter2()
{
    auto path = "D:/dev/sti/data/DSC_7000.jpg"s;
    auto image = sti::read_image(path);

    auto filter = sti::core::mean_shift_filter<uint8_t>(4, 25.f);
    auto dst = sti::apply_filter(image, filter);

    sti::write_image(dst, "D:/dev/sti/data/mean-shift.bmp");
}

void test_threshold_adaptive_bradley()
{
    auto path = "D:/dev/sti/data/DSC_7000.jpg"s;
    auto image = sti::read_image(path);
    image = sti::core::filter_lowpass_copy(image);
    image = sti::core::filter_mean_shift_copy(image, 4, 25.f);
    image = sti::core::filter_threshold_adaptive_bradley_copy(image, 25, 0.15f);
    sti::write_image(image, "D:/dev/sti/data/threshold-adaptive-bradley.bmp");
}

void test_filter_range_compression_c()
{
    auto path = "D:/dev/sti/data/DSC_7000.jpg"s;
    auto image = sti::read_image(path);
    auto result = sti::image(image.width(), image.height(), image.stride());

    range_i420_c(image.data(), result.data(), image.stride(), image.height(), 100, 200);
    auto result_hist = sti::to_histogram(result);

    sti::write_image(result, "D:/dev/sti/data/range-compression-c.bmp");
    auto result_hist_img = sti::to_image(result_hist);
    sti::write_image(result_hist_img, "D:/dev/sti/data/range-compression-c-hist.bmp");
}

void test_filter_range_compression_sse()
{
    auto path = "D:/dev/sti/data/DSC_7000.jpg"s;
    auto image = sti::read_image(path);
    auto result_orig = sti::image(image.width(), image.height(), image.stride());
    auto result = sti::image(image.width(), image.height(), image.stride());
    for (int i = 0; i < 16; ++i)
        image.data()[i] = (i + 1) * 15;

    range_i420_c(image.data(), result_orig.data(), image.stride(), image.height(), 100, 200);

    sti::stop_watch stopwatch;
    stopwatch.time_start();
    range_i420_sse_c(image.data(), result.data(), image.stride(), image.height(), 100, 200);
    double dt = stopwatch.time_since();


    auto result_hist = sti::to_histogram(result);

    sti::write_image(result, "D:/dev/sti/data/range-compression-sse.bmp");
    //auto result_hist_img = sti::to_image(result_hist);
    //sti::write_image(result_hist_img, "D:/dev/sti/data/range-compression-sse-hist.bmp");
    printf("sse range took - %f ms\n", dt * 1000.0);

    auto pct = result_orig.equal_pct(result);

    if (result != result_orig)
        printf("sse and c version are not equal (%f%%)\n", pct);
}

