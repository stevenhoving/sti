#pragma once

#include <vector>

namespace sti
{

template <typename pixel_type_t>
class slice final
{
public:
    using pixel_type = pixel_type_t;

    slice();
    explicit slice(const int width, const int height, const int stride);
    explicit slice(const int width, const int height, const int stride, std::vector<pixel_type_t> &&data);
    ~slice() = default;

    slice(slice &&other) = default;
    auto operator=(slice &&other) -> slice & = default;

    slice(slice const &) = default;
    auto operator=(slice const &) -> slice & = default;

    auto width() const;
    auto height() const;
    auto stride() const;

    auto data() -> pixel_type_t *;
    auto data() const -> const pixel_type_t *;

    auto begin() const;
    auto end() const;

    auto cbegin() const;
    auto cend() const;

private:
    int width_;
    int height_;
    int stride_;
    std::vector<pixel_type_t> data_;
};

template <typename pixel_type_t>
slice<pixel_type_t>::slice()
    : width_(0)
    , height_(0)
    , stride_(0)
    , data_()
{
}

template <typename pixel_type_t>
slice<pixel_type_t>::slice(const int width, const int height, const int stride)
    : width_(width)
    , height_(height)
    , stride_(stride)
    , data_(height * stride)
{
}

template <typename pixel_type_t>
slice<pixel_type_t>::slice(const int width, const int height, const int stride, std::vector<pixel_type_t> &&data)
    : width_(width)
    , height_(height)
    , stride_(stride)
    , data_(std::move(data))
{
}

template <typename pixel_type_t>
auto slice<pixel_type_t>::width() const
{
    return width_;
}

template <typename pixel_type_t>
auto slice<pixel_type_t>::height() const
{
    return height_;
}

template <typename pixel_type_t>
auto slice<pixel_type_t>::stride() const
{
    return stride_;
}

template <typename pixel_type_t>
auto slice<pixel_type_t>::data() -> pixel_type_t *
{
    return data_.data();
}

template <typename pixel_type_t>
auto slice<pixel_type_t>::data() const -> const pixel_type_t *
{
    return data_.data();
}

template <typename pixel_type_t>
auto slice<pixel_type_t>::begin() const
{
    return data_.begin();
}

template <typename pixel_type_t>
auto slice<pixel_type_t>::end() const
{
    return data_.end();
}

template <typename pixel_type_t>
auto slice<pixel_type_t>::cbegin() const
{
    return data_.cbegin();
}

template <typename pixel_type_t>
auto slice<pixel_type_t>::cend() const
{
    return data_.cend();
}

} // namespace sti
