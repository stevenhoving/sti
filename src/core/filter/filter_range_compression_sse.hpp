#pragma once

#include <stdint.h>
#include <xmmintrin.h>
#include <emmintrin.h>
#include <immintrin.h>
#include <tmmintrin.h>
#include <array>
#include <cassert>

#include "core\sse\sse_utilities.hpp"
#include "core\sse\sse_integer_division_s16.hpp"

/*
(pixel - black)
---------------  * 255 = dst
(white - black)
*/
static
void range_i420_sse_c(const uint8_t *src, uint8_t *dst, const int width, const int height, const int black, const int white)
{
    const int min_norm = black;
    const int max_norm = white;
    const int div_delta = (max_norm - min_norm);

    assert(div_delta <= 255);
    assert(div_delta > 1);

    auto div = s16_div_lookup[div_delta];
    div.shift += 16;

    const auto minnorm = _mm_set1_epi32(min_norm);
    const auto divmul = _mm_set1_epi32(div.mul_value);
    const auto mul255 = _mm_set1_epi32(255);

    auto src_size = width * height;
    auto sse_size = sti::detail::align_down(src_size, 16);

    int i = 0;
    __no_unroll
    for (; i < sse_size;)
    {
        auto chunk = _mm_lddqu_si128((__m128i *)&src[i]);

        // 8 bit -> 16 bit
        auto lo = _mm_unpacklo_epi8(chunk, _mm_setzero_si128());
        auto hi = _mm_unpackhi_epi8(chunk, _mm_setzero_si128());

        // 16 bit -> 32 bit
        auto lo_lo = _mm_unpacklo_epi16(lo, _mm_setzero_si128());
        auto lo_hi = _mm_unpackhi_epi16(lo, _mm_setzero_si128());
        auto hi_lo = _mm_unpacklo_epi16(hi, _mm_setzero_si128());
        auto hi_hi = _mm_unpackhi_epi16(hi, _mm_setzero_si128());

        lo_lo = _mm_sub_epi32(lo_lo, minnorm);
        lo_hi = _mm_sub_epi32(lo_hi, minnorm);
        hi_lo = _mm_sub_epi32(hi_lo, minnorm);
        hi_hi = _mm_sub_epi32(hi_hi, minnorm);

        // store the 'negative' flag :P
        auto lo_lo_neg_mask = _mm_srli_epi32(lo_lo, 31);
        auto lo_hi_neg_mask = _mm_srli_epi32(lo_hi, 31);
        auto hi_lo_neg_mask = _mm_srli_epi32(hi_lo, 31);
        auto hi_hi_neg_mask = _mm_srli_epi32(hi_hi, 31);

        lo_lo = _mm_mullo_epi32(lo_lo, mul255);
        lo_hi = _mm_mullo_epi32(lo_hi, mul255);
        hi_lo = _mm_mullo_epi32(hi_lo, mul255);
        hi_hi = _mm_mullo_epi32(hi_hi, mul255);

        lo_lo = _mm_mullo_epi32(lo_lo, divmul);
        lo_hi = _mm_mullo_epi32(lo_hi, divmul);
        hi_lo = _mm_mullo_epi32(hi_lo, divmul);
        hi_hi = _mm_mullo_epi32(hi_hi, divmul);

        lo_lo = _mm_srai_epi32(lo_lo, div.shift);
        lo_hi = _mm_srai_epi32(lo_hi, div.shift);
        hi_lo = _mm_srai_epi32(hi_lo, div.shift);
        hi_hi = _mm_srai_epi32(hi_hi, div.shift);

        // if value is negative do a +1
        lo_lo = _mm_add_epi32(lo_lo, lo_lo_neg_mask);
        lo_hi = _mm_add_epi32(lo_hi, lo_hi_neg_mask);
        hi_lo = _mm_add_epi32(hi_lo, hi_lo_neg_mask);
        hi_hi = _mm_add_epi32(hi_hi, hi_hi_neg_mask);

        lo_lo = _mm_max_epi32(lo_lo, _mm_set1_epi32(0));
        lo_hi = _mm_max_epi32(lo_hi, _mm_set1_epi32(0));
        hi_lo = _mm_max_epi32(hi_lo, _mm_set1_epi32(0));
        hi_hi = _mm_max_epi32(hi_hi, _mm_set1_epi32(0));

        lo_lo = _mm_min_epi32(lo_lo, _mm_set1_epi32(255));
        lo_hi = _mm_min_epi32(lo_hi, _mm_set1_epi32(255));
        hi_lo = _mm_min_epi32(hi_lo, _mm_set1_epi32(255));
        hi_hi = _mm_min_epi32(hi_hi, _mm_set1_epi32(255));

        lo = _mm_packus_epi32(lo_lo, lo_hi);
        hi = _mm_packus_epi32(hi_lo, hi_hi);

        chunk = _mm_packus_epi16(lo, hi);

        _mm_storeu_si128((__m128i*)&dst[i], chunk);

        i += 16;
    }

    for (; i < src_size; ++i)
    {
        const int pixel = src[i];
        const auto temp = ((pixel - min_norm) * 255) / div_delta;
        dst[i] = sti::detail::clamp(temp, 0, 255);
    }
}

static
void range_i420_c(const uint8_t *src, uint8_t *dst, int width, int height,
    int black, int white)
{
    int min_norm = black;
    int max_norm = white;
    int divdelta = (max_norm - min_norm);

    for (int i = 0; i < width * height; ++i)
    {
        const int pixel = src[i];
        const int temp = ((pixel - min_norm) * 255) / divdelta;
        dst[i] = sti::detail::clamp(temp, 0, 255);
    }
}
