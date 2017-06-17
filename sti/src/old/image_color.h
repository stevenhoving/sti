#pragma once

#include "memory_view.h"
#include "image.h"

#include <cstdint>
#include <vector>

namespace
{
// align up shizzle
#define IMG_WIDTH_ALIGN(size, align) ((size + (align - 1)) & ~(align - 1))
}

static_assert(IMG_WIDTH_ALIGN(130, 4) == 132, "");

namespace sti
{

class image_color
{
public:
    enum class image_format
    {
        BGR,
        JPEG,
        RGB,
        YUV420,
        invalid
    };

    /* class that keeps track of the image info */
    struct image_info
    {
        image_info()
            : image_info(0, 0, 0, 0, image_format::invalid) // C++11 delegating constructors :D
        {
        }

        image_info(image_format _format)
            : image_info(0, 0, 0, 0, _format) // C++11 delegating constructors :D
        {
        }

        // _bpp -> bytes per pixel
        image_info(int _width, int _height, int _bpp, int _stride, image_format _format)
            : width(_width)
            , height(_height)
            , bpp(_bpp)
            , stride(_stride)
            , size(_stride * height * bpp)
            , format(_format)
        {
        }

        int width;
        int height;
        int bpp;
        int stride;
        int size;

        image_format format = image_format::invalid;
    };

    image_color(image_info &info)
        : info_(info)
    {
        resize(info_.size);
    }

    ~image_color()
    {
    }

    image_info &info()
    {
        return info_;
    }

    void resize(int size)
    {
        buffer_.resize(size);
    }

    size_t size()
    {
        return buffer_.size();
    }
    uint8_t *data()
    {
        return buffer_.data();
    }

    /* how about the power to move you*/
    image_color(image_color &&other)
        : info_(std::move(other.info_))
        , buffer_(std::move(other.buffer_))
    {
    }

    image_color &operator=(image_color &&other)
    {
        info_ = std::move(other.info_);
        buffer_ = std::move(other.buffer_);
        return *this;
    }

    const uint8_t *y() const
    {
        return buffer_.data();
    }
    long y_size() const
    {
        return info_.stride * info_.height;
    }

    int width() const
    {
        return info_.width;
    }
    int height() const
    {
        return info_.height;
    }

    template <typename T>
    sti::image<T> to_image()
    {
        if (info_.stride == info_.width)
        {
            // nice we are aligned, and we can blindly create the sti image from our y channel
            return sti::image<T>(y(), width(), height());
        }
        else
        {
            // fuck we need to 'fix' the alignment as our sti::image can not handle it (yet)
            auto img = sti::image<T>(width(), height());
            auto width_aligned = info_.stride;
            for (int i = 0; i < height(); ++i)
            {
                auto src_index = i * width_aligned;
                auto dst_index = i * width();

                auto src_line = &data()[src_index];
                auto dst_line = &img.data()[dst_index];

                memcpy(dst_line, src_line, width());
            }
            return img;
        }
    }

    /* we need no copy stuff */
    image_color(image_color const &) = delete;
    image_color &operator=(image_color const &) = delete;

private:
    image_info info_;
    std::vector<uint8_t> buffer_;
};

} // namespace sti
