#pragma once

#include <sti/slice.hpp>
#include <array>

namespace sti
{

template <typename pixel_type_t, int slice_count_t>
class image final
{
public:
    using pixel_type = pixel_type_t;
    using slice_type = slice<pixel_type>;

    image();
    image(const int width, const int height);
    image(const int width, const int height, const int stride);
    image(const int width, const int height, const int stride, std::array<slice_type, slice_count_t> &&slices);
    ~image() = default;

    image(image &&other) = default;
    auto operator=(image &&other) -> image & = default;

    image(image const &) = delete;
    auto operator=(image const &) -> image & = delete;

    auto width() const;
    auto height() const;
    auto stride() const;
    auto slice_count() const;
    auto get_slice(const int slice) -> slice_type &;
    auto get_slice(const int slice) const -> const slice_type &;

private:
    int width_;
    int height_;
    int stride_;
    std::array<slice_type, slice_count_t> slices_;
};

template <typename pixel_type_t, int slice_count_t>
image<pixel_type_t, slice_count_t>::image()
    : width_(0)
    , height_(0)
    , stride_(0)
{
}

template <typename pixel_type_t, int slice_count_t>
image<pixel_type_t, slice_count_t>::image(const int width, const int height)
    : image(width, height, width)
{
}

template <typename pixel_type_t, int slice_count_t>
image<pixel_type_t, slice_count_t>::image(const int width, const int height, const int stride)
    : width_(width)
    , height_(height)
    , stride_(stride)
    , slices_()
{
    for (auto i = 0; i < slice_count_t; ++i)
        slices_[i] = slice<pixel_type_t>(height_, stride_);
}

template <typename pixel_type_t, int slice_count_t>
image<pixel_type_t, slice_count_t>::image(const int width, const int height, const int stride,
                                          std::array<slice_type, slice_count_t> &&slices)
    : width_(width)
    , height_(height)
    , stride_(width)
    , slices_(std::move(slices))
{
}

template <typename pixel_type_t, int slice_count_t>
auto image<pixel_type_t, slice_count_t>::width() const
{
    return width_;
}

template <typename pixel_type_t, int slice_count_t>
auto image<pixel_type_t, slice_count_t>::height() const
{
    return height_;
}

template <typename pixel_type_t, int slice_count_t>
auto image<pixel_type_t, slice_count_t>::stride() const
{
    return stride_;
}

template <typename pixel_type_t, int slice_count_t>
auto image<pixel_type_t, slice_count_t>::slice_count() const
{
    return slice_count_t;
}

template <typename pixel_type_t, int slice_count_t>
auto image<pixel_type_t, slice_count_t>::get_slice(const int slice) -> slice_type &
{
    return slices_[slice];
}

template <typename pixel_type_t, int slice_count_t>
auto image<pixel_type_t, slice_count_t>::get_slice(const int slice) const -> const slice_type &
{
    return slices_[slice];
}

} // namespace sti
