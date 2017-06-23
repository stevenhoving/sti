#pragma once

#include <sti/plane.hpp>
#include <array>
#include <vector>

namespace sti
{

template <typename pixel_type>
class image final
{
public:
    using pixel_type_t = pixel_type;
    using plane_type_t = plane<pixel_type_t>;

    image();
    image(const int width, const int height, const int plane_count);
    image(const int width, const int height, const int stride, const int plane_count);
    image(const int width, const int height, const int stride, std::vector<plane_type_t> &&planes);
    ~image() = default;

    image(image &&other) = default;
    auto operator=(image &&other) -> image & = default;

    image(image const &) = delete;
    auto operator=(image const &) -> image & = delete;

    auto width() const;
    auto height() const;
    auto stride() const;
    auto plane_count() const;
    auto get_plane(const int index) -> plane_type_t &;
    auto get_plane(const int index) const -> const plane_type_t &;
    auto get_planes() -> std::vector<plane_type_t> &;
    auto get_planes() const -> const std::vector<plane_type_t> &;

    void remove_plane(const int index);

    template <int... indices>
    void swap_planes();

    template <int... indices>
    void rearrange_planes();

private:
    int width_;
    int height_;
    int stride_;
    std::vector<plane_type_t> planes_;
};

template <typename pixel_type_t>
image<pixel_type_t>::image()
    : width_(0)
    , height_(0)
    , stride_(0)
    , planes_()
{
}

template <typename pixel_type_t>
image<pixel_type_t>::image(const int width, const int height, const int plane_count)
    : image(width, height, width, plane_count)
{
}

template <typename pixel_type_t>
image<pixel_type_t>::image(const int width, const int height, const int stride, const int plane_count)
    : width_(width)
    , height_(height)
    , stride_(stride)
    , planes_(plane_count, plane<pixel_type_t>(width_, height_, stride_))
{
}

template <typename pixel_type_t>
image<pixel_type_t>::image(const int width, const int height, const int stride, std::vector<plane_type_t> &&planes)
    : width_(width)
    , height_(height)
    , stride_(stride)
    , planes_(std::move(planes))
{
}

template <typename pixel_type_t>
auto image<pixel_type_t>::width() const
{
    return width_;
}

template <typename pixel_type_t>
auto image<pixel_type_t>::height() const
{
    return height_;
}

template <typename pixel_type_t>
auto image<pixel_type_t>::stride() const
{
    return stride_;
}

template <typename pixel_type_t>
auto image<pixel_type_t>::plane_count() const
{
    return static_cast<int>(planes_.size());
}

template <typename pixel_type_t>
auto image<pixel_type_t>::get_plane(const int index) -> plane_type_t &
{
    return planes_.at(index);
}

template <typename pixel_type_t>
auto image<pixel_type_t>::get_plane(const int index) const -> const plane_type_t &
{
    return planes_.at(index);
}

template <typename pixel_type_t>
auto image<pixel_type_t>::get_planes() -> std::vector<plane_type_t> &
{
    return planes_;
}

template <typename pixel_type_t>
auto image<pixel_type_t>::get_planes() const -> const std::vector<plane_type_t> &
{
    return planes_;
}

template <typename pixel_type>
void image<pixel_type>::remove_plane(const int index)
{
    std::vector<plane_type_t> planes;

    for (auto i = 0; i < planes_.size(); ++i)
    {
        if (i != index)
            planes.emplace_back(std::move(planes_.at(i)));
    }

    planes_ = std::move(planes);
}

template <typename pixel_type>
template <int... indices>
void image<pixel_type>::rearrange_planes()
{
    constexpr auto new_planes_count = sizeof...(indices);

    std::vector<plane_type_t> planes;

    for (auto i : {indices...})
    {
        planes.push_back(planes_.at(i));
    }

    planes_ = std::move(planes);
}

template <typename pixel_type>
template <int... indices>
void image<pixel_type>::swap_planes()
{
    constexpr auto new_planes_count = sizeof...(indices);

    std::vector<plane_type_t> planes;

    for (auto i : {indices...})
    {
        planes.emplace_back(std::move(planes_.at(i)));
    }

    planes_ = std::move(planes);
}

} // namespace sti
