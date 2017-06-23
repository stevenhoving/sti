#pragma once

#include <vector>

namespace sti
{

template <typename pixel_type_t>
class plane final
{
public:
    using pixel_type = pixel_type_t;

    plane();
    explicit plane(const int width, const int height);
    explicit plane(const int width, const int height, const int stride);
    explicit plane(const int width, const int height, const int stride, std::vector<pixel_type_t> &&data);
    ~plane() = default;

    plane(plane &&other) = default;
    auto operator=(plane &&other) -> plane & = default;

    plane(plane const &) = default;
    auto operator=(plane const &) -> plane & = default;

    auto width() const;
    auto height() const;
    auto stride() const;

    auto data() -> pixel_type_t *;
    auto data() const -> const pixel_type_t *;

    auto begin() const;
    auto end() const;

    auto cbegin() const;
    auto cend() const;

    auto operator[](const std::size_t offset) -> pixel_type_t &;
    auto operator[](const std::size_t offset) const -> const pixel_type_t &;

private:
    int width_;
    int height_;
    int stride_;
    std::vector<pixel_type_t> data_;
};

template <typename pixel_type_t>
plane<pixel_type_t>::plane()
    : width_(0)
    , height_(0)
    , stride_(0)
    , data_()
{
}

template <typename pixel_type_t>
plane<pixel_type_t>::plane(const int width, const int height)
    : plane<pixel_type_t>(width, height, width)
{
}

template <typename pixel_type_t>
plane<pixel_type_t>::plane(const int width, const int height, const int stride)
    : width_(width)
    , height_(height)
    , stride_(stride)
    , data_(height * stride)
{
}

template <typename pixel_type_t>
plane<pixel_type_t>::plane(const int width, const int height, const int stride, std::vector<pixel_type_t> &&data)
    : width_(width)
    , height_(height)
    , stride_(stride)
    , data_(std::move(data))
{
}

template <typename pixel_type_t>
auto plane<pixel_type_t>::width() const
{
    return width_;
}

template <typename pixel_type_t>
auto plane<pixel_type_t>::height() const
{
    return height_;
}

template <typename pixel_type_t>
auto plane<pixel_type_t>::stride() const
{
    return stride_;
}

template <typename pixel_type_t>
auto plane<pixel_type_t>::data() -> pixel_type_t *
{
    return data_.data();
}

template <typename pixel_type_t>
auto plane<pixel_type_t>::data() const -> const pixel_type_t *
{
    return data_.data();
}

template <typename pixel_type_t>
auto plane<pixel_type_t>::begin() const
{
    return data_.begin();
}

template <typename pixel_type_t>
auto plane<pixel_type_t>::end() const
{
    return data_.end();
}

template <typename pixel_type_t>
auto plane<pixel_type_t>::cbegin() const
{
    return data_.cbegin();
}

template <typename pixel_type_t>
auto plane<pixel_type_t>::cend() const
{
    return data_.cend();
}

template <typename pixel_type_t>
auto plane<pixel_type_t>::operator[](const std::size_t offset) -> pixel_type_t &
{
    return data_.at(offset);
}

template <typename pixel_type_t>
auto plane<pixel_type_t>::operator[](const std::size_t offset) const -> const pixel_type_t &
{
    return data_.at(offset);
}

} // namespace sti
