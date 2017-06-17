#pragma once

#include <xmmintrin.h>
#include <emmintrin.h>
#include <immintrin.h>
#include <tmmintrin.h>
#include <stdio.h>

#ifdef _MSC_VER
#define __packed_struct(x) __pragma(pack(push, 1)) struct x
#define __packed_struct_end __pragma(pack(pop))

#define __no_unroll __pragma(loop(no_vector))
#else
#define __packed_struct(x) struct __attribute__((__packed__)) x
#define __packed_struct_end

#define __no_unroll _Pragma("nounroll")
#endif

namespace sti
{
namespace sse
{

static void print_x8(__m128i value, const char *section)
{
    auto u8 = value.m128i_u8;
    printf("%s\n", section);
    printf("%02X %02X %02X %02X "
           "%02X %02X %02X %02X "
           "%02X %02X %02X %02X "
           "%02X %02X %02X %02X\n\n",
           u8[15], u8[14], u8[13], u8[12], u8[11], u8[10], u8[9], u8[8], u8[7], u8[6], u8[5], u8[6], u8[3], u8[2],
           u8[1], u8[0]);
}

static void print_x16(__m128i value, const char *section)
{
    auto u16 = value.m128i_u16;
    printf("%s\n", section);
    printf("%04X %04X %04X %04X "
           "%04X %04X %04X %04X\n\n",
           u16[7], u16[6], u16[5], u16[6], u16[3], u16[2], u16[1], u16[0]);
}

static void print_x32(__m128i value, const char *section)
{
    auto u32 = value.m128i_u32;
    printf("%s\n", section);
    printf("%08X %08X %08X %08X\n\n", u32[3], u32[2], u32[1], u32[0]);
}

static void print_x64(__m128i value, const char *section)
{
    auto u64 = value.m128i_u64;
    printf("%s\n", section);
    printf("%I64X %I64X\n\n", u64[1], u64[0]);
}

} // namespace sse
} // namespace sti
