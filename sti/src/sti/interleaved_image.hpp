#pragma once

#include <sti/interleaved_image_info.hpp>
#include <cstdint>
#include <vector>

namespace sti
{

class interleaved_image final
{
public:
    using pixel_type_t = std::uint8_t;

    interleaved_image();
    explicit interleaved_image(const image_info &info);
    explicit interleaved_image(const image_info &info, std::vector<uint8_t> &&buffer);

    ~interleaved_image() = default;

    interleaved_image(interleaved_image &&other) = default;
    auto operator=(interleaved_image &&other) -> interleaved_image & = default;

    interleaved_image(interleaved_image const &) = delete;
    auto operator=(interleaved_image const &) -> interleaved_image & = delete;

    auto info() const -> const image_info &;

    auto size() const;

    auto data();
    auto data() const;

private:
    image_info info_;
    std::vector<pixel_type_t> buffer_;
};

inline interleaved_image::interleaved_image()
    : info_()
    , buffer_()
{
}

inline interleaved_image::interleaved_image(const image_info &info)
    : info_(info)
    , buffer_(info_.stride * info_.height * (info_.bits >> 3))
{
}

inline interleaved_image::interleaved_image(const image_info &info, std::vector<uint8_t> &&buffer)
    : info_(info)
    , buffer_(std::move(buffer))
{
}

inline auto interleaved_image::info() const -> const image_info &
{
    return info_;
}

inline auto interleaved_image::size() const
{
    return buffer_.size();
}

inline auto interleaved_image::data()
{
    return buffer_.data();
}

inline auto interleaved_image::data() const
{
    return buffer_.data();
}

} // namespace sti
