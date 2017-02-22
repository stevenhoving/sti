/* sti simple templated image lib (the l is silent) */

#include <stdio.h>

#include "sti.h"

#include "image.h"
#include "image_filter.h"
#include "image_codec_bmp.h"
#include "image_codec_jpeg.h"
#include "histogram.h"
#include "generator.h"
#include "adaptive.h"
#include "kernel.h"
#include "gaussian.h"
#include "sharpen.h"
#include "low_pass.h"
#include "edge.h"
#include "invert.h"
#include "emboss.h"
#include "sobel.h"
#include "selective_gaussian_blur_ex.h"
#include "canny.h"
#include "stop_watch.h"
#include "denoise_vague_denoiser.h"
#include "noise.h"

#if 0
void test_brightness()
{
	const int width = 1280;
	const int height = 960;

	uint8_t * test1 = (uint8_t *)malloc(width * height);
	uint8_t * test2 = (uint8_t *)malloc(width * height);
	memset(test1, 0, width * height);
	memset(test2, 0, width * height);

	for (int i = 0; i < width * height; ++i)
	{
		test1[i] = (255.f / float(width * height)) * i;
		i++;
	}

	for (int i = 0; i < width * height; ++i)
	{
		test2[i] = (255.f / float(width * height)) * i;
		i++;
	}

	//bbyuv::SaveGrayBitmap("test_bright_orig.bmp", width, height, test1);

	double dt;
	sti::stop_watch stopwatch;

	stopwatch.time_start();
	sti::BrightnessI420(test1, width, height, 100);
	dt = stopwatch.time_since();

	sti::SaveGrayBitmap("test_bright_native.bmp", width, height, test1);
	printf("native: %f\n", dt);

	stopwatch.time_start();
	sti::BrightnessI420SSE(test2, width, height, 100);
	dt = stopwatch.time_since();

	sti::SaveGrayBitmap("test_bright_sse.bmp", width, height, test2);
	printf("SSE:    %f\n", dt);

	if (memcmp(test1, test2, width * height) != 0)
		printf("brightness test failed\n\n");
	else
		printf("brightness test success\n\n");
}

void test_range()
{
	const int width = 1280;
	const int height = 960;

	uint8_t * test1 = (uint8_t *)malloc(width * height);
	uint8_t * test2 = (uint8_t *)malloc(width * height);
	memset(test1, 0, width * height);
	memset(test2, 0, width * height);

	for (int i = 0; i < width * height; ++i)
	{
		test1[i] = (255.f / float(width * height)) * float(i);
		i++;
	}

	for (int i = 0; i < width * height; ++i)
	{
		test2[i] = (255.f / float(width * height)) * float(i);
		i++;
	}

	double dt;
	sti::stop_watch stopwatch;

	stopwatch.time_start();
	sti::RangeI420float(test1, width, height, 50, 200);
	dt = stopwatch.time_since();

	sti::SaveGrayBitmap("test_range_native.bmp", width, height, test1);
	printf("native: %f\n", dt);

	stopwatch.time_start();
	sti::RangeI420LOOKUP(test2, width, height, 50, 200);
	dt = stopwatch.time_since();

	sti::SaveGrayBitmap("test_range_lookup.bmp", width, height, test2);
	
	printf("lookup: %f\n", dt);

	if (memcmp(test1, test2, width * height) != 0)
		printf("range test failed\n\n");
	else
		printf("range test success\n\n");

}

void test_blur()
{
	const int width = 1280;
	const int height = 960;

	uint8_t * test0 = (uint8_t *)malloc(width * height);
	uint8_t * test1 = (uint8_t *)malloc(width * height);
	uint8_t * test2 = (uint8_t *)malloc(width * height);
	memset(test0, 0, width * height);
	memset(test1, 0, width * height);
	memset(test2, 0, width * height);

	for (int i = 0; i < width * height; ++i)
	{
		test1[i] = (255.f / float(width * height)) * i;
		i++;
	}

	
	for (int i = 0; i < width * height; ++i)
	{
		test2[i] = (255.f / float(width * height)) * i;
		i++;
	}

	double dt;
	sti::stop_watch stopwatch;

	sti::LowPassI420(test0, width, height);
	sti::LowPassI420SSE(test0, width, height);

	stopwatch.time_start();
	sti::LowPassI420(test1, width, height);
	dt = stopwatch.time_since();

	sti::SaveGrayBitmap("test_blur_native.bmp", width, height, test1);
	printf("native: %f\n", dt);

	stopwatch.time_start();
	sti::LowPassI420SSE(test2, width, height);
	dt = stopwatch.time_since();

	sti::SaveGrayBitmap("test_blur_int.bmp", width, height, test2);
	printf("float:  %f\n", dt);

	if (memcmp(test1, test2, width * height) != 0)
		printf("blur test failed\n\n");
	else
		printf("blur test success\n\n");
}

