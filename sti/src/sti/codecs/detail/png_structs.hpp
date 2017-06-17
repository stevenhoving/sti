#pragma once

#include <png.h>
#include <cassert>

namespace sti
{
namespace codecs
{
namespace detail
{

class png_structs
{
public:
    png_structs();

    png_structs(png_structs &&o) = default;
    auto operator=(png_structs &&other) -> png_structs & = default;

    png_structs(png_structs const &) = delete;
    auto operator=(png_structs const &) -> png_structs & = delete;

    auto png_ptr() const;
    auto info_ptr() const;
    auto end_info() const;

protected:
    ~png_structs();

    auto create_info_struct() const -> png_infop;

    png_structp png_ptr_;
    png_infop info_ptr_;
    png_infop end_info_;
};

inline png_structs::png_structs()
    : png_ptr_(nullptr)
    , info_ptr_(nullptr)
    , end_info_(nullptr)
{
}

inline png_structs::~png_structs()
{
    png_ptr_ = nullptr;
    info_ptr_ = nullptr;
    end_info_ = nullptr;
}

inline auto png_structs::png_ptr() const
{
    return png_ptr_;
}

inline auto png_structs::info_ptr() const
{
    return info_ptr_;
}

inline auto png_structs::end_info() const
{
    return end_info_;
}

inline auto png_structs::create_info_struct() const -> png_infop
{
    // Create png info struct
    auto info_ptr = png_create_info_struct(png_ptr_);
    assert(info_ptr);
    return info_ptr;
}

} // namespace detail
} // namespace codecs
} // namespace sti
