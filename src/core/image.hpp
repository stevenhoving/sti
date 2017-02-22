#pragma once

#include "detail/memory_file.hpp"
#include "detail/codec_jpeg_detail.hpp"
#include "detail/codec_bmp_detail.hpp"
#include "detail/image_color_detail.hpp"
#include "detail/image_detail.hpp"
#include "detail/image_resize_detail.hpp"

#include <cstdint>
#include <vector>
#include <string>

namespace sti
{
namespace core
{
template<typename T>
class image
{
    using value_type = std::vector<T>;
    using iterator_type = typename value_type::iterator;
    using const_iterator_type = typename value_type::const_iterator;
public:
    image()
        : image(0, 0, 0)
    {
    }

    explicit image(const int width, const int height, const int stride)
        : width_(width)
        , height_(height)
        , stride_(stride)
        , data_(stride * height, sti::detail::image::pixel_default_value<T>())
    {
    }

    explicit image(const int width, const int height)
        : image(width, height, width)
    {
    }

    explicit image(const uint8_t *data, const int width, const int height,
                   const int stride)
        : image(width, height, stride)
    {
        sti::detail::image::normalize_image(data, &data_[0], width_, height_);
    }

    explicit image(const uint8_t *data, const int width, const int height)
        : image(data, width, height, width)
    {
    }

    // how about the power to move you
    image(image&& other) = default;
    image & operator=(image &&other) = default;

    // we don't want to copy our image, we only want to move it or explicitly
    // clone it
    image(const image &) = delete;
    image& operator=(const image &) = delete;

    inline iterator_type begin()
    {
        return data_.begin();
    }

    inline iterator_type end()
    {
        return data_.end();
    }

    inline const_iterator_type begin() const
    {
        return data_.begin();
    }

    inline const_iterator_type end() const
    {
        return data_.end();
    }

    inline T *data() const
    {
        return const_cast<T *>(data_.data());
    }

    inline const int width() const
    {
        return width_;
    }

    inline const int height() const
    {
        return height_;
    }

    inline const int stride() const
    {
        return stride_;
    }

    // \todo check the input params
    inline T &at(const int x, const int y)
    {
        auto index = (y * stride_) + x;
        return data_[index];
    }

    // \todo check the input params
    inline const T at(const int x, const int y) const
    {
        auto index = (y * stride_) + x;
        return data_[index];
    }
private:
    int width_ = 0;
    int height_ = 0;
    int stride_ = 0;
    value_type data_;
};

template <typename T>
sti::core::image<T> read(const std::string &filename)
{
    sti::detail::memory_file file(filename);
    file.load();
    auto img = sti::detail::codec::jpeg::decompress_YUV(file);
    return sti::core::to_image<T>(img);
}

template <typename T>
void write(const sti::core::image<T> &src, const std::string &filename)
{
    //sti::detail::memory_file file(filename.c_str());
    //file.load();

    auto image = std::vector<uint8_t>(src.width() * src.height());
    sti::detail::image::denormalize_image<T>(src.data(), &image[0], src.width(),
        src.height());

    //for (int i = 0; i < src.width() * src.height(); ++i)
        // \todo clamp pixel first from 0.0 to 1.0 before multiply
        //image[i] = static_cast<uint8_t>(
            //src.data()[i] * static_cast<T>(255.0));

    //save_internal(image.data(), src.width(), src.height(), filename);

    sti::detail::codec::bmp::save(image.data(), src.width(), src.height(),
        filename.c_str());
    //sti::detail::codec::bmp::save(image.data(), src.width(), src.height(),
    //   filename.c_str());
}

template <typename T>
sti::core::image<T> to_image(sti::detail::image_color &src)
{
    if (src.stride() == src.width())
    {
        // nice we are aligned, and we can blindly create the sti image from
        // our y channel
        return sti::core::image<T>(src.data(), src.width(), src.height(),
            src.stride());
    }
    else
    {
        // fuck we need to 'fix' the alignment as our sti::image can not handle
        // it (yet)
        auto result = sti::core::image<T>(src.width(), src.height(),
            src.stride());

        auto data = src.data();
        auto dest = result.data();

        const auto height = src.height();
        const auto width = src.width();
        const auto width_aligned = src.stride();
        for (auto i = 0; i < height; ++i)
        {
            const auto src_index = i * width_aligned;
            const auto dst_index = i * width;

            const auto src_line = &data[src_index];
            const auto dst_line = &dest[dst_index];

            std::memmove(dst_line, src_line, width);
        }
        return result;
    }
}

} // namespace core

using image = core::image<uint8_t>;

template <typename T = uint8_t>
static
auto read_image(const std::string &filename)
{
    return sti::core::read<T>(filename);
}

template <typename T>
static
void write_image(const sti::core::image<T> &img, const std::string &filename)
{
    sti::core::write(img, filename);
}

template <typename T>
static
void resize_image(const sti::core::image<T> &src, sti::core::image<T> &dst)
{
    sti::detail::image_resize::resize_bilinear(
        src.data(), src.width(), src.height(),
        dst.data(), dst.width(), dst.height()
    );
}

template <typename T>
static
sti::core::image<T> resize_image_copy(const sti::core::image<T> &src, const int width, const int height)
{
    sti::core::image<T> dst(width, height);
    sti::detail::image_resize::resize_bilinear(
        src.data(), src.width(), src.height(),
        dst.data(), dst.width(), dst.height()
    );
    return dst;
}

} // namespace sti