void test_contrast()
{
	const int contrast_adj = 10;

	const int width = 1280;
	const int height = 960;

	uint8_t * test1 = (uint8_t *)malloc(width * height);
	uint8_t * test2 = (uint8_t *)malloc(width * height);
	uint8_t * test3 = (uint8_t *)malloc(width * height);
	uint8_t * test4 = (uint8_t *)malloc(width * height);
	memset(test1, 0, width * height);
	memset(test2, 0, width * height);
	memset(test3, 0, width * height);
	memset(test4, 0, width * height);

	for (int i = 0; i < width * height; ++i)
	{
		test1[i] = uint8_t((255.f / float(width * height)) * float(i));
		test2[i] = uint8_t((255.f / float(width * height)) * float(i));
		test3[i] = uint8_t((255.f / float(width * height)) * float(i));
		test4[i] = uint8_t((255.f / float(width * height)) * float(i));
		i++;
	}

	double dt;
	sti::stop_watch stopwatch;

	stopwatch.time_start();
	sti::ContrastI420(test1, width, height, contrast_adj);
	dt = stopwatch.time_since();

	sti::SaveGrayBitmap("test_contrast_native.bmp", width, height, test1);
	printf("native: %f\n", dt);

	stopwatch.time_start();
	sti::ContrastI420LOOKUP(test2, width, height, contrast_adj);
	dt = stopwatch.time_since();

	sti::SaveGrayBitmap("test_contrast_lookup.bmp", width, height, test2);
	printf("lookup: %f\n", dt);

	stopwatch.time_start();
	sti::ContrastI420SSE(test3, width, height, contrast_adj);
	dt = stopwatch.time_since();

	sti::SaveGrayBitmap("test_contrast_sse.bmp", width, height, test3);
	printf("sse:    %f\n", dt);

	stopwatch.time_start();
	sti::ContrastI420INT(test4, width, height, contrast_adj);
	dt = stopwatch.time_since();

	sti::SaveGrayBitmap("test_contrast_int.bmp", width, height, test4);
	printf("int:    %f\n", dt);

	for (int i = 0; i < width * height; ++i)
	{
		if (test1[i] != test3[i])
		{
			printf("hmms [%u] %u | %u\n", i, test1[i], test3[i]);
		}
	}

	if (memcmp(test1, test2, width * height) != 0)
		printf("contrast lookup test failed\n\n");

	if (memcmp(test1, test3, width * height) != 0)
		printf("contrast SSE test failed\n\n");

	if (memcmp(test1, test4, width * height) != 0)
		printf("contrast INT test failed\n\n");

	printf("contrast test finished\n\n");
}

void test_sharping()
{
	const int width = 1280;
	const int height = 960;

	uint8_t * test1 = (uint8_t *)malloc(width * height);
	uint8_t * test2 = (uint8_t *)malloc(width * height);
	
	memset(test1, 0, width * height);
	memset(test2, 0, width * height);
	
	for (int i = 0; i < width * height; ++i)
	{
		test1[i] = uint8_t((255.f / float(width * height)) * float(i));
		test2[i] = uint8_t((255.f / float(width * height)) * float(i));
		i++;
	}

	double dt;
	sti::stop_watch stopwatch;

	stopwatch.time_start();
	sti::SharpenI420(test2, width, height);
	dt = stopwatch.time_since();

	sti::SaveGrayBitmap("test_sharpen_original.bmp", width, height, test1);
	sti::SaveGrayBitmap("test_sharpen_native.bmp", width, height, test2);
	printf("native: %f\n", dt);

	printf("sharpen test finished\n\n");
}

