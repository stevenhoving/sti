#pragma once

#include <sti/codecs/detail/png_structs.hpp>

namespace sti
{
namespace codecs
{
namespace detail
{

class png_write_structs : public png_structs
{
public:
    png_write_structs();
    ~png_write_structs();

    png_write_structs(png_write_structs &&o) = default;
    auto operator=(png_write_structs &&other) -> png_write_structs & = default;

    png_write_structs(png_write_structs const &) = delete;
    auto operator=(png_write_structs const &) -> png_write_structs & = delete;

private:
    void create_write_struct();
};

inline png_write_structs::png_write_structs()
    : png_structs()
{
    create_write_struct();

    // Create png info struct
    info_ptr_ = create_info_struct();
}

inline png_write_structs::~png_write_structs()
{
    png_destroy_write_struct(&png_ptr_, &info_ptr_);
}

inline void png_write_structs::create_write_struct()
{
    png_ptr_ = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    assert(png_ptr_);
}

} // namespace detail
} // namespace codecs
} // namespace sti
