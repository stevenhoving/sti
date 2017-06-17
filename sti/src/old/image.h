#pragma once

#include "memory_view.h"

#include <cstdint>
#include <vector>

namespace sti
{

// image_normalize is a better name for this function
template <typename T>
void image_normalize(const uint8_t *src, T *dst, const int width, const int height)
{
    const T henk = static_cast<T>(1.0) / static_cast<T>(255.0);
    for (auto i = 0; i < width * height; ++i)
        dst[i] = src[i] * henk;
}

template <>
void image_normalize<uint8_t>(const uint8_t *src, uint8_t *dst, const int width, const int height)
{
    std::memmove(dst, src, width * height);
}

// currently only supporting grayscale
template <typename T>
class image
{
    using value_type = std::vector<T>;
    using iterator_type = typename value_type::const_iterator;

public:
    explicit image(const int width, const int height)
        : width_(width)
        , height_(height)
    {
        data_.resize(width * height);
    }

    explicit image(const uint8_t *data, const int width, const int height)
        : width_(width)
        , height_(height)
    {
        // \todo we miss a possible improvement here, because a resize like this will NULL/init the newly created
        // elements.
        data_.resize(width * height);
        image_normalize(data, &data_[0], width, height);
    }

    /* how about the power to move you*/
    image(image &&other) = default;
    image &operator=(image &&other) = default;

    /* we don't want to copy our image, we only want to move it or explicitly clone it */
    image(const image &) = delete;
    image &operator=(const image &) = delete;

    inline T *data() const
    {
        return const_cast<T *>(data_.data());
    }
    inline iterator_type begin() const
    {
        return data_.begin();
    }
    inline iterator_type end() const
    {
        return data_.end();
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
        return width_;
    } // \note atm the stride is equal to the width
private:
    int width_ = 0;
    int height_ = 0;
    value_type data_;
};

template <typename T>
T pixel_min()
{
    return static_cast<T>(0.0);
}
template <typename T>
T pixel_max()
{
    return static_cast<T>(1.0);
}
template <>
uint8_t pixel_min<uint8_t>()
{
    return 0;
}
template <>
uint8_t pixel_max<uint8_t>()
{
    return 0xFF;
}

} // namespace sti
