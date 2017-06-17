#pragma once

#include "image.h"
#include "image_color.h"
#include "memory_file.h"

//#include <turbojpeg.h>
//#define _NJ_INCLUDE_HEADER_ONLY
//#include "dep/nanojpeg/nanojpeg.c"
#include "dep/ujpeg/ujpeg.h"

#include <exception>

namespace sti
{
namespace image_codec
{
namespace jpeg
{
class jpeg_decoder
{
public:
    jpeg_decoder()
    {
        //decompress_handle_ = ujCreate();
        
    }

    ~jpeg_decoder()
    {
        //ujDestroy(decompress_handle_);
    }

    sti::image_color decompress_YUV(memory_file &src)
    {
        uj.decode(src.data(), src.size());

        image_color::image_info info(uj.getWidth(), uj.getHeight(), 3, uj.getPlane(0)->stride, image_color::image_format::YUV420);
        image_color img(info);

        // \me crosses his fingers I allocated enought mem
        auto henk = uj.getPlane(0)->height * uj.getPlane(0)->stride;
        memcpy(img.data(), uj.getPlane(0)->pixels, henk);

        return img;
    }

    //ujImage decompress_handle_;
    uJPEG uj;
};


#if 0

class jpeg_decoder
{
public:
    jpeg_decoder()
        : decompress_handle_(0)
    {
        //__init();
        njInit();
    }

    ~jpeg_decoder()
    {
        //__deinit();
        njDone();
    }

    sti::image_color decompress_YUV(memory_file &src)
    {
        // fuck... just don't fail on me for now
        njDecode(src.data(), src.size());

        auto width = njGetWidth();
        auto height = njGetHeight();

        //int njGetWidth(void) { return nj.width; }
        //int njGetHeight(void) { return nj.height; }
        //int njIsColor(void) { return (nj.ncomp != 1); }
        //unsigned char* njGetImage(void) { return (nj.ncomp == 1) ? nj.comp[0].pixels : nj.rgb; }
        //int njGetImageSize(void) { return nj.width * nj.height * nj.ncomp; }

        njIsColor()




        image_color::image_info info(width, height, 3, 4, image_color::image_format::YUV420);

        image_color img(info);

        result = tjDecompressToYUV2(
            decompress_handle_,
            src.data(),
            src.size(),
            img.data(),
            width,
            info.padding,
            height,
            TJFLAG_ACCURATEDCT
            //TJFLAG_FASTDCT | TJFLAG_FASTUPSAMPLE
            );

        //Failed?
        if (result == -1)
        {
            __reinit();
            throw std::runtime_error("unable to decode jpeg file");
        }

        return img;
    }

#if 0
    bool decompress_YUV(image_buffer *source, int &width, int &height, image_buffer *destination)
    {
        if (!source || !destination)
            return false;

        //Decompress the jpeg header to get the size of the image
        int result = tjDecompressHeader(
            decompress_handle_,
            source->get(),
            source->get_info().size,
            &width,
            &height);

        if (result == -1)
        {
            __reinit();
            return false;
        }

        //Create the image info based on these sizes.
        image_buffer::buffer_info info(
            width,
            height,
            4,
            width * height * 4,
            image_buffer::image_format::YUV420);

        //Make sure we can fit this data
        destination->resize(info.size);

        //Is all still ok?
        if (!destination)
            return false;

        result = tjDecompressToYUV2(
            decompress_handle_,
            source->get(),
            source->get_info().size,
            destination->get(),
            width,
            4,
            height,
            TJFLAG_ACCURATEDCT
            //TJFLAG_FASTDCT | TJFLAG_FASTUPSAMPLE
            );

        //Failed?
        if (result == -1)
        {
            __reinit();
            return false;
        }

        //Update the info
        destination->set_info(info);

        return true;
    }
#endif
#if 0
    bool decompress_ARGB(image_buffer *source, int &width, int &height, image_buffer *destination)
    {
        if (!source || !destination)
            return false;

        if (!source->get() || !destination->get())
            return false;

        //Decompress the jpeg header to get the size of the image
        int result = tjDecompressHeader(
            decompress_handle_,
            source->get(),
            source->get_info().size,
            &width,
            &height);

        if (result == -1)
        {
            __reinit();
            return false;
        }

        //Create the image info based on these sizes.
        image_buffer::buffer_info info(
            width,
            height,
            4, // 32 bit (RGBX)
            width * height * 4,
            image_buffer::image_format::RGB);

        //Make sure we can fit this data
        destination->resize(info.size);

        //Is all still ok?
        if (!destination)
            return false;

        result = tjDecompress2(
            decompress_handle_,
            source->get(),
            source->get_info().size,
            destination->get(),
            width,
            0,
            height,
            TJPF_BGRA,
            TJFLAG_FASTDCT //TJFLAG_ACCURATEDCT
            );

        //Failed?
        if (result == -1)
        {
            __reinit();
            printf("tjDecompress2 failed\n");
            return false;
        }

        //Update the info
        destination->set_info(info);

        return true;
    }
#endif

    void __init()
    {
        decompress_handle_ = tjInitDecompress();
    }

    void __deinit()
    {
        if (decompress_handle_ == nullptr)
            return;

        tjDestroy(decompress_handle_);
        decompress_handle_ = nullptr;
    }

    void __reinit()
    {
        __deinit();
        __init();
    }

private:
    tjhandle decompress_handle_;
};

#endif

template <typename T>
sti::image<T> load(const char *filename)
{
    sti::memory_file file(filename);
    file.load();

    jpeg_decoder decoder;
    auto img = decoder.decompress_YUV(file);
    return img.to_image<T>();
}

} // namespace jpeg
} // namespace image_codec
} // namespace sti
