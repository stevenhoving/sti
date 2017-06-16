#include <gtest/gtest.h>

#include <sti/image.hpp>
#include <sti/filter/filter.hpp>
#include <sti/filter/filter_mean_shift.hpp>
#include <sti/filter/filter_threshold_adaptive_bradley.hpp>
#include <sti/color_image.hpp>
#include <sti/codecs/codec_png.h>
#include <sti/convert/convert_image.hpp>
#include <sti/kernel/kernel_lowpass.hpp>
#include <sti/kernel/kernel_gaussian.hpp>
#include <sti/kernel/kernel_edge.hpp>
#include <sti/kernel/kernel_emboss.hpp>
#include <sti/kernel/kernel_sharpen.hpp>
#include <sti/kernel/kernel_sobel.hpp>
#include <aeon/streams/file_stream.h>
#include <build_config.h>
#include <string>

namespace filter_test
{

template <typename pixel_type_t>
sti::image<pixel_type_t> load_png(const char *path)
{
    std::string file_path = STI_TEST_DATA_PATH;
    file_path += "/";
    file_path += path;

    auto stream = aeon::streams::file_stream(file_path);
    auto image = sti::codecs::png::decode(stream);

    return sti::convert_image<pixel_type_t>::from_color_image(image);
}

template <typename pixel_type_t>
void save_png(const sti::image<pixel_type_t> &image, const char *path)
{
    auto new_color_image = sti::convert_image<pixel_type_t>::to_color_image(image);

    auto output_stream =
        aeon::streams::file_stream(path, aeon::streams::access_mode::write | aeon::streams::access_mode::truncate);
    sti::codecs::png::encode(new_color_image, output_stream);
}

template <typename pixel_type_t, typename K>
auto apply_kernel(const sti::image<pixel_type_t> &image, const K &kernel) -> sti::image<pixel_type_t>
{
    auto filtered_image = sti::image<pixel_type_t>(image.width(), image.height(), image.stride(), image.plane_count());
    for (int i = 0; i < image.plane_count(); ++i)
        sti::filter::apply_kernel(image.get_plane(i), filtered_image.get_plane(i), kernel);

    return filtered_image;
}
} // filter_test

TEST(test_filter, test_apply_lowpass_filter)
{
    auto image = filter_test::load_png<std::uint8_t>("DSC_7000.png");
    auto kernel = sti::kernel::lowpass::make_kernel<float, 3>();
    auto filtered_image = filter_test::apply_kernel(image, kernel);
    filter_test::save_png<std::uint8_t>(filtered_image, "DSC_7000_lowpass_filtered.png");
}

TEST(test_filter, test_apply_gaussian_filter)
{
    auto image = filter_test::load_png<std::uint8_t>("DSC_7000.png");
    auto kernel = sti::kernel::gaussian::make_kernel<float, 3>();
    auto filtered_image = filter_test::apply_kernel(image, kernel);
    filter_test::save_png<std::uint8_t>(filtered_image, "DSC_7000_gaussian_filtered.png");
}

TEST(test_filter, test_apply_edge_filter)
{
    auto image = filter_test::load_png<std::uint8_t>("DSC_7000.png");
    auto kernel = sti::kernel::edge::make_kernel<float, 3>();
    auto filtered_image = filter_test::apply_kernel(image, kernel);
    filter_test::save_png<std::uint8_t>(filtered_image, "DSC_7000_edge_filtered.png");
}

TEST(test_filter, test_apply_emboss_filter)
{
    auto image = filter_test::load_png<std::uint8_t>("DSC_7000.png");
    auto kernel = sti::kernel::emboss::make_kernel<float, 3>();
    auto filtered_image = filter_test::apply_kernel(image, kernel);
    filter_test::save_png<std::uint8_t>(filtered_image, "DSC_7000_emboss_filtered.png");
}

TEST(test_filter, test_apply_sharpen_filter)
{
    auto image = filter_test::load_png<std::uint8_t>("DSC_7000.png");
    auto kernel = sti::kernel::sharpen::make_kernel<float, 3>();
    auto filtered_image = filter_test::apply_kernel(image, kernel);
    filter_test::save_png<std::uint8_t>(filtered_image, "DSC_7000_sharpen_filtered.png");
}

TEST(test_filter, test_apply_sobel_filter)
{
    auto image = filter_test::load_png<std::uint8_t>("DSC_7000.png");
    auto kernel = sti::kernel::sobel_horizontal::make_kernel<float, 3>();
    auto filtered_image = filter_test::apply_kernel(image, kernel);
    filter_test::save_png<std::uint8_t>(filtered_image, "DSC_7000_sobel_horizontal_filtered.png");
}

TEST(test_filter, test_apply_mean_shift_filter)
{
    auto image = filter_test::load_png<std::uint8_t>("DSC_7000.png");
    auto filtered_image = sti::image<std::uint8_t>(image.width(), image.height(), image.stride(), image.plane_count());

    sti::filter::filter_mean_shift(image.get_plane(0), filtered_image.get_plane(0), 8, 60.0f);
    sti::filter::filter_mean_shift(image.get_plane(1), filtered_image.get_plane(1), 8, 60.0f);
    sti::filter::filter_mean_shift(image.get_plane(2), filtered_image.get_plane(2), 8, 60.0f);
    sti::filter::filter_mean_shift(image.get_plane(3), filtered_image.get_plane(3), 8, 60.0f);
    filter_test::save_png<std::uint8_t>(filtered_image, "DSC_7000_mean_shift_filtered.png");
}

TEST(test_filter, test_apply_threshold_adaptive_bradley)
{
    auto image = filter_test::load_png<std::uint8_t>("DSC_7000.png");
    auto filtered_image = sti::image<std::uint8_t>(image.width(), image.height(), image.stride(), image.plane_count());

    sti::filter::filter_threshold_adaptive_bradley(image.get_plane(0), filtered_image.get_plane(0), 4, 0.1f);
    sti::filter::filter_threshold_adaptive_bradley(image.get_plane(1), filtered_image.get_plane(1), 4, 0.1f);
    sti::filter::filter_threshold_adaptive_bradley(image.get_plane(2), filtered_image.get_plane(2), 4, 0.1f);
    sti::filter::filter_threshold_adaptive_bradley(image.get_plane(3), filtered_image.get_plane(3), 4, 0.1f);
    filter_test::save_png<std::uint8_t>(filtered_image, "DSC_7000_adaptive_bradley_filtered.png");
}