#endif

template <typename T>
void test_adaptive_threshold_mean(sti::image<T> &src)
{
    auto img = sti::image<T>(src.width(), src.height());
    sti::image_filter::adaptive_threshold_mean(src, img, 32, 2);
    sti::image_codec::bmp::save(img, "adaptive_threshold_mean.bmp");
}

template <typename T>
void test_adaptive_threshold_bradley(sti::image<T> &src)
{
    auto img = sti::image<T>(src.width(), src.height());
    sti::image_filter::adaptive_threshold_bradley(src, img, 32, 0.15);
    sti::image_codec::bmp::save(img, "adaptive_threshold_bradley.bmp");
}

template <typename T>
void test_adaptive_threshold_otsu_segmented(sti::image<T> &src)
{
    auto img = sti::image<T>(src.width(), src.height());
    sti::image_filter::adaptive_threshold_otsu_segmented(src, img);
    sti::image_codec::bmp::save(img, "adaptive_threshold_otsu_segmented.bmp");
}

template <typename T>
void test_adaptive_threshold_shanbhag(sti::image<T> &src)
{
    auto img = sti::image(src.width(), src.height());
    sti::image_filter::adaptive_threshold_shanbhag(src, img);
    
    sti::image_codec::bmp::save(img, "adaptive_threshold_shanbhag.bmp");
}

template <typename T>
void test_adaptive_threshold_triangle(sti::image<T> &src)
{
    auto img = sti::image(src.width(), src.height());
    sti::image_filter::adaptive_threshold_triangle(src, img);
    sti::image_codec::bmp::save(img, "adaptive_threshold_triangle.bmp");
}

template <typename T>
void test_adaptive_threshold_yen(sti::image<T> &src)
{
    auto img = sti::image(src.width(), src.height());
    sti::image_filter::adaptive_threshold_yen(src, img);
    sti::image_codec::bmp::save(img, "adaptive_threshold_yen.bmp");
}

void test_histogram()
{
    auto img_src = sti::image_codec::jpeg::load<float>("data/DSC_7000.jpg");
    //test_adaptive_threshold_mean(img_src);
    //test_adaptive_threshold_bradley(img_src);
    test_adaptive_threshold_otsu_segmented(img_src);
    //test_adaptive_threshold_shanbhag(img_src);
    //test_adaptive_threshold_triangle(img_src);
    //test_adaptive_threshold_yen(img_src);
}

void test_edge_detection()
{
    sti::stop_watch timer;
    timer.time_start();

    auto src = sti::image_codec::jpeg::load<uint8_t>("data/DSC_7000.jpg");
    auto blur = sti::image<uint8_t>(src.width(), src.height());

    //sti::image_filter::apply_kernel(src, blur, sti::gaussian::make_kernel<double, 5>(0.5));
    sti::image_filter::apply_kernel(src, blur, sti::gaussian::make_kernel<double, 5>(2.0));

    auto img = sti::image<uint8_t>(src.width(), src.height());
    //sti::image_filter::adaptive_threshold_mean(blur, img, 32, 2);
    sti::image_filter::adaptive_threshold_mean(blur, img, 16, -4);

    auto dst = sti::image<uint8_t>(src.width(), src.height());
    sti::image_filter::apply_kernel(img, dst, sti::edge::make_kernel<double, 5>());
    sti::image_filter::apply_kernel(dst, img, sti::gaussian::make_kernel<double, 5>(2.0));

    sti::invert::invert_image(img, dst);

    sti::image_codec::bmp::save(dst, "edge_detection.bmp");

    printf("test_edge_detection - took: %f\n", timer.time_since());
}

void test_gaussian()
{
    auto kern5x5 = sti::gaussian::make_kernel<float, 5>(5.0);
    //auto src = sti::generator::create_checker_board(512, 512, 8, 8);
    auto src = sti::image_codec::jpeg::load<uint8_t>("data/DSC_7000.jpg");

    auto dst = sti::image<uint8_t>(src.width(), src.height());
    sti::image_filter::apply_kernel(src, dst, kern5x5);
    sti::image_codec::bmp::save(dst, "gaussian_float.bmp");
}

