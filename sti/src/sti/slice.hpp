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
    explicit slice(const int height, const int stride);
    explicit slice(std::vector<pixel_type_t> &&data);
    ~slice() = default;

    slice(slice &&other) = default;
    auto operator=(slice &&other) -> slice & = default;

    slice(slice const &) = default;
    auto operator=(slice const &) -> slice & = default;

    auto data() -> pixel_type_t *;
    auto data() const -> const pixel_type_t *;

    auto begin() const;
    auto end() const;

    auto cbegin() const;
    auto cend() const;

private:
    std::vector<pixel_type_t> data_;
};

template <typename pixel_type_t>
slice<pixel_type_t>::slice()
    : data_()
{
}

template <typename pixel_type_t>
slice<pixel_type_t>::slice(const int height, const int stride)
    : data_(height * stride)
{
}

template <typename pixel_type_t>
slice<pixel_type_t>::slice(std::vector<pixel_type_t> &&data)
    : data_(std::move(data))
{
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
