#pragma once

#include <sti/codecs/codec_png.h>
#include <sti/interleaved_image.hpp>
#include <aeon/streams/file_stream.h>
#include <build_config.h>

#include <gtest/gtest.h>

struct load_file_fixture : public ::testing::Test
{
    load_file_fixture()
        : loaded_image()
    {
    }

    void SetUp() override
    {
        auto stream = aeon::streams::file_stream(STI_TEST_DATA_PATH "/DSC_7000.png");
        ASSERT_NO_THROW(loaded_image = sti::codecs::png::decode(stream));
    }

    void write_result(const std::string &filename, const sti::interleaved_image &interleaved_image) const
    {
        auto output_stream = aeon::streams::file_stream(
            filename, aeon::streams::access_mode::write | aeon::streams::access_mode::truncate);
        sti::codecs::png::encode(interleaved_image, output_stream);
    }

    sti::interleaved_image loaded_image;
};
