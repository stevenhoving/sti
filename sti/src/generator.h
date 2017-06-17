#pragma once

#include "image.h"

namespace sti
{
namespace generator
{

template <typename T>
static sti::image<T> create_checker_board(int width, int height, int rows, int cols)
{
    auto image = sti::image<uint8_t>(width, height);

    int row_width = image.width() / rows;
    int col_height = image.height() / cols;
    int data_size = image.width() * image.height();
    static int colors[] = {0, 0xFF};

    for (int i = 0; i < data_size; i += row_width)
    {
        uint8_t *chunk = &image.data()[i];

        int henk = (i / (width * col_height)) & 1;
        int banaan = (i / row_width) & 1;

        int color_index = banaan ^ henk;
        memset(chunk, colors[color_index], row_width);
    }

    return sti::image<T>(image.data(), width, height);
}

} // namespace generator
} // namespace sti
