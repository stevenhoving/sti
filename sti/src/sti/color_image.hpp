#pragma once

#include <sti/color_image_info.hpp>
#include <cstdint>
#include <vector>

namespace sti
{

class color_image
{
public:
    explicit color_image(const image_info &info);

    ~color_image() = default;

    color_image(color_image &&other) = default;
    color_image &operator=(color_image &&other) = default;

    color_image(color_image const &) = delete;
    color_image &operator=(color_image const &) = delete;

    auto info() const -> const image_info &;
    auto size() const;
    auto data();
    auto width() const;
    auto height() const;
    auto stride() const;

private:
    image_info info_;
    std::vector<uint8_t> buffer_;
};

inline color_image::color_image(const image_info &info)
    : info_(info)
    , buffer_(info_.stride * info_.height * info_.bytes_per_pixel)
{
}

inline auto color_image::info() const -> const image_info &
{
    return info_;
}

inline auto color_image::size() const
{
    return buffer_.size();
}

inline auto color_image::data()
{
    return buffer_.data();
}

inline auto color_image::width() const
{
    return info_.width;
}

inline auto color_image::height() const
{
    return info_.height;
}

inline auto color_image::stride() const
{
    return info_.stride;
}

} // namespace sti
