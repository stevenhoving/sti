#pragma once

#include <cstdint>
#include <vector>

namespace sti
{
namespace detail
{
/* \note hacky color image class so we can extract a single channel (yuv) from
 * a jpeg.
 */
class image_color
{
public:
    enum class image_format
    {
        BGR,
        RGB,
        YUV420,
        invalid
    };

    /* class that keeps track of the image info */
    struct image_info
    {
        image_info() = default;
        image_info(const image_format _format)
            : image_info(0, 0, 0, 0, _format)
        {
        }

        // _bpp -> bytes per pixel
        image_info(int _width, int _height, int _bpp, int _stride, const image_format _format)
            : width(_width)
            , height(_height)
            , stride(_stride)
            , bpp(_bpp)
            , format(_format)
        {
        }

        int width = 0;
        int height = 0;
        int stride = 0;
        int bpp = 0; // bytes per pixel!!!!
        image_format format = image_format::invalid;
    };

    explicit image_color(const image_info &info)
        : info_(info)
        , buffer_(info_.stride * info_.height * info_.bpp)
    {
    }

    inline const image_info &info() const
    {
        return info_;
    }

    inline size_t size()
    {
        return buffer_.size();
    }
    inline uint8_t *data()
    {
        return const_cast<uint8_t *>(buffer_.data());
    }

    /* how about the power to move you*/
    image_color(image_color &&other) = default;
    image_color &operator=(image_color &&other) = default;
    /* we need no copy stuff */
    image_color(image_color const &) = delete;
    image_color &operator=(image_color const &) = delete;

    inline int width() const
    {
        return info_.width;
    }
    inline int height() const
    {
        return info_.height;
    }
    inline int stride() const
    {
        return info_.stride;
    }

private:
    image_info info_;
    std::vector<uint8_t> buffer_;
};

} // namespace detail
} // namespace sti
