#pragma once

#if 0

#include <stdio.h>
#include <stdint.h>
#include <string>

#include <stdio.h>

#include <windows.h > // for bmp shizzle

#include <xmmintrin.h>
#include <emmintrin.h>
#include <ammintrin.h>
#include <immintrin.h>
#include <algorithm>


namespace sti
{
	/* libyuv does store his stuff always bgra */
static void BGRAToRGB24Row_C(const uint8_t* src_argb, uint8_t* dst_rgb, int width) {
	int x;
	for (x = 0; x < width; ++x) {
		uint8_t b = src_argb[0];
		uint8_t g = src_argb[1];
		uint8_t r = src_argb[2];
		dst_rgb[0] = r;
		dst_rgb[1] = g;
		dst_rgb[2] = b;
		dst_rgb += 3;
		src_argb += 4;
	}
}

static int BGRAToRGB24(const uint8_t* src_argb, int src_stride_argb,
	uint8_t* dst_rgb24, int dst_stride_rgb24,
	int width, int height) {
	if (!src_argb || !dst_rgb24 || width <= 0 || height == 0) {
		return -1;
	}

	if (height < 0) {
		height = -height;
		src_argb = src_argb + (height - 1) * src_stride_argb;
		src_stride_argb = -src_stride_argb;
	}
	// Coalesce rows.
	if (src_stride_argb == width * 4 &&
		dst_stride_rgb24 == width * 3) {
		width *= height;
		height = 1;
		src_stride_argb = dst_stride_rgb24 = 0;
	}
	void(*BGRAToRGB24Row)(const uint8_t* src_argb, uint8_t* dst_rgb, int pix) =
		BGRAToRGB24Row_C;

	for (int y = 0; y < height; ++y) {
		BGRAToRGB24Row(src_argb, dst_rgb24, width);
		src_argb += src_stride_argb;
		dst_rgb24 += dst_stride_rgb24;
	}
	return 0;
}

static int BrightnessI420(uint8_t* src_argb, int width, int height, int brightness)
{
	for (int i = 0; i < width * height; ++i)
	{
		int pix = src_argb[i];
		pix += brightness;
		if (pix > 0xFF) pix = 0xFF;
		if (pix < 0)	pix = 0;
		src_argb[i] = pix;
	}
	return 0;
}

static int BrightnessI420(uint8_t* src, uint8_t* dst, int width, int height, int brightness)
{
	for (int i = 0; i < width * height; ++i)
	{
		int pix = src[i];
		pix += brightness;
		if (pix > 0xFF) pix = 0xFF;
		if (pix < 0)	pix = 0;
		dst[i] = pix;
	}
	return 0;
}

static int GammaI420(uint8_t* src, int width, int height, int gamma)
{
	//return qBound<int>(0, int( pow( value / 255.0, 100.0 / gamma ) * 255 ), 255 );

	for (int i = 0; i < width * height; ++i)
	{
		int pixel = src[i];

		pixel = int(std::pow<float>(float(pixel)/255.f, 100.f/float(gamma)) * 255.f);

		if (pixel > 0xFF) pixel = 0xFF;
		if (pixel < 0)pixel = 0;
		src[i] = pixel;
	}
	return 0;
}

static int ThresholdI420(uint8_t* src, uint8_t* dst, int width, int height, int threshold)
{
    for (int i = 0; i < width * height; ++i)
    {
        int pix = src[i];
        if (pix > threshold) dst[i] = 0xFF;
        if (pix < threshold) dst[i] = 0;
    }
    return 0;
}

static int ThresholdI420(uint8_t* src, uint8_t* dst, int width, int height, int low, int high)
{
	for (int i = 0; i < width * height; ++i)
	{
		int pix = src[i];
		//if (pix > 0xFF) pix = 0xFF;
		//if (pix < 0)	pix = 0;

		if (pix > low && pix < high)
			dst[i] = pix;
		else
			dst[i] = 0xFF;
	}
	return 0;
}


static int BrightnessI420SSE(uint8_t* src_argb, int width, int height, int brightness)
{
	uint8_t * src = src_argb;
	int loop = (width * height) / 64;
	int rest = (width * height) % 64;

	__m128i brigt = _mm_set1_epi8(brightness);

	while(loop--)
	{
		__m128i pix1 = _mm_loadu_si128((__m128i*)(&src[0]));
		__m128i pix2 = _mm_loadu_si128((__m128i*)(&src[16]));
		__m128i pix3 = _mm_loadu_si128((__m128i*)(&src[32]));
		__m128i pix4 = _mm_loadu_si128((__m128i*)(&src[48]));

		__m128i pix1a = _mm_adds_epu8(brigt, pix1);
		__m128i pix2a = _mm_adds_epu8(brigt, pix2);
		__m128i pix3a = _mm_adds_epu8(brigt, pix3);
		__m128i pix4a = _mm_adds_epu8(brigt, pix4);
		
		_mm_storeu_si128((__m128i*)&src[0], pix1a);
		_mm_storeu_si128((__m128i*)&src[16], pix2a);
		_mm_storeu_si128((__m128i*)&src[32], pix3a);
		_mm_storeu_si128((__m128i*)&src[48], pix4a);

		src+=64;
	}

	return 0;
}

/*
function contrastImage(imageData, contrast) {

    var data = imageData.data;
    var factor = (259 * (contrast + 255)) / (255 * (259 - contrast));

    for(var i=0;i<data.length;i+=4)
    {
        data[i] = factor * (data[i] - 128) + 128;
        data[i+1] = factor * (data[i+1] - 128) + 128;
        data[i+2] = factor * (data[i+2] - 128) + 128;
    }
    return imageData;
}
*/

static int ContrastI420(uint8_t* src_argb, int width, int height, int contrast)
{
	float real_factor = (259.f * (float(contrast) + 255.f)) / (255.f * (259.f - float(contrast)));

	for (int i = 0; i < width * height; ++i)
	{
		float henk = src_argb[i];
		float res = real_factor * (henk - 128.f) + 128.f;
		if (res < 0.f)	 res = 0.f;
		if (res > 255.f) res = 255.f;
		src_argb[i] = (uint8_t)res;
	}

	return 0;
}

static int ContrastI420INT(uint8_t* src_argb, int width, int height, int contrast)
{
	const int const_upper = 259 * (contrast + 255);
	const int const_downer = 255 * (259 - contrast);

	for (int i = 0; i < width * height; ++i)
	{
		int henk = src_argb[i];
		//int res = ((henk - 128) * 259 * (contrast + 255)) / (255 * (259 - contrast));
		int upper	= (henk - 128) * const_upper;
		int res		= upper / const_downer;
		res += 128;

		if (res < 0)	res = 0;
		if (res > 255)	res = 255;
		src_argb[i] = res;
	}

	return 0;
}

static inline __m128i muly(const __m128i &a, const __m128i &b)
{
	__m128i tmp1 = _mm_mul_epu32(a, b); /* mul 2,0*/
	__m128i tmp2 = _mm_mul_epu32(_mm_srli_si128(a, 4), _mm_srli_si128(b, 4)); /* mul 3,1 */
	return _mm_unpacklo_epi32(
		_mm_shuffle_epi32(tmp1, _MM_SHUFFLE(0, 0, 2, 0)),
		_mm_shuffle_epi32(tmp2, _MM_SHUFFLE(0, 0, 2, 0))); /* shuffle results to [63..0] and pack */
}


static int ContrastI420SSE(uint8_t* src_argb, int width, int height, int contrast)
{
	float temp_real_factor	= (259.f * (float(contrast) + 255.f)) / (255.f * (259.f - float(contrast)));

	__m128 real_factor		= _mm_set_ps1((float)temp_real_factor);
	__m128 henk128			= _mm_set_ps1(128.f);
	__m128 henkzero			= _mm_set_ps1(0.f);
	__m128 henkmax			= _mm_set_ps1(255.f);

	__m128i henk128int		= _mm_set1_epi16(128);

	uint8_t * src = src_argb;

	int loop = (width * height) / 16;
	int rest = (width * height) % 16;

	while(loop--)
	{
		//float henk = src_argb[i];
		//float res = real_factor * (henk - 128.f) + 128.f;
		//if (res < 0.f)	 res = 0.f;
		//if (res > 255.f) res = 255.f;
		//src_argb[i] = res;

		__m128i pix_a_hex = _mm_loadu_si128((__m128i*)(&src[0])); // load 16 pixels
		
		__m128i pix_a_oct_a = _mm_unpacklo_epi8(pix_a_hex, _mm_setzero_si128()); // split 16 pixels into 2x 8 pixels
		__m128i pix_a_oct_b = _mm_unpackhi_epi8(pix_a_hex, _mm_setzero_si128());

		__m128i ipix_a_quad_a = _mm_unpacklo_epi16(pix_a_oct_a, _mm_setzero_si128()); // split 8 pixels into 4x 4 pixels
		__m128i ipix_a_quad_b = _mm_unpackhi_epi16(pix_a_oct_a, _mm_setzero_si128());

		__m128i ipix_a_quad_c = _mm_unpacklo_epi16(pix_a_oct_b, _mm_setzero_si128());
		__m128i ipix_a_quad_d = _mm_unpackhi_epi16(pix_a_oct_b, _mm_setzero_si128());
	
		__m128 pix_a_quad_a = _mm_cvtepi32_ps(ipix_a_quad_a);
		__m128 pix_a_quad_b = _mm_cvtepi32_ps(ipix_a_quad_b);
		__m128 pix_a_quad_c = _mm_cvtepi32_ps(ipix_a_quad_c);
		__m128 pix_a_quad_d = _mm_cvtepi32_ps(ipix_a_quad_d);

		pix_a_quad_a = _mm_sub_ps(pix_a_quad_a, henk128); // (henk - 128.f)
		pix_a_quad_b = _mm_sub_ps(pix_a_quad_b, henk128);
		pix_a_quad_c = _mm_sub_ps(pix_a_quad_c, henk128);
		pix_a_quad_d = _mm_sub_ps(pix_a_quad_d, henk128);

		pix_a_quad_a = _mm_mul_ps(pix_a_quad_a, real_factor); // real_factor * henk
		pix_a_quad_b = _mm_mul_ps(pix_a_quad_b, real_factor);
		pix_a_quad_c = _mm_mul_ps(pix_a_quad_c, real_factor);
		pix_a_quad_d = _mm_mul_ps(pix_a_quad_d, real_factor);

		pix_a_quad_a = _mm_add_ps(pix_a_quad_a, henk128); // henk + 128.f
		pix_a_quad_b = _mm_add_ps(pix_a_quad_b, henk128);
		pix_a_quad_c = _mm_add_ps(pix_a_quad_c, henk128);
		pix_a_quad_d = _mm_add_ps(pix_a_quad_d, henk128);

		pix_a_oct_a = _mm_packus_epi32(_mm_cvttps_epi32(pix_a_quad_a), _mm_cvttps_epi32(pix_a_quad_b));
		pix_a_oct_b = _mm_packus_epi32(_mm_cvttps_epi32(pix_a_quad_c), _mm_cvttps_epi32(pix_a_quad_d));

		pix_a_hex = _mm_packus_epi16 (pix_a_oct_a, pix_a_oct_b);

		_mm_storeu_si128((__m128i*)&src[0],  pix_a_hex);
		src+=16;
	}

	return 0;
}

static int ContrastI420LOOKUP(uint8_t* src_argb, int width, int height, int contrast)
{
	uint32_t lookup[0xFF];

	float real_factor = (259.f * (float(contrast) + 255.f)) / (255.f * (259.f - float(contrast)));

	for (int i = 0; i < 0xFF; ++i)
	{
		float res = real_factor * float((i - 128)) + 128.f;
		if (res < 0.f)	 res = 0.f;
		if (res > 255.f) res = 255.f;
		lookup[i] = (uint32_t)res;
	}

	int loop = (width * height) / 32;
	int rest = (width * height) % 32;
	uint8_t * src = src_argb;
	while(loop--)
	{
		uint32_t *pixel1 = (uint32_t*)(src + 0);
		uint32_t *pixel2 = (uint32_t*)(src + 4);
		uint32_t *pixel3 = (uint32_t*)(src + 8);
		uint32_t *pixel4 = (uint32_t*)(src + 12);

		uint32_t *pixel5 = (uint32_t*)(src + 16);
		uint32_t *pixel6 = (uint32_t*)(src + 20);
		uint32_t *pixel7 = (uint32_t*)(src + 24);
		uint32_t *pixel8 = (uint32_t*)(src + 28);

#define CONTRAST_LOOKUP(x)                                                                                             \
    do                                                                                                                 \
    {                                                                                                                  \
        *(x) = lookup[*(x) >> 0 & 0xFF] << 0 | lookup[*(x) >> 8 & 0xFF] << 8 | lookup[*(x) >> 16 & 0xFF] << 16 |       \
               lookup[*(x) >> 24 & 0xFF] << 24;                                                                        \
    } while (0)

		CONTRAST_LOOKUP(pixel1);
		CONTRAST_LOOKUP(pixel2);
		CONTRAST_LOOKUP(pixel3);
		CONTRAST_LOOKUP(pixel4);

		CONTRAST_LOOKUP(pixel5);
		CONTRAST_LOOKUP(pixel6);
		CONTRAST_LOOKUP(pixel7);
		CONTRAST_LOOKUP(pixel8);

#undef CONTRAST_LOOKUP
		src += 32;
	}

	while(rest--)
    {
		*src = lookup[*src]; src++;
    }

	return 0;
}

static int ContrastI420LOOKUP(uint8_t* src, uint8_t* dst, int width, int height, int contrast)
{
	uint32_t lookup[0xFF];

	float real_factor = (259.f * (float(contrast) + 255.f)) / (255.f * (259.f - float(contrast)));

	for (int i = 0; i < 0xFF; ++i)
	{
		float res = real_factor * float((i - 128)) + 128.f;
		if (res < 0.f)	 res = 0.f;
		if (res > 255.f) res = 255.f;
		lookup[i] = (uint32_t)res;
	}

	int loop = (width * height) / 32;
	int rest = (width * height) % 32;

	while (loop--)
	{
		uint32_t *pixel1 = (uint32_t*)(src + 0);
		uint32_t *pixel2 = (uint32_t*)(src + 4);
		uint32_t *pixel3 = (uint32_t*)(src + 8);
		uint32_t *pixel4 = (uint32_t*)(src + 12);

		uint32_t *pixel5 = (uint32_t*)(src + 16);
		uint32_t *pixel6 = (uint32_t*)(src + 20);
		uint32_t *pixel7 = (uint32_t*)(src + 24);
		uint32_t *pixel8 = (uint32_t*)(src + 28);

		uint32_t *dpixel1 = (uint32_t*)(dst + 0);
		uint32_t *dpixel2 = (uint32_t*)(dst + 4);
		uint32_t *dpixel3 = (uint32_t*)(dst + 8);
		uint32_t *dpixel4 = (uint32_t*)(dst + 12);

		uint32_t *dpixel5 = (uint32_t*)(dst + 16);
		uint32_t *dpixel6 = (uint32_t*)(dst + 20);
		uint32_t *dpixel7 = (uint32_t*)(dst + 24);
		uint32_t *dpixel8 = (uint32_t*)(dst + 28);

#define CONTRAST_LOOKUP(a, x)                                                                                          \
    do                                                                                                                 \
    {                                                                                                                  \
        *(a) = lookup[*(x) >> 0 & 0xFF] << 0 | lookup[*(x) >> 8 & 0xFF] << 8 | lookup[*(x) >> 16 & 0xFF] << 16 |       \
               lookup[*(x) >> 24 & 0xFF] << 24;                                                                        \
    } while (0)

		CONTRAST_LOOKUP(dpixel1, pixel1);
		CONTRAST_LOOKUP(dpixel2, pixel2);
		CONTRAST_LOOKUP(dpixel3, pixel3);
		CONTRAST_LOOKUP(dpixel4, pixel4);

		CONTRAST_LOOKUP(dpixel5, pixel5);
		CONTRAST_LOOKUP(dpixel6, pixel6);
		CONTRAST_LOOKUP(dpixel7, pixel7);
		CONTRAST_LOOKUP(dpixel8, pixel8);

#undef CONTRAST_LOOKUP

		dst += 32;
		src += 32;
	}

	while (rest--)
	{
		*dst = lookup[*src]; src++;
		dst++;
	}

	return 0;
}

static int RangeI420float(uint8_t* src_argb, int width, int height, int black = 100, int white = 200)
{
	float min_norm = (float)black;
	float max_norm = (float)white;
	float divdelta = (max_norm - min_norm);
		
	for (int i = 0; i < width * height; ++i)
	{
		float g = src_argb[i];
		float henk = ((g - min_norm) * 255.f) / divdelta;
			
		if (henk < 0.f)
			henk = 0.f;

		if (henk > 255.f)
			henk = 255.f;

		src_argb[i] = (uint8_t)henk;
	}

	/*
	for (int i = 0; i < 0xFF; ++i)
	{
		printf("[% 3u] %u\n", i, mapping[i]);
	}
	*/

	//printf("min_norm: %u | max_norm: %u\n", min_norm, max_norm);
	return 0;
}

static int RangeI420(uint8_t* src_argb, int width, int height, int black = 100, int white = 200)
{
	int min_norm = black;
	int max_norm = white;
	int divdelta = (max_norm - min_norm);
		
	for (int i = 0; i < width * height; ++i)
	{
		int g = src_argb[i];

		int henk = ((g - min_norm) * 255) / divdelta;
		if (henk < 0)	henk = 0;
		if (henk > 255)	henk = 255;

		/*
		int henkxx = ((g << 8) - (min_norm << 8)) / divdelta;
		float henkf = ((float(g) - float(min_norm)) * 255.f) / float(divdelta);

		henkf = std::max<float>(std::min<float>(henkf, 255.f), 0.f);
		henkxx = std::max<int>(std::min<int>(henkxx, 255.f), 0.f);

		if (henk != (int)henkf)
		{
			int banaan = 3;
		}
		*/

		src_argb[i] = henk;
	}
	return 0;
}

static int RangeI420LOOKUP(uint8_t* src_argb, int width, int height, int black = 100, int white = 200)
{
	int min_norm = black;
	int max_norm = white;
	int divdelta = (max_norm - min_norm);

	uint8_t lookup[0xFF + 1];
	for (int i = 0; i <= 0xFF; ++i)
	{
		int henk = ((i - min_norm) * 255) / divdelta;
		if (henk < 0)	henk = 0;
		if (henk > 255)	henk = 255;
		lookup[i] = henk;
	}

	for (int i = 0; i < width * height; ++i)
	{
		uint8_t pixel = src_argb[i];
		src_argb[i] = lookup[pixel];
	}

	return 0;
}

static int RangeI420LOOKUP(uint8_t* src, uint8_t* dst, int width, int height, int black = 100, int white = 200)
{
	int min_norm = black;
	int max_norm = white;
	int divdelta = (max_norm - min_norm);

	uint8_t lookup[0xFF + 1];
	for (int i = 0; i <= 0xFF; ++i)
	{
		int henk = ((i - min_norm) * 255) / divdelta;
		if (henk < 0)	henk = 0;
		if (henk > 255)	henk = 255;
		lookup[i] = henk;
	}

	for (int i = 0; i < width * height; ++i)
	{
		uint8_t pixel = src[i];
		dst[i] = lookup[pixel];
	}

	return 0;
}

static int RangeI420LOOKUP2(uint8_t* src, uint8_t* dst, int width, int height, int black = 100, int white = 200)
{
	const int min_norm = black;
	const int max_norm = white;
	const int divdelta = (max_norm - min_norm);

	int lookup[0x100];
	for (int i = 0; i < 0x100; ++i)
	{
		int value = ((i - min_norm) * 255) / divdelta;
		if (value < 0) value = 0;
		if (value > 255)	value = 255;
		lookup[i] = value;
	}

	const uint32_t * quad_src = (uint32_t *)src;
	uint32_t * quad_dst = (uint32_t *)dst;

	unsigned int loop = (width * height)/8;
	while(loop--)
	{
		uint32_t pixel;
		
		pixel = *quad_src++;
		*dst++ = lookup[(pixel >> 0) & 0xFF];
		*dst++ = lookup[(pixel >> 8) & 0xFF];
		*dst++ = lookup[(pixel >> 16) & 0xFF];
		*dst++ = lookup[(pixel >> 24) & 0xFF];

		pixel = *quad_src++;
		*dst++ = lookup[(pixel >> 0) & 0xFF];
		*dst++ = lookup[(pixel >> 8) & 0xFF];
		*dst++ = lookup[(pixel >> 16) & 0xFF];
		*dst++ = lookup[(pixel >> 24) & 0xFF];
	}

	src = (uint8_t*)quad_src;
	loop = (width * height) % 8;
	while(loop--)
	{
		uint8_t pixel = *src++;
		*dst++ = lookup[pixel];
	}

	return 0;
}

// in order to check if its a dark image we create a histogram
static bool IsDarkImage(uint8_t* src, int width, int height)
{
	uint8_t histogram[0xFF + 1];
	memset(histogram, 0, sizeof(histogram));

	for (int i = 0; i < width * height; ++i)
		histogram[src[i]]++;

	int dark = 0;
	int light = 0;

	// how many dark pixels do we have
	for (int i = 0; i < 75; ++i)
		dark += histogram[i];

	for (int i = 255-75; i < 255; ++i)
		light += histogram[i];

	return dark > light;
}

static int AutoRangeI420LOOKUP(uint8_t* src_argb, int width, int height)
{
	int min_norm = 0xFF;
	int max_norm = 0x00;

	for (int i = 0; i < width * height; ++i)
	{
		uint8_t pixel = src_argb[i];
		min_norm = std::min<int>(min_norm, pixel);
		max_norm = std::max<int>(max_norm, pixel);
	}

	int divdelta = (max_norm - min_norm);

	//259
	uint8_t lookup[0xFF + 1];
	for (int i = 0; i <= 0xFF; ++i)
	{
		int henk = ((i - min_norm) * 255) / divdelta;
		if (henk < 0)	henk = 0;
		if (henk > 255)	henk = 255;
		lookup[i] = henk;
	}

	for (int i = 0; i < width * height; ++i)
	{
		uint8_t pixel = src_argb[i];
		src_argb[i] = lookup[pixel];
	}

	return 0;
}

/* auto contrast process step that takes the image histogram into account */
static int AutoHistoRangeI420(uint8_t* src_argb, int width, int height, int dark_threshold=30, int light_threshold=30)
{
	int histogram[0x100];

	int min_norm = 0x00;
	int max_norm = 0xFF;

	memset(histogram, 0, sizeof(histogram));

	/* first build image histogram */
	for (int i = 0; i < width * height; ++i)
	{
		uint8_t pixel = src_argb[i];
		histogram[pixel]++;
	}

	/* get histogram low bound */
	for (int i = 0; i < 0xFF; ++i)
	{
		if (histogram[i] > dark_threshold)
		{
			min_norm = i;
			break;
		}
	}

	/* get histogram high bound */
	for (int i = 0xFF; i > 0; --i)
	{
		if (histogram[i] > light_threshold)
		{
			max_norm = i;
			break;
		}
	}

	int divdelta = (max_norm - min_norm);

	//259
	uint8_t lookup[0xFF + 1];
	for (int i = 0; i <= 0xFF; ++i)
	{
		int henk = ((i - min_norm) * 255) / divdelta;
		if (henk < 0)	henk = 0;
		if (henk > 255)	henk = 255;
		lookup[i] = henk;
	}

	for (int i = 0; i < width * height; ++i)
	{
		uint8_t pixel = src_argb[i];
		src_argb[i] = lookup[pixel];
	}

	return 0;
}

static void Filter(const uint8_t * src, size_t width, size_t height, double contrast, double brightness, uint8_t * dst)
{
	const int shift = 8;
	size_t size = width*height;
	__m128i _contrast16 = _mm_set1_epi16(int16_t(contrast*(1 << shift)));
	__m128i _brightness16 = _mm_set1_epi16(int16_t(brightness*(1 << shift)));
	for (size_t i = 0; i < size; i += sizeof(__m128i))
	{
		__m128i _src8 = _mm_load_si128((__m128i*)(src + i));
		__m128i _srcLo16 = _mm_unpacklo_epi8(_src8, _mm_setzero_si128());
		__m128i _srcHi16 = _mm_unpackhi_epi8(_src8, _mm_setzero_si128());

		__m128i _dstLo16 = _mm_srai_epi16(_mm_add_epi16(_brightness16, _mm_mullo_epi16(_contrast16, _srcLo16)), shift);
		__m128i _dstHi16 = _mm_srai_epi16(_mm_add_epi16(_brightness16, _mm_mullo_epi16(_contrast16, _srcHi16)), shift);
		_mm_store_si128((__m128i*)(dst + i), _mm_packus_epi16(_dstLo16, _dstHi16));
	}
}

/* low pass filter.. aka kind of blur */
static int LowPassI420(uint8_t* src_argb, int width, int height)
{
	//uint8_t * temp = (uint8_t *)malloc(width * height);
	static uint8_t * temp = nullptr;
	if (!temp)
	{
		temp = (uint8_t *)malloc(1280 * 960);
		memset(temp, 0, 1280 * 960);
	}

	if (!temp)
		return -1;

	memcpy(temp, src_argb, width * height);

	for (int j = 1; j < height - 1; ++j)
	{
		uint8_t * line0 = &temp[(j-1) * width];
		uint8_t * line1 = &temp[(j-0) * width];
		uint8_t * line2 = &temp[(j+1) * width];

		uint8_t * dst = &src_argb[(j-0) * width];

		for (int i = 1; i < width - 1; ++i)
		{
			int line0_p0 = line0[i - 1];
			int line0_p1 = line0[i - 0];
			int line0_p2 = line0[i + 1];

			int line1_p0 = line1[i - 1];
			int line1_p1 = line1[i - 0];
			int line1_p2 = line1[i + 1];

			int line2_p0 = line2[i - 1];
			int line2_p1 = line2[i - 0];
			int line2_p2 = line2[i + 1];

			int sumtemp =
				line0_p0 + line0_p1 + line0_p2 +
				line1_p0 + line1_p1 + line1_p2 +
				line2_p0 + line2_p1 + line2_p2;

			dst[i] = sumtemp / 9;
		}
	}

	//memcpy(src_argb, temp, width * height);
	return 0;
}

// http://www1.adept.com/main/KE/DATA/ACE/AdeptSight_User/ImageProcessing_Operations.html
//Sharpen Filter
static float kernel_sharpen_div = 1.f;
static float kernel_sharpen[5 * 5] = {
	-1.f, -1.f, -1.f, -1.f, -1.f,
	-1.f, -1.f, -1.f, -1.f, -1.f,
	-1.f, -1.f, 25.f, -1.f, -1.f,
	-1.f, -1.f, -1.f, -1.f, -1.f,
	-1.f, -1.f, -1.f, -1.f, -1.f };

//SharpenLow Filter
static float kernel_sharpen_lowpass_div = 1.f;
static float kernel_sharpen_lowpass[5 * 5] = {
	-1.f, -3.f, -4.f, -3.f, -1.f,
	-3.f, 0.f, 6.f, 0.f, -3.f,
	-4.f, 6.f, 40.f, 6.f, -4.f,
	-3.f, 0.f, 6.f, 0.f, -3.f,
	-1.f, -3.f, -4.f, -3.f, -1 };

//Laplacian Filter
static float kernel_laplacian_div = 1.f;
static float kernel_laplacian[5 * 5] = {
	-1.f, -3.f, -4.f, -3.f, -1.f,
	-3.f, 0.f, 6.f, 0.f, -3.f,
	-4.f, 6.f, 20.f, 6.f, -4.f,
	-3.f, 0.f, 6.f, 0.f, -3.f,
	-1.f, -3.f, -4.f, -3.f, -1.f };

//Highpass Filter
static float kernel_highpass_div = 1.f;
static float kernel_highpass[5 * 5] = {
	-1.f, -1.f, -1.f, -1.f, -1.f,
	-1.f, -1.f, -1.f, -1.f, -1.f,
	-1.f, -1.f, 24.f, -1.f, -1.f,
	-1.f, -1.f, -1.f, -1.f, -1.f,
	-1.f, -1.f, -1.f, -1.f, -1.f };

#define gaus_mul(x) ((x)*571.f)

//http://dev.theomader.com/gaussian-kernel-calculator/
//Gaussian Filter (used to remove noise) (possible sigma=0.85)

/*
static float kernel_gaussian_div = 571.f;
static float kernel_gaussian[5 * 5] = {
	2.f, 7.f, 12.f, 7.f, 2.f,
	7.f, 31.f, 52.f, 31.f, 7.f,
	12.f, 52.f, 127.f, 52.f, 12.f,
	7.f, 31.f, 52.f, 31.f, 7.f,
	2.f, 7.f, 12.f, 7.f, 2.f };
*/

static float kernel_gaussian_div = gaus_mul(1.f);
static const float kernel_gaussian[5 * 5] = {
	gaus_mul(0.001391f),	gaus_mul(0.008957f),	gaus_mul(0.016599f),	gaus_mul(0.008957f),	gaus_mul(0.001391f),
	gaus_mul(0.008957f),	gaus_mul(0.057680f),	gaus_mul(0.106893f),	gaus_mul(0.057680f),	gaus_mul(0.008957f),
	gaus_mul(0.016599f),	gaus_mul(0.106893f),	gaus_mul(0.198097f),	gaus_mul(0.106893f),	gaus_mul(0.016599f),
	gaus_mul(0.008957f),	gaus_mul(0.057680f),	gaus_mul(0.106893f),	gaus_mul(0.057680f),	gaus_mul(0.008957f),
	gaus_mul(0.001391f),	gaus_mul(0.008957f),	gaus_mul(0.016599f),	gaus_mul(0.008957f),	gaus_mul(0.001391f)
};


//Discrete approximation to Gaussian function with sigma=1.0
/*
static const float kernel_gaussian_1_div = 273.f;
static const float kernel_gaussian_1[5 * 5] = {
	1.f, 4.f, 7.f, 4.f, 1.f,
	4.f,16.f,26.f,16.f, 4.f,
	7.f,26.f,41.f,26.f, 7.f,
	4.f,16.f,26.f,16.f, 4.f,
	1.f, 4.f, 7.f, 4.f, 1.f};
*/

static const float kernel_gaussian_1_div = gaus_mul(1.f);
static const float kernel_gaussian_1[5 * 5] = {
	gaus_mul(0.003765f),	gaus_mul(0.015019f),	gaus_mul(0.023792f),	gaus_mul(0.015019f),	gaus_mul(0.003765f),
	gaus_mul(0.015019f),	gaus_mul(0.059912f),	gaus_mul(0.094907f),	gaus_mul(0.059912f),	gaus_mul(0.015019f),
	gaus_mul(0.023792f),	gaus_mul(0.094907f),	gaus_mul(0.150342f),	gaus_mul(0.094907f),	gaus_mul(0.023792f),
	gaus_mul(0.015019f),	gaus_mul(0.059912f),	gaus_mul(0.094907f),	gaus_mul(0.059912f),	gaus_mul(0.015019f),
	gaus_mul(0.003765f),	gaus_mul(0.015019f),	gaus_mul(0.023792f),	gaus_mul(0.015019f),	gaus_mul(0.003765f)
};

static const float kernel_gaussian_125_div = gaus_mul(1.f);
static const float kernel_gaussian_125[5 * 5] = {
	gaus_mul(0.009355f), gaus_mul(0.023256f), gaus_mul(0.031498f), gaus_mul(0.023256f),	gaus_mul(0.009355f),
	gaus_mul(0.023256f), gaus_mul(0.057816f), gaus_mul(0.078305f), gaus_mul(0.057816f),	gaus_mul(0.023256f),
	gaus_mul(0.031498f), gaus_mul(0.078305f), gaus_mul(0.106055f), gaus_mul(0.078305f),	gaus_mul(0.031498f),
	gaus_mul(0.023256f), gaus_mul(0.057816f), gaus_mul(0.078305f), gaus_mul(0.057816f),	gaus_mul(0.023256f),
	gaus_mul(0.009355f), gaus_mul(0.023256f), gaus_mul(0.031498f), gaus_mul(0.023256f),	gaus_mul(0.009355f),
};

static const float kernel_gaussian_075_div = gaus_mul(1.f);
static const float kernel_gaussian_075[5 * 5] = {
	gaus_mul(0.000499f), gaus_mul(0.005137f), gaus_mul(0.011068f), gaus_mul(0.005137f), gaus_mul(0.000499f),
	gaus_mul(0.005137f), gaus_mul(0.052872f), gaus_mul(0.113921f), gaus_mul(0.052872f), gaus_mul(0.005137f),
	gaus_mul(0.011068f), gaus_mul(0.113921f), gaus_mul(0.245461f), gaus_mul(0.113921f), gaus_mul(0.011068f),
	gaus_mul(0.005137f), gaus_mul(0.052872f), gaus_mul(0.113921f), gaus_mul(0.052872f), gaus_mul(0.005137f),
	gaus_mul(0.000499f), gaus_mul(0.005137f), gaus_mul(0.011068f), gaus_mul(0.005137f), gaus_mul(0.000499f)
};

static const float kernel_gaussian_080_div = gaus_mul(1.f);
static const float kernel_gaussian_080[5 * 5] = {
	gaus_mul(0.000874f), gaus_mul(0.006976f), gaus_mul(0.013860f), gaus_mul(0.006976f), gaus_mul(0.000874f),
	gaus_mul(0.006976f), gaus_mul(0.055700f), gaus_mul(0.110656f), gaus_mul(0.055700f), gaus_mul(0.006976f),
	gaus_mul(0.013860f), gaus_mul(0.110656f), gaus_mul(0.219833f), gaus_mul(0.110656f), gaus_mul(0.013860f),
	gaus_mul(0.006976f), gaus_mul(0.055700f), gaus_mul(0.110656f), gaus_mul(0.055700f), gaus_mul(0.006976f),
	gaus_mul(0.000874f), gaus_mul(0.006976f), gaus_mul(0.013860f), gaus_mul(0.006976f), gaus_mul(0.000874f)
};

static float kernel_horizontal_sobel_div = 1.f;
static float kernel_horizontal_sobel[5 * 5] = {
	1.f, 4.f, 7.f, 4.f, 1.f,
	2.f, 10.f, 17.f, 10.f, 2.f,
	0.f, 0.f, 0.f, 0.f, 0.f,
	-2.f, -10.f, -17.f, -10.f, -2.f,
	-1.f, -4.f, -7.f, -4.f, -1.f };

static float kernel_vertical_sobel_div = 1.f;
static float kernel_vertical_sobel[5 * 5] = {
	-1.f, -2.f, 0.f, 2.f, 1.f,
	-4.f, -10.f, 0.f, 10.f, 4.f,
	-7.f, -17.f, 0.f, 17.f, 7.f,
	-4.f, -10.f, 0.f, 10.f, 4.f,
	-1.f, -2.f, 0.f, 2.f, 1.f };

// internal function for applying 5x5 kernel matrix processing
static int ApplyKernel5x5I420(uint8_t* src_argb, int width, int height, const float * kernel, const float kernel_div)
{
	static uint8_t * temp = nullptr;
	if (!temp)
	{
		temp = (uint8_t *)malloc(1280 * 960);
		memset(temp, 0, 1280 * 960);
	}

	if (!temp)
		return -1;

	memcpy(temp, src_argb, width * height);

	for (int j = 2; j < height - 2; ++j)
	{
		uint8_t * line0 = &temp[(j - 2) * width];
		uint8_t * line1 = &temp[(j - 1) * width];
		uint8_t * line2 = &temp[(j + 0) * width];
		uint8_t * line3 = &temp[(j + 1) * width];
		uint8_t * line4 = &temp[(j + 2) * width];

		uint8_t * dst = &src_argb[(j - 0) * width];

		for (int i = 2; i < width - 2; ++i)
		{
			double line0_p0 = double(line0[i - 2]) * kernel[0];
			double line0_p1 = double(line0[i - 1]) * kernel[1];
			double line0_p2 = double(line0[i + 0]) * kernel[2];
			double line0_p3 = double(line0[i + 1]) * kernel[3];
			double line0_p4 = double(line0[i + 2]) * kernel[4];

			double line1_p0 = double(line1[i - 2]) * kernel[5];
			double line1_p1 = double(line1[i - 1]) * kernel[6];
			double line1_p2 = double(line1[i + 0]) * kernel[7];
			double line1_p3 = double(line1[i + 1]) * kernel[8];
			double line1_p4 = double(line1[i + 2]) * kernel[9];

			double line2_p0 = double(line2[i - 2]) * kernel[10];
			double line2_p1 = double(line2[i - 1]) * kernel[11];
			double line2_p2 = double(line2[i + 0]) * kernel[12];
			double line2_p3 = double(line2[i + 1]) * kernel[13];
			double line2_p4 = double(line2[i + 2]) * kernel[14];

			double line3_p0 = double(line3[i - 2]) * kernel[15];
			double line3_p1 = double(line3[i - 1]) * kernel[16];
			double line3_p2 = double(line3[i + 0]) * kernel[17];
			double line3_p3 = double(line3[i + 1]) * kernel[18];
			double line3_p4 = double(line3[i + 2]) * kernel[19];

			double line4_p0 = double(line4[i - 2]) * kernel[20];
			double line4_p1 = double(line4[i - 1]) * kernel[21];
			double line4_p2 = double(line4[i + 0]) * kernel[22];
			double line4_p3 = double(line4[i + 1]) * kernel[23];
			double line4_p4 = double(line4[i + 2]) * kernel[24];

			double sumtemp =
				line0_p0 + line0_p1 + line0_p2 + line0_p3 + line0_p4 +
				line1_p0 + line1_p1 + line1_p2 + line1_p3 + line1_p4 +
				line2_p0 + line2_p1 + line2_p2 + line2_p3 + line2_p4 +
				line3_p0 + line3_p1 + line3_p2 + line3_p3 + line3_p4 +
				line4_p0 + line4_p1 + line4_p2 + line4_p3 + line4_p4;

			sumtemp /= kernel_div;

			if (sumtemp > 255)
				sumtemp = 255;
			if (sumtemp < 0)
				sumtemp = 0;

			dst[i] = (uint8_t)sumtemp;
		}
	}

	return 0;
}

static int ScaleDouble(uint8_t* src, uint8_t* dst, int width, int height)
{
	for (int i = 0; i < height; ++i)
	{
		memcpy(dst, src, width);
		dst+=width;
		memcpy(dst, src, width);
		dst+=width;
		src+=width;
	}
	return 0;
}

static int GaussianI420(uint8_t* src_argb, int width, int height)
{
	return ApplyKernel5x5I420(src_argb, width, height, kernel_gaussian, kernel_gaussian_div);
}

static int Gaussian_1_I420(uint8_t* src_argb, int width, int height)
{
	return ApplyKernel5x5I420(src_argb, width, height, kernel_gaussian_1, kernel_gaussian_1_div);
}

static int Gaussian_2_I420(uint8_t* src_argb, int width, int height)
{
	return ApplyKernel5x5I420(src_argb, width, height, kernel_gaussian_125, kernel_gaussian_125_div);
}

static int Gaussian_3_I420(uint8_t* src_argb, int width, int height)
{
	return ApplyKernel5x5I420(src_argb, width, height, kernel_gaussian_075, kernel_gaussian_075_div);
}

static int Gaussian_4_I420(uint8_t* src_argb, int width, int height)
{
	return ApplyKernel5x5I420(src_argb, width, height, kernel_gaussian_080, kernel_gaussian_080_div);
}

static int HighPassI420(uint8_t* src_argb, int width, int height)
{
	return ApplyKernel5x5I420(src_argb, width, height, kernel_highpass, kernel_highpass_div);
}

// internal function for applying 7x7 kernel matrix processing
static int ApplyKernel7x7I420(uint8_t* src_argb, int width, int height, const float * kernel, const float kernel_div)
{
	static uint8_t * temp = nullptr;
	if (!temp)
	{
		temp = (uint8_t *)malloc(1280 * 960);
		memset(temp, 0, 1280 * 960);
	}

	if (!temp)
		return -1;

	memcpy(temp, src_argb, width * height);

	for (int j = 3; j < height - 3; ++j)
	{
		uint8_t * line0 = &temp[(j - 3) * width];
		uint8_t * line1 = &temp[(j - 2) * width];
		uint8_t * line2 = &temp[(j - 1) * width];
		uint8_t * line3 = &temp[(j + 0) * width];
		uint8_t * line4 = &temp[(j + 1) * width];
		uint8_t * line5 = &temp[(j + 2) * width];
		uint8_t * line6 = &temp[(j + 3) * width];

		uint8_t * dst = &src_argb[(j - 0) * width];

		for (int i = 3; i < width - 3; ++i)
		{
			double line0_p0 = double(line0[i - 3]) * kernel[0];
			double line0_p1 = double(line0[i - 2]) * kernel[1];
			double line0_p2 = double(line0[i - 1]) * kernel[2];
			double line0_p3 = double(line0[i + 0]) * kernel[3];
			double line0_p4 = double(line0[i + 1]) * kernel[4];
			double line0_p5 = double(line0[i + 2]) * kernel[5];
			double line0_p6 = double(line0[i + 3]) * kernel[6];

			double line1_p0 = double(line1[i - 3]) * kernel[7];
			double line1_p1 = double(line1[i - 2]) * kernel[8];
			double line1_p2 = double(line1[i - 1]) * kernel[9];
			double line1_p3 = double(line1[i + 0]) * kernel[10];
			double line1_p4 = double(line1[i + 1]) * kernel[11];
			double line1_p5 = double(line1[i + 2]) * kernel[12];
			double line1_p6 = double(line1[i + 3]) * kernel[13];

			double line2_p0 = double(line2[i - 3]) * kernel[14];
			double line2_p1 = double(line2[i - 2]) * kernel[15];
			double line2_p2 = double(line2[i - 1]) * kernel[16];
			double line2_p3 = double(line2[i + 0]) * kernel[17];
			double line2_p4 = double(line2[i + 1]) * kernel[18];
			double line2_p5 = double(line2[i + 2]) * kernel[19];
			double line2_p6 = double(line2[i + 3]) * kernel[20];

			double line3_p0 = double(line3[i - 3]) * kernel[21];
			double line3_p1 = double(line3[i - 2]) * kernel[22];
			double line3_p2 = double(line3[i - 1]) * kernel[23];
			double line3_p3 = double(line3[i + 0]) * kernel[24];
			double line3_p4 = double(line3[i + 1]) * kernel[25];
			double line3_p5 = double(line3[i + 2]) * kernel[26];
			double line3_p6 = double(line3[i + 3]) * kernel[27];

			double line4_p0 = double(line4[i - 3]) * kernel[28];
			double line4_p1 = double(line4[i - 2]) * kernel[29];
			double line4_p2 = double(line4[i - 1]) * kernel[30];
			double line4_p3 = double(line4[i + 0]) * kernel[31];
			double line4_p4 = double(line4[i + 1]) * kernel[32];
			double line4_p5 = double(line4[i + 2]) * kernel[33];
			double line4_p6 = double(line4[i + 3]) * kernel[34];

			double line5_p0 = double(line5[i - 3]) * kernel[35];
			double line5_p1 = double(line5[i - 2]) * kernel[36];
			double line5_p2 = double(line5[i - 1]) * kernel[37];
			double line5_p3 = double(line5[i + 0]) * kernel[38];
			double line5_p4 = double(line5[i + 1]) * kernel[39];
			double line5_p5 = double(line5[i + 2]) * kernel[40];
			double line5_p6 = double(line5[i + 3]) * kernel[41];

			double line6_p0 = double(line6[i - 3]) * kernel[42];
			double line6_p1 = double(line6[i - 2]) * kernel[43];
			double line6_p2 = double(line6[i - 1]) * kernel[44];
			double line6_p3 = double(line6[i + 0]) * kernel[45];
			double line6_p4 = double(line6[i + 1]) * kernel[46];
			double line6_p5 = double(line6[i + 2]) * kernel[47];
			double line6_p6 = double(line6[i + 3]) * kernel[48];

			double sumtemp =
				line0_p0 + line0_p1 + line0_p2 + line0_p3 + line0_p4 + line0_p5 + line0_p6 +
				line1_p0 + line1_p1 + line1_p2 + line1_p3 + line1_p4 + line1_p5 + line1_p6 +
				line2_p0 + line2_p1 + line2_p2 + line2_p3 + line2_p4 + line2_p5 + line2_p6 +
				line3_p0 + line3_p1 + line3_p2 + line3_p3 + line3_p4 + line3_p5 + line3_p6 +
				line4_p0 + line4_p1 + line4_p2 + line4_p3 + line4_p4 + line4_p5 + line4_p6 +
				line5_p0 + line5_p1 + line5_p2 + line5_p3 + line5_p4 + line5_p5 + line5_p6 +
				line6_p0 + line6_p1 + line6_p2 + line6_p3 + line6_p4 + line6_p5 + line6_p6;

			sumtemp /= kernel_div;

			if (sumtemp > 255)
				sumtemp = 255;
			if (sumtemp < 0)
				sumtemp = 0;

			dst[i] = (uint8_t)sumtemp;
		}
	}

	return 0;
}

#if 0
#define gaus_mul2(x) ((x)*573.0)
static const float kernel_gaussian_7x7_080_div = gaus_mul2(1.f);
static const float kernel_gaussian_7x7_080[7 * 7] = {
	gaus_mul2(7.796e-7),	gaus_mul2(0.0000260542), gaus_mul2(0.0002080194), gaus_mul2(0.0004132579), gaus_mul2(0.0002080194), gaus_mul2(0.0000260542), gaus_mul2(7.796e-7),
	gaus_mul2(0.0000260542), gaus_mul2(0.0008707041), gaus_mul2(0.0069517777), gaus_mul2(0.0138106229), gaus_mul2(0.0069517777), gaus_mul2(0.0008707041), gaus_mul2(0.0000260542),
	gaus_mul2(0.0002080194), gaus_mul2(0.0069517777), gaus_mul2(0.0555036037), gaus_mul2(0.1102652266), gaus_mul2(0.0555036037), gaus_mul2(0.0069517777), gaus_mul2(0.0002080194),
	gaus_mul2(0.0004132579), gaus_mul2(0.0138106229), gaus_mul2(0.1102652266), gaus_mul2(0.2190564106), gaus_mul2(0.1102652266), gaus_mul2(0.0138106229), gaus_mul2(0.0004132579),
	gaus_mul2(0.0002080194), gaus_mul2(0.0069517777), gaus_mul2(0.0555036037), gaus_mul2(0.1102652266), gaus_mul2(0.0555036037), gaus_mul2(0.0069517777), gaus_mul2(0.0002080194),
	gaus_mul2(0.0000260542), gaus_mul2(0.0008707041), gaus_mul2(0.0069517777), gaus_mul2(0.0138106229), gaus_mul2(0.0069517777), gaus_mul2(0.0008707041), gaus_mul2(0.0000260542),
	gaus_mul2(7.796e-7),	gaus_mul2(0.0000260542), gaus_mul2(0.0002080194), gaus_mul2(0.0004132579), gaus_mul2(0.0002080194), gaus_mul2(0.0000260542), gaus_mul2(7.796e-7),
};

static const float kernel_gaussian_7x7_100_div = gaus_mul2(1.f);
static const float kernel_gaussian_7x7_100[7 * 7] = {
gaus_mul2(0.0000357582), gaus_mul2(	0.0003625309), gaus_mul2(	0.0014461764), gaus_mul2(	0.0022908874), gaus_mul2(	0.0014461764), gaus_mul2(	0.0003625309), gaus_mul2(	0.0000357582),
gaus_mul2(0.0003625309), gaus_mul2(	0.0036754807), gaus_mul2(	0.0146619028), gaus_mul2(	0.0232259138), gaus_mul2(	0.0146619028), gaus_mul2(	0.0036754807), gaus_mul2(	0.0003625309),
gaus_mul2(0.0014461764), gaus_mul2(	0.0146619028), gaus_mul2(	0.0584879674), gaus_mul2(	0.0926507637), gaus_mul2(	0.0584879674), gaus_mul2(	0.0146619028), gaus_mul2(	0.0014461764),
gaus_mul2(0.0022908874), gaus_mul2(	0.0232259138), gaus_mul2(	0.0926507637), gaus_mul2(	0.1467680346), gaus_mul2(	0.0926507637), gaus_mul2(	0.0232259138), gaus_mul2(	0.0022908874),
gaus_mul2(0.0014461764), gaus_mul2(	0.0146619028), gaus_mul2(	0.0584879674), gaus_mul2(	0.0926507637), gaus_mul2(	0.0584879674), gaus_mul2(	0.0146619028), gaus_mul2(	0.0014461764),
gaus_mul2(0.0003625309), gaus_mul2(	0.0036754807), gaus_mul2(	0.0146619028), gaus_mul2(	0.0232259138), gaus_mul2(	0.0146619028), gaus_mul2(	0.0036754807), gaus_mul2(	0.0003625309),
gaus_mul2(0.0000357582), gaus_mul2(	0.0003625309), gaus_mul2(	0.0014461764), gaus_mul2(	0.0022908874), gaus_mul2(	0.0014461764), gaus_mul2(	0.0003625309), gaus_mul2(	0.0000357582),
};

static const float kernel_gaussian_7x7_050_div = gaus_mul2(1.f);
static const float kernel_gaussian_7x7_050[7 * 7] = {
gaus_mul2(8.22e-14), gaus_mul2(	3.868665e-10), gaus_mul2(	4.50916318e-8), gaus_mul2(	1.956931668e-7), gaus_mul2(	4.50916318e-8), gaus_mul2(	3.868665e-10), gaus_mul2(	8.22e-14),
gaus_mul2(3.868665e-10), gaus_mul2(	0.0000018214509242), gaus_mul2(	0.000212301101039), gaus_mul2(	0.0009213655198062), gaus_mul2(	0.000212301101039), gaus_mul2(	0.0000018214509242), gaus_mul2(	3.868665e-10),
gaus_mul2(4.50916318e-8), gaus_mul2(	0.000212301101039), gaus_mul2(	0.0247449749561229), gaus_mul2(	0.1073907134793947), gaus_mul2(	0.0247449749561229), gaus_mul2(	0.000212301101039), gaus_mul2(	4.50916318e-8),
gaus_mul2(1.956931668e-7), gaus_mul2(	0.0009213655198062), gaus_mul2(	0.1073907134793947), gaus_mul2(	0.4660649429657137), gaus_mul2(	0.1073907134793946), gaus_mul2(	0.0009213655198062), gaus_mul2(	1.956931668e-7),
gaus_mul2(4.50916318e-8), gaus_mul2(	0.000212301101039), gaus_mul2(	0.0247449749561229), gaus_mul2(	0.1073907134793946), gaus_mul2(	0.0247449749561229), gaus_mul2(	0.000212301101039), gaus_mul2(	4.50916318e-8),
gaus_mul2(3.868665e-10), gaus_mul2(	0.0000018214509242), gaus_mul2(	0.000212301101039), gaus_mul2(	0.0009213655198062), gaus_mul2(	0.000212301101039), gaus_mul2(	0.0000018214509242), gaus_mul2(	3.868665e-10),
gaus_mul2(8.22e-14), gaus_mul2(	3.868665e-10), gaus_mul2(	4.50916318e-8), gaus_mul2(	1.956931668e-7), gaus_mul2(	4.50916318e-8), gaus_mul2(	3.868665e-10), gaus_mul2(	8.22e-14),
};



static int Gaussian_7x7_I420(uint8_t* src_argb, int width, int height)
{
	//return ApplyKernel7x7I420(src_argb, width, height, kernel_gaussian_7x7_100, kernel_gaussian_7x7_100_div);
	//return ApplyKernel7x7I420(src_argb, width, height, kernel_gaussian_7x7_050, kernel_gaussian_7x7_050_div);
	return ApplyKernel7x7I420(src_argb, width, height, kernel_gaussian_7x7_080, kernel_gaussian_7x7_080_div);
}

#endif

static int LowPassI420(uint8_t* src, uint8_t* dst, int width, int height)
{
	memcpy(dst, src, width * height);

	for (int j = 1; j < height - 1; ++j)
	{
		uint8_t * line0 = &dst[(j - 1) * width];
		uint8_t * line1 = &dst[(j - 0) * width];
		uint8_t * line2 = &dst[(j + 1) * width];

		uint8_t * dst_pixel = &src[(j - 0) * width];

		for (int i = 1; i < width - 1; ++i)
		{
			int line0_p0 = line0[i - 1];
			int line0_p1 = line0[i - 0];
			int line0_p2 = line0[i + 1];

			int line1_p0 = line1[i - 1];
			int line1_p1 = line1[i - 0];
			int line1_p2 = line1[i + 1];

			int line2_p0 = line2[i - 1];
			int line2_p1 = line2[i - 0];
			int line2_p2 = line2[i + 1];

			int sumtemp =
				line0_p0 + line0_p1 + line0_p2 +
				line1_p0 + line1_p1 + line1_p2 +
				line2_p0 + line2_p1 + line2_p2;

			dst_pixel[i] = sumtemp / 9;
		}
	}
	return 0;
}

static int LowPassI420SSE(uint8_t* src_argb, int width, int height)
{
	//uint8_t * temp = (uint8_t *)malloc(width * height);
	static uint8_t * temp = nullptr;
	if (!temp)
		temp = (uint8_t *)malloc(1280 * 960);

	if (!temp)
		return -1;

	//memset(temp, 0, width * height);
	memcpy(temp, src_argb, width * height);

	for (int j = 1; j < height - 1; ++j)
	{
		uint8_t * line0 = &src_argb[(j - 1) * width];
		uint8_t * line1 = &src_argb[(j - 0) * width];
		uint8_t * line2 = &src_argb[(j + 1) * width];

		uint8_t * dst = &temp[(j - 0) * width];

		for (int i = 1; i < width - 1; ++i)
		{
			uint8_t line0_p0 = line0[i - 1];
			uint8_t line0_p1 = line0[i - 0];
			uint8_t line0_p2 = line0[i + 1];

			uint8_t line1_p0 = line1[i - 1];
			uint8_t line1_p1 = line1[i - 0];
			uint8_t line1_p2 = line1[i + 1];

			uint8_t line2_p0 = line2[i - 1];
			uint8_t line2_p1 = line2[i - 0];
			uint8_t line2_p2 = line2[i + 1];

			int temp =
				line0_p0 + line0_p1 + line0_p2 +
				line1_p0 + line1_p1 + line1_p2 +
				line2_p0 + line2_p1 + line2_p2;

			temp = temp / 9;

			dst[i] = temp;
		}
	}

	memcpy(src_argb, temp, width * height);
	//free(temp);

	return 0;
}

static int SharpenI420(uint8_t* src_argb, int width, int height)
{
	static int templates[25] = {
		-1, -4, -7, -4, -1,
		-4, -16, -26, -16, -4,
		-7, -26, 505, -26, -7,
		-4, -16, -26, -16, -4,
		-1, -4, -7, -4, -1 };

	static uint8_t * temp = nullptr;
	if (!temp)
		temp = (uint8_t *)malloc(1280 * 960);

	if (!temp)
		return -1;

	memcpy(temp, src_argb, width * height * sizeof(unsigned char));

	for (int j = 2; j < height - 2; j++)
	{
		for (int i = 2; i < width - 2; i++)
		{
			int sum = 0;
			int index = 0;
			for (int m = j - 2; m < j + 3; m ++)
			{
				for (int n = i - 2; n < i + 3; n++)
				{
					sum += src_argb[m * width + n] * templates[index ++];
				}
			}
			sum /= 273;
			if (sum > 255)
				sum = 255;
			if (sum < 0)
				sum = 0;
			temp[j * width + i] = sum;
		}
	}

	memcpy(src_argb, temp, width * height);
	return 0;
}

static void dump_histogram(uint8_t* src_argb, int width, int height)
{
    unsigned int histogram[0xFF] = {0};
	for (int i = 0; i < width * height; ++i)
		histogram[src_argb[i]]++;
}

static void SaveGrayBitmap(const std::string & filename, int width, int height, uint8_t * data)
{
	BITMAPFILEHEADER bitmapFileHeader;
	memset(&bitmapFileHeader, 0xff, sizeof(BITMAPFILEHEADER));
	bitmapFileHeader.bfType = ('B' | 'M' << 8);
	bitmapFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bitmapFileHeader.bfSize = bitmapFileHeader.bfOffBits + (width + (width % 4 ? (4 - width % 4) : 0)) * height; // multiply by 3 if you wanna switch to RGB

	BITMAPINFOHEADER bitmapInfoHeader;
	memset(&bitmapInfoHeader, 0, sizeof(BITMAPINFOHEADER));
	bitmapInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfoHeader.biWidth = width;
	bitmapInfoHeader.biHeight = height;
	bitmapInfoHeader.biPlanes = 1;
	bitmapInfoHeader.biBitCount = 8; // this means grayscale, change to 24 if you wanna switch to RGB

	FILE *filePtr = fopen(filename.c_str(), "wb");
	if (filePtr == NULL)
		return;

	fwrite(&bitmapFileHeader, 1, sizeof(BITMAPFILEHEADER), filePtr);

	/* write the bitmap info header */
	fwrite(&bitmapInfoHeader, 1, sizeof(BITMAPINFOHEADER), filePtr);

	// bitmaps grayscale must have a table of colors... don't write this table if you want RGB
	unsigned char grayscale[4];

	for (int i(0); i < 256; ++i)
	{
		memset(grayscale, i, sizeof(grayscale));
		fwrite(grayscale, 1, sizeof(grayscale), filePtr);
	}

	// here we record the pixels... I created a gradient...
	// remember that the pixels ordem is from left to right, "bottom to top"...
	unsigned char pixel[1]; // do pixel[ 3 ] if you want RGB

	unsigned int imgsize = width * height;

	for (int y = 0; y < height; ++y)
	{
		// + ( p_width % 4 ? ( 4 - p_width % 4 ) : 0 ) because BMP has a padding of 4 bytes per line
		for (int x = 0; x < width + (width % 4 ? (4 - width % 4) : 0); ++x)
		{
			if (x > width) /* check if we need to write padding... */
			{
				pixel[0] = 0;
				fwrite(pixel, 1, 1, filePtr);
			}
			else
			{
				int index = imgsize - (((y)* width) + (width - x));
				pixel[0] = data[index];
				fwrite(pixel, 1, 1, filePtr);
			}
		}
	}

	fclose(filePtr);
}

static void dump_histogram(const std::string & filename, uint8_t* src_argb, int width, int height)
{
	/* calculate histogram */
	unsigned int histogram[256];
	memset(histogram, 0, sizeof(histogram));

	unsigned int highest = 0;

	for (int i = 0; i < width * height; ++i)
	{
		histogram[src_argb[i]]++;
		highest = std::max<int>(highest, histogram[src_argb[i]]);
	}

	float div_me = float(highest) / 255.f;

	/* create histogram bitmap */
	uint8_t * bitmap = (uint8_t *)malloc(256 * 256);
	if (!bitmap)
		return;

	uint8_t * dst = bitmap;

	for (int y = int(float(highest) / div_me); y >= 0; --y)
	{
		for (int x = 0; x < 255; ++x)
		{
			if (float(histogram[x])/div_me >= y)
				*dst++ = 0x0;
			else
				*dst++ = 0xFF;
		}
	}

	SaveGrayBitmap(filename, 255, 255, bitmap);
	free(bitmap);
}
} // namespace sti

#endif