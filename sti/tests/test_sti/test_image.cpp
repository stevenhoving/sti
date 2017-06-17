#include <gtest/gtest.h>

#include <sti/image.hpp>

TEST(test_image, default_stride_value)
{
    auto image = sti::image<float, 3>(10, 10);
    EXPECT_EQ(10, image.width());
    EXPECT_EQ(10, image.width());
    EXPECT_EQ(image.width(), image.stride());
    EXPECT_EQ(3, image.slice_count());
}