void test_sharpen()
{
    auto kern5x5 = sti::sharpen::make_kernel<float, 5>();
    auto src = sti::image_codec::jpeg::load<uint8_t>("data/DSC_7000.jpg");
    auto dst = sti::image<uint8_t>(src.width(), src.height());
    sti::image_filter::apply_kernel(src, dst, kern5x5);
    sti::image_codec::bmp::save(dst, "sharpen.bmp");
}

void test_low_pass()
{
    auto kern5x5 = sti::low_pass::make_kernel<float, 3>();
    auto src = sti::image_codec::jpeg::load<uint8_t>("data/DSC_7000.jpg");
    auto dst = sti::image<uint8_t>(src.width(), src.height());
    sti::image_filter::apply_kernel(src, dst, kern5x5);
    sti::image_codec::bmp::save(dst, "low_pass.bmp");
}

void test_emboss()
{
    auto kern5x5 = sti::emboss::make_kernel<float, 5>();
    //auto kern5x5 = sti::emboss::make_kernel<double, 3>();
    auto src = sti::image_codec::jpeg::load<uint8_t>("data/DSC_7000.jpg");
    
    auto dst = sti::image<uint8_t>(src.width(), src.height());
    sti::image_filter::apply_kernel(src, dst, kern5x5);
    sti::image_codec::bmp::save(dst, "emboss.bmp");
}

void test_selective_gaussian_blur()
{
    auto src = sti::image_codec::jpeg::load<uint8_t>("data/DSC_7000.jpg");
    auto dst = sti::image<uint8_t>(src.width(), src.height());
    sti::experimental::selective_gaussian_blur(src, dst, 5, 50);
    sti::image_codec::bmp::save(dst, "selective_gaussian_blur.bmp");
}

void test_sobel()
{
    auto kern5x5 = sti::sobel::edge::horizontal::make_kernel<float, 3>();

    auto src = sti::image_codec::jpeg::load<uint8_t>("data/DSC_7000.jpg");
    auto dst = sti::image<uint8_t>(src.width(), src.height());
    sti::image_filter::apply_kernel(src, dst, kern5x5);
    sti::image_codec::bmp::save(dst, "sobel.bmp");
}

void test_canny()
{
    //auto src = sti::image_codec::jpeg::load<uint8_t>("data/DSC_7000.jpg");
    //auto dst = sti::image<uint8_t>(src.width(), src.height());
    //sti::canny::canny_edge_detection(src, dst, 100, 30, 1.0);
    //sti::image_codec::bmp::save(dst, "canny.bmp");
}

#if 0
void test_vague_denoiser()
{
    auto src = sti::noise_generator::generate(512, 512);
    auto dst = sti::image(src.width(), src.height());
    //sti::image_filter::apply_kernel(src, dst, sti::low_pass::make_kernel<float, 5>());
    //sti::image_filter::apply_kernel(dst, src, sti::low_pass::make_kernel<float, 5>());

    sti::image_codec::bmp::save(src, "vague_denoiser_orig.bmp");

    //sti::vague_denoiser::vague_denoiser(src, dst, 0.8, 1, 6, 85);
    //sti::vague_denoiser::vague_denoiser(src, dst, 0.8, 2, 6, 85);
    //sti::vague_denoiser::vague_denoiser(src, dst, 2.0, 2, 6, 85);
    //sti::vague_denoiser::vague_denoiser(src, dst, 2.0, 1, 6, 50);

    int henkhenk = 1;
    int count = 0;
    while(henkhenk < src.width())
    {
        henkhenk = henkhenk << 1;
        count++;
    }

    //sti::vague_denoiser::vague_denoiser(src, dst, 2.0, 0, 6, 100);
    sti::vague_denoiser::vague_denoiser(src, dst, 0.01, 0, count-2, 100);
    sti::image_codec::bmp::save(dst, "vague_denoiser.bmp");
}

#endif

int main( int argc, char* argv[] )
{
    //test_brightness();
    //test_range();
    //test_blur();
    //test_contrast();
    //test_sharping();
    test_histogram();
    //test_edge_detection();
    //test_gaussian();
    //test_sharpen();
    //test_low_pass();
    //test_emboss();
    //test_selective_gaussian_blur();
    //test_sobel();
    //test_canny();
    //test_vague_denoiser();

    system("pause");
    return 0;
}