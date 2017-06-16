#pragma once

#include "image_color_detail.hpp"
#include "memory_file.hpp"
#include "utility_detail.hpp"

#include <cstdint>

namespace
{

// \todo make this non visual studio
#pragma pack(push, 2)
struct _BITMAPFILEHEADER
{
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
};
#pragma pack(pop)

// \todo make this non visual studio
#pragma pack(push, 2)
struct _BITMAPINFOHEADER
{
    uint32_t biSize;
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};
#pragma pack(pop)
}

namespace sti
{
namespace detail
{
namespace codec
{
namespace bmp
{

/*
sti::image load(const char *filename)
{

}
*/

void save_internal(uint8_t *src, int width, int height, const char *filename);

/*template<typename T>
void save(sti::core::image<T> &src, const char *filename)
{
    auto image = std::vector<uint8_t>(src.width() * src.height());
    for (int i = 0; i < src.width() * src.height(); ++i)
        // \todo clamp pixel first from 0.0 to 1.0 before multiply
        image[i] = static_cast<uint8_t>(src.data()[i] * static_cast<T>(255.0));

    save_internal(image.data(), src.width(), src.height(), filename);
}


template<>
void save(sti::image<uint8_t> &src, const char *filename)
{
    save_internal(src.data(), src.width(), src.height(), filename);
}
*/

static
void save(const uint8_t *src, const int width, const int height, const char *filename)
{
    auto width_padd = align_up(width, 4);

    _BITMAPFILEHEADER hdr;
    memset(&hdr, 0xff, sizeof(_BITMAPFILEHEADER));
    hdr.bfType = ('B' | 'M' << 8);
    hdr.bfOffBits = sizeof(_BITMAPFILEHEADER) + sizeof(_BITMAPINFOHEADER);
    hdr.bfSize = hdr.bfOffBits + width_padd * height; // multiply by 3 if you wanna switch to RGB

    _BITMAPINFOHEADER info_hdr;
    memset(&info_hdr, 0, sizeof(_BITMAPINFOHEADER));
    info_hdr.biSize = sizeof(_BITMAPINFOHEADER);
    info_hdr.biWidth = width;
    info_hdr.biHeight = height;
    info_hdr.biPlanes = 1;
    info_hdr.biBitCount = 8; // this means grayscale, change to 24 if you wanna switch to RGB

    FILE *filePtr = fopen(filename, "wb");
    if (filePtr == NULL)
        return;

    fwrite(&hdr, 1, sizeof(_BITMAPFILEHEADER), filePtr);

    /* write the bitmap info header */
    fwrite(&info_hdr, 1, sizeof(_BITMAPINFOHEADER), filePtr);

    // bitmaps grayscale must have a table of colors... don't write this table if you want RGB
    uint8_t grayscale[4];

    for (int i = 0; i < 0x100; ++i)
    {
        memset(grayscale, i, sizeof(grayscale));
        fwrite(grayscale, 1, sizeof(grayscale), filePtr);
    }

    // here we record the pixels... I created a gradient...
    // remember that the pixels ordem is from left to right, "bottom to top"...
    uint8_t pixel; // do pixel[ 3 ] if you want RGB

    const auto imgsize = width * height;

    for (auto y = 0; y < height; ++y)
    {
        // + ( p_width % 4 ? ( 4 - p_width % 4 ) : 0 ) because BMP has a padding of 4 bytes per line
        for (auto x = 0; x < width_padd; ++x)
        {
            if (x > width) /* check if we need to write padding... */
            {
                pixel = 0;
                fwrite(&pixel, 1, 1, filePtr);
                //uint8_t pixel4[4] = {0};
                //fwrite(&pixel4, 2, 1, filePtr);
            }
            else
            {
                auto index = imgsize - ((y * width) + (width - x));
                pixel = src[index];
                fwrite(&pixel, 1, 1, filePtr);
            }
        }
    }

    fclose(filePtr);
}


} // namespace bmp
} // namespace codec
} // namespace detail
} // namespace sti
