#include <gtest/gtest.h>

#include <sti/image.hpp>
#include <sti/filter/filter.hpp>
#include <sti/filter/filter_mean_shift.hpp>
#include <sti/color_image.hpp>
#include <sti/codecs/codec_png.h>
#include <sti/convert/convert_image.hpp>
#include <sti/kernel/kernel_lowpass.hpp>
#include <aeon/streams/file_stream.h>
#include <build_config.h>

TEST(test_filter, test_apply_lowpass_filter)
{
    auto stream = aeon::streams::file_stream(STI_TEST_DATA_PATH "/DSC_7000.png");
    auto image = sti::color_image();
    ASSERT_NO_THROW(image = sti::codecs::png::decode(stream));

    auto result = sti::convert_image<std::uint8_t, 4>::from_color_image(image);
    auto filtered_image = sti::image<std::uint8_t, 4>(result.width(), result.height(), result.stride());

    auto kernel = sti::kernel::lowpass::make_kernel<float, 3>();
    sti::filter::apply_kernel(result.get_slice(0), filtered_image.get_slice(0), kernel);
    sti::filter::apply_kernel(result.get_slice(1), filtered_image.get_slice(1), kernel);
    sti::filter::apply_kernel(result.get_slice(2), filtered_image.get_slice(2), kernel);
    sti::filter::apply_kernel(result.get_slice(3), filtered_image.get_slice(3), kernel);

    auto new_color_image = sti::convert_image<std::uint8_t, 4>::to_color_image(filtered_image);

    auto output_stream = aeon::streams::file_stream("DSC_7000_lowpass_filtered.png", aeon::streams::access_mode::write |
                                                                                 aeon::streams::access_mode::truncate);
    sti::codecs::png::encode(new_color_image, output_stream);
}

TEST(test_filter, test_apply_mean_shift_filter)
{
    auto stream = aeon::streams::file_stream(STI_TEST_DATA_PATH "/DSC_7000.png");
    auto image = sti::color_image();
    ASSERT_NO_THROW(image = sti::codecs::png::decode(stream));

    auto result = sti::convert_image<std::uint8_t, 4>::from_color_image(image);
    auto filtered_image = sti::image<std::uint8_t, 4>(result.width(), result.height(), result.stride());

    auto kernel = sti::kernel::lowpass::make_kernel<float, 3>();
    sti::filter::filter_mean_shift(result.get_slice(0), filtered_image.get_slice(0), 8, 60.0f);
    sti::filter::filter_mean_shift(result.get_slice(1), filtered_image.get_slice(1), 8, 60.0f);
    sti::filter::filter_mean_shift(result.get_slice(2), filtered_image.get_slice(2), 8, 60.0f);
    sti::filter::filter_mean_shift(result.get_slice(3), filtered_image.get_slice(3), 8, 60.0f);

    auto new_color_image = sti::convert_image<std::uint8_t, 4>::to_color_image(filtered_image);

    auto output_stream = aeon::streams::file_stream("DSC_7000_mean_shift_filtered.png", aeon::streams::access_mode::write |
                                                                                 aeon::streams::access_mode::truncate);
    sti::codecs::png::encode(new_color_image, output_stream);
}

#if 0
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

#endif