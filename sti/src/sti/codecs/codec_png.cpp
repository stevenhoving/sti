#include <sti/codecs/codec_png.h>
#include <sti/codecs/detail/png_read_structs.hpp>
#include <sti/codecs/detail/png_write_structs.hpp>
#include <sti/interleaved_image.hpp>
#include <aeon/common/compilers.h>
#include <png.h>
#include <array>
#include <cassert>

#define PNG_HEADER_SIGNATURE_SIZE 8

namespace sti
{
namespace codecs
{

struct png_pixel_info
{
    int bytes_per_pixel = 0;
    interleaved_image_format format = interleaved_image_format::invalid;
};

static void __png_read_callback(png_structp png_ptr, png_bytep output_ptr, png_size_t output_size)
{
    auto stream = static_cast<aeon::streams::stream *>(png_get_io_ptr(png_ptr));
    assert(stream);
    stream->read(output_ptr, static_cast<size_t>(output_size));
}

static void __png_write_callback(png_structp png_ptr, png_bytep data, png_size_t length)
{
    auto stream = static_cast<aeon::streams::stream *>(png_get_io_ptr(png_ptr));
    assert(stream);
    stream->write(data, length);
}

static auto __get_image_info(const int width, const int height, const int bit_depth, const int color_type) -> image_info
{
    image_info info;

    int bytes_per_pixel = 0;
    switch (color_type)
    {
        case PNG_COLOR_TYPE_RGB:
            info.format = interleaved_image_format::RGB;
            bytes_per_pixel = 3;
            break;
        case PNG_COLOR_TYPE_RGB_ALPHA:
            info.format = interleaved_image_format::RGBA;
            bytes_per_pixel = 4;
            break;
        default:
            throw std::runtime_error("Invalid or unknown pixel format.");
    }

    bytes_per_pixel *= (bit_depth >> 3);

    info.bits = bit_depth;
    info.width = width;
    info.height = height;
    info.stride = width * bytes_per_pixel;

    return info;
}

static auto __get_png_pixel_format(const image_info &info)
{
    switch (info.format)
    {
        case interleaved_image_format::RGB:
            return PNG_COLOR_TYPE_RGB;
        case interleaved_image_format::RGBA:
            return PNG_COLOR_TYPE_RGB_ALPHA;
        case interleaved_image_format::BGR:
        case interleaved_image_format::YUV420:
        case interleaved_image_format::invalid:
        default:
            throw std::runtime_error("Unsupported image format.");
    }
}

auto png::decode(aeon::streams::stream &stream) -> interleaved_image
{
    // Read the header
    auto png_header = std::array<png_byte, PNG_HEADER_SIGNATURE_SIZE>();
    stream.read(png_header.data(), PNG_HEADER_SIGNATURE_SIZE);

    // Check the header
    if (png_sig_cmp(png_header.data(), 0, PNG_HEADER_SIGNATURE_SIZE))
        throw std::runtime_error("Could not decode png image. Invalid signature.");

    auto png_structs = detail::png_read_structs();

    // Bind errors from libpng
    AEON_IGNORE_VS_WARNING_PUSH(4611)
    if (setjmp(png_jmpbuf(png_structs.png_ptr())))
        throw std::runtime_error("Could not decode png image. Error reported by libpng while decoding.");
    AEON_IGNORE_VS_WARNING_POP()

    // Init png reading. We will be using a read function, as we can't read
    // from a file.
    png_set_read_fn(png_structs.png_ptr(), &stream, __png_read_callback);

    // Let libpng know we already read the signature
    png_set_sig_bytes(png_structs.png_ptr(), PNG_HEADER_SIGNATURE_SIZE);

    // Read all the info up to the image data
    png_read_info(png_structs.png_ptr(), png_structs.info_ptr());

    // Variables to pass to get info
    auto bit_depth = 0;
    auto color_type = 0;
    auto temp_width = png_uint_32();
    auto temp_height = png_uint_32();

    // Get info about png
    png_get_IHDR(png_structs.png_ptr(), png_structs.info_ptr(), &temp_width, &temp_height, &bit_depth, &color_type,
                 nullptr, nullptr, nullptr);

    // Update the png info struct.
    png_read_update_info(png_structs.png_ptr(), png_structs.info_ptr());

    auto rowbytes = png_get_rowbytes(png_structs.png_ptr(), png_structs.info_ptr());

    // Allocate the image_data as a big block
    auto bitmap_buff_size = rowbytes * temp_height * sizeof(png_byte);
    auto bitmap_buffer = std::vector<std::uint8_t>(bitmap_buff_size);

    // Cast to png_byte since this is what libpng likes as buffer.
    // Just passing the pointer should be fine. But this ensures 100% compatibility.
    auto image_data = static_cast<png_byte *>(bitmap_buffer.data());

    // Row_pointers is for pointing to image_data for reading the png with libpng
    auto rowpointer_buffer = std::vector<png_bytep>(temp_height);
    auto row_pointers = static_cast<png_bytep *>(rowpointer_buffer.data());

    // Set the individual row_pointers to point at the correct offsets
    // of image_data
    for (auto i = 0ul; i < temp_height; ++i)
    {
        row_pointers[i] = image_data + i * rowbytes;
    }

    // Read the png into image_data through row_pointers
    png_read_image(png_structs.png_ptr(), row_pointers);

    return interleaved_image(__get_image_info(temp_width, temp_height, bit_depth, color_type),
                             std::move(bitmap_buffer));
}

void png::encode(const interleaved_image &image, aeon::streams::stream &stream)
{
    auto png_structs = detail::png_write_structs();

    // Bind errors from libpng
    AEON_IGNORE_VS_WARNING_PUSH(4611)
    if (setjmp(png_jmpbuf(png_structs.png_ptr())))
        throw std::runtime_error("Could not encode png image. Error reported by libpng while encoding.");
    AEON_IGNORE_VS_WARNING_POP()

    const auto &image_info = image.info();
    const auto color_type = __get_png_pixel_format(image_info);
    const auto width = static_cast<png_uint_32>(image_info.width);
    const auto height = static_cast<png_uint_32>(image_info.height);
    const auto bit_depth = image_info.bits;

    png_set_IHDR(png_structs.png_ptr(), png_structs.info_ptr(), width, height, bit_depth, color_type,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    auto rowpointer_buffer = std::vector<const std::uint8_t *>(height);
    auto rowpointer_buffer_ptr = rowpointer_buffer.data();
    auto pixel_data_ptr = image.data();

    for (auto y = 0u; y < height; ++y)
        rowpointer_buffer_ptr[y] = pixel_data_ptr + y * image_info.stride;

    png_set_rows(png_structs.png_ptr(), png_structs.info_ptr(), const_cast<png_bytepp>(rowpointer_buffer_ptr));
    png_set_write_fn(png_structs.png_ptr(), &stream, __png_write_callback, nullptr);
    png_write_png(png_structs.png_ptr(), png_structs.info_ptr(), PNG_TRANSFORM_IDENTITY, nullptr);

    stream.flush();
}

} // namespace codecs
} // namespace sti
