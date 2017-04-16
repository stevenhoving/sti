#pragma once

#include <stdint.h>
#include <xmmintrin.h>
#include <emmintrin.h>
#include <immintrin.h>
#include <tmmintrin.h>
#include <array>
#include <cassert>

#ifdef _MSC_VER
#define __packed_struct(x) __pragma(pack(push, 1)) struct x
#define __packed_struct_end __pragma(pack(pop))

#define __no_unroll __pragma(loop(no_vector))
#else
#define __packed_struct(x) struct __attribute__((__packed__)) x
#define __packed_struct_end

#define __no_unroll _Pragma("nounroll")
#endif

__packed_struct(magics)
{
    uint16_t mul_value;
    uint8_t shift;
};
__packed_struct_end

static_assert(sizeof(magics) == 3);

/* division by multiplication lookup table for 16 bits div by a 8 bits */
constexpr std::array<magics, 256> s16_div_lookup = { {
    { 0,0 },{ 0,0 },
    { 0x8001,0 },{ 0x5556,0 },{ 0x8001,1 },{ 0x6667,1 },
    { 0x2AAB,0 },{ 0x4925,1 },{ 0x8001,2 },{ 0x1C72,0 },
    { 0x6667,2 },{ 0x1746,0 },{ 0x2AAB,1 },{ 0x4EC5,2 },
    { 0x4925,2 },{ 0x8889,3 },{ 0x8001,3 },{ 0x7879,3 },
    { 0x0E39,0 },{ 0xD795,4 },{ 0x6667,3 },{ 0xC30D,4 },
    { 0x0BA3,0 },{ 0xB217,4 },{ 0x2AAB,2 },{ 0x147B,1 },
    { 0x4EC5,3 },{ 0x97B5,4 },{ 0x4925,3 },{ 0x469F,3 },
    { 0x8889,4 },{ 0x4211,3 },{ 0x8001,4 },{ 0x07C2,0 },
    { 0x7879,4 },{ 0x0EA1,1 },{ 0x0E39,1 },{ 0x1BAD,2 },
    { 0xD795,5 },{ 0x6907,4 },{ 0x6667,4 },{ 0xC7CF,5 },
    { 0xC30D,5 },{ 0x2FA1,3 },{ 0x0BA3,1 },{ 0x2D83,3 },
    { 0xB217,5 },{ 0xAE4D,5 },{ 0x2AAB,3 },{ 0x0A73,1 },
    { 0x147B,2 },{ 0xA0A1,5 },{ 0x4EC5,4 },{ 0x4D49,4 },
    { 0x97B5,5 },{ 0x94F3,5 },{ 0x4925,4 },{ 0x8FB9,5 },
    { 0x469F,4 },{ 0x456D,4 },{ 0x8889,5 },{ 0x2193,3 },
    { 0x4211,4 },{ 0x8209,5 },{ 0x8001,5 },{ 0x0FC1,2 },
    { 0x03E1,0 },{ 0x7A45,5 },{ 0x7879,5 },{ 0x3B5D,4 },
    { 0x0EA1,2 },{ 0x39B1,4 },{ 0x0E39,2 },{ 0xE071,6 },
    { 0x1BAD,3 },{ 0xDA75,6 },{ 0xD795,6 },{ 0x1A99,3 },
    { 0x6907,5 },{ 0x67B3,5 },{ 0x6667,5 },{ 0x6523,5 },
    { 0xC7CF,6 },{ 0x62B3,5 },{ 0xC30D,6 },{ 0x6061,5 },
    { 0x2FA1,4 },{ 0xBC53,6 },{ 0x0BA3,2 },{ 0x1703,3 },
    { 0x2D83,4 },{ 0x2D03,4 },{ 0xB217,6 },{ 0xB02D,6 },
    { 0xAE4D,6 },{ 0xAC77,6 },{ 0x2AAB,4 },{ 0xA8E9,6 },
    { 0x0A73,2 },{ 0x0296,0 },{ 0x147B,3 },{ 0x1447,3 },
    { 0xA0A1,6 },{ 0x4F89,5 },{ 0x4EC5,5 },{ 0x4E05,5 },
    { 0x4D49,5 },{ 0x04C9,1 },{ 0x97B5,6 },{ 0x0965,2 },
    { 0x94F3,6 },{ 0x939B,6 },{ 0x4925,5 },{ 0x487F,5 },
    { 0x8FB9,6 },{ 0x8E79,6 },{ 0x469F,5 },{ 0x8C09,6 },
    { 0x456D,5 },{ 0x89AF,6 },{ 0x8889,6 },{ 0x10ED,3 },
    { 0x2193,4 },{ 0x8535,6 },{ 0x4211,5 },{ 0x8313,6 },
    { 0x8209,6 },{ 0x8103,6 },{ 0x8001,6 },{ 0x3F81,5 },
    { 0x0FC1,3 },{ 0x3E89,5 },{ 0x03E1,1 },{ 0x7B31,6 },
    { 0x7A45,6 },{ 0x795D,6 },{ 0x7879,6 },{ 0x0EF3,3 },
    { 0x3B5D,5 },{ 0x75DF,6 },{ 0x0EA1,3 },{ 0x7433,6 },
    { 0x39B1,5 },{ 0x7293,6 },{ 0x0E39,3 },{ 0x70FF,6 },
    { 0xE071,7 },{ 0x6F75,6 },{ 0x1BAD,4 },{ 0x36FB,5 },
    { 0xDA75,7 },{ 0x6C81,6 },{ 0xD795,7 },{ 0x358B,5 },
    { 0x1A99,4 },{ 0xD369,7 },{ 0x6907,6 },{ 0x1A17,4 },
    { 0x67B3,6 },{ 0x19C3,4 },{ 0x6667,6 },{ 0x1971,4 },
    { 0x6523,6 },{ 0x1921,4 },{ 0xC7CF,7 },{ 0xC699,7 },
    { 0x62B3,6 },{ 0x1887,4 },{ 0xC30D,7 },{ 0x60F3,6 },
    { 0x6061,6 },{ 0xBFA1,7 },{ 0x2FA1,5 },{ 0x5EB5,6 },
    { 0xBC53,7 },{ 0x02ED,1 },{ 0x0BA3,3 },{ 0x5C91,6 },
    { 0x1703,4 },{ 0x0B71,3 },{ 0x2D83,5 },{ 0x5A85,6 },
    { 0x2D03,5 },{ 0x0B31,3 },{ 0xB217,7 },{ 0x0589,2 },
    { 0xB02D,7 },{ 0x2BCF,5 },{ 0xAE4D,7 },{ 0xAD61,7 },
    { 0xAC77,7 },{ 0x0AB9,3 },{ 0x2AAB,5 },{ 0xA9C9,7 },
    { 0xA8E9,7 },{ 0xA80B,7 },{ 0x0A73,3 },{ 0x532B,6 },
    { 0x014B,0 },{ 0x5255,6 },{ 0x147B,4 },{ 0xA307,7 },
    { 0x1447,4 },{ 0x285B,5 },{ 0xA0A1,7 },{ 0x9FD9,7 },
    { 0x4F89,6 },{ 0x9E4D,7 },{ 0x4EC5,6 },{ 0x4E65,6 },
    { 0x4E05,6 },{ 0x9B4D,7 },{ 0x4D49,6 },{ 0x133B,4 },
    { 0x04C9,2 },{ 0x9869,7 },{ 0x97B5,7 },{ 0x4B81,6 },
    { 0x0965,3 },{ 0x95A1,7 },{ 0x94F3,7 },{ 0x4A23,6 },
    { 0x939B,7 },{ 0x4979,6 },{ 0x4925,6 },{ 0x91A3,7 },
    { 0x487F,6 },{ 0x905B,7 },{ 0x8FB9,7 },{ 0x11E3,4 },
    { 0x8E79,7 },{ 0x8DDB,7 },{ 0x469F,6 },{ 0x8CA3,7 },
    { 0x8C09,7 },{ 0x8B71,7 },{ 0x456D,6 },{ 0x2291,5 },
    { 0x89AF,7 },{ 0x891B,7 },{ 0x8889,7 },{ 0x10FF,4 },
    { 0x10ED,4 },{ 0x436D,6 },{ 0x2193,5 },{ 0x0217,1 },
    { 0x8535,7 },{ 0x4255,6 },{ 0x4211,6 },{ 0x41CD,6 },
    { 0x8313,7 },{ 0x828D,7 },{ 0x8209,7 },{ 0x8185,7 },
    { 0x8103,7 },{ 0x8081,7 }
} };

