#pragma once

#include <sti/codecs/codec_png.h>
#include <sti/interleaved_image.hpp>
#include <aeon/streams/file_stream.h>
#include <build_config.h>

#include <gtest/gtest.h>

struct write_file_fixture : ::testing::Test
{
    void write_result(const std::string &filename, const sti::interleaved_image &interleaved_image) const
    {
        auto output_stream = aeon::streams::file_stream(
            filename, aeon::streams::access_mode::write | aeon::streams::access_mode::truncate);
        sti::codecs::png::encode(interleaved_image, output_stream);
    }
};
