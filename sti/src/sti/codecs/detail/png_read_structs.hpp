#pragma once

#include <sti/codecs/detail/png_structs.hpp>

namespace sti
{
namespace codecs
{
namespace detail
{

class png_read_structs : public png_structs
{
public:
    png_read_structs();
    ~png_read_structs();

    png_read_structs(png_read_structs &&o) = default;
    auto operator=(png_read_structs &&other) -> png_read_structs & = default;

    png_read_structs(png_read_structs const &) = delete;
    auto operator=(png_read_structs const &) -> png_read_structs & = delete;

private:
    void create_read_struct();
};

inline png_read_structs::png_read_structs()
    : png_structs()
{
    create_read_struct();

    // Create png info struct
    info_ptr_ = create_info_struct();

    // Create png end info struct
    end_info_ = create_info_struct();
}

inline png_read_structs::~png_read_structs()
{
    png_destroy_read_struct(&png_ptr_, &info_ptr_, &end_info_);
}

inline void png_read_structs::create_read_struct()
{
    png_ptr_ = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    assert(png_ptr_);
}

} // namespace detail
} // namespace codecs
} // namespace sti