void range_i420_sse_c(const uint8_t *src, uint8_t *dst, const int width, const int height,
    const int black /*= 100*/, const int white /*= 200*/)
{
    const int min_n = black;
    const int max_n = white;
    const int divdelta = (max_n - min_n);
    assert(divdelta > 2); // \todo fix div by 2

    auto div = s16_div_lookup[divdelta];
    div.shift += 16;

    const auto minnorm = _mm_set1_epi16(min_n);
    const auto divmul = _mm_set1_epi32(div.mul_value * 255);
    const auto src_size = width * height;
    const auto sse_size = static_cast<int>(src_size & ~15ul);

    int i = 0;

    __no_unroll
    do
    {
        auto chunk = _mm_lddqu_si128((__m128i *)&src[i]);

        // 8 bit -> 16 bit
        auto lo = _mm_unpacklo_epi8(chunk, _mm_setzero_si128());
        auto hi = _mm_unpackhi_epi8(chunk, _mm_setzero_si128());

        lo = _mm_sub_epi16(lo, minnorm);
        hi = _mm_sub_epi16(hi, minnorm);

        // store the 'negative' flag :P
        const auto lo_neg_mask = _mm_srli_epi32(lo, 15);
        const auto hi_neg_mask = _mm_srli_epi32(hi, 15);

        // 16 bit -> 32 bit
        auto lo_lo = _mm_unpacklo_epi16(lo, _mm_setzero_si128());
        auto lo_hi = _mm_unpackhi_epi16(lo, _mm_setzero_si128());
        auto hi_lo = _mm_unpacklo_epi16(hi, _mm_setzero_si128());
        auto hi_hi = _mm_unpackhi_epi16(hi, _mm_setzero_si128());

        lo_lo = _mm_mullo_epi32(lo_lo, divmul);
        lo_hi = _mm_mullo_epi32(lo_hi, divmul);
        hi_lo = _mm_mullo_epi32(hi_lo, divmul);
        hi_hi = _mm_mullo_epi32(hi_hi, divmul);

        lo_lo = _mm_srli_epi32(lo_lo, div.shift);
        lo_hi = _mm_srli_epi32(lo_hi, div.shift);
        hi_lo = _mm_srli_epi32(hi_lo, div.shift);
        hi_hi = _mm_srli_epi32(hi_hi, div.shift);

        lo = _mm_packus_epi32(lo_lo, lo_hi);
        hi = _mm_packus_epi32(hi_lo, hi_hi);

        // if value is negative do a +1
        lo = _mm_add_epi16(lo, lo_neg_mask);
        hi = _mm_add_epi16(hi, hi_neg_mask);

        chunk = _mm_packus_epi16(lo, hi);
        _mm_store_si128((__m128i*)&dst[i], chunk);

        i += 16;
    } while (i < sse_size);

    for (; i < src_size; ++i)
    {
        const int g = src[i];
        int henk = ((g - min_n) * 255) / divdelta;
        if (henk < 0)    henk = 0;
        if (henk > 255)  henk = 255;
        dst[i] = henk;
    }
}

