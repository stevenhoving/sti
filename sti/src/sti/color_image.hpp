#pragma once

#include <sti/color_image_info.hpp>
#include <cstdint>
#include <vector>

namespace sti
{

class color_image final
{
public:
    color_image();
    explicit color_image(const image_info &info);
    explicit color_image(const image_info &info, std::vector<uint8_t> &&buffer);

    ~color_image() = default;

    color_image(color_image &&other) = default;
    auto operator=(color_image &&other) -> color_image & = default;

    color_image(color_image const &) = delete;
    auto operator=(color_image const &) -> color_image & = delete;

    auto info() const -> const image_info &;

    auto size() const;

    auto data();
    auto data() const;

private:
    image_info info_;
    std::vector<uint8_t> buffer_;
};

inline color_image::color_image()
    : info_()
    , buffer_()
{
}

inline color_image::color_image(const image_info &info)
    : info_(info)
    , buffer_(info_.stride * info_.height * (info_.bits >> 3))
{
}

inline color_image::color_image(const image_info &info, std::vector<uint8_t> &&buffer)
    : info_(info)
    , buffer_(std::move(buffer))
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

inline auto color_image::data() const
{
    return buffer_.data();
}

} // namespace sti
