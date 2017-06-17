#include "image.hpp"
#include "histogram.hpp"
#include "core/filter/filter_range_compression_sse.hpp"

void test_image();
void test_histogram();
void test_mean_shift_filter();
void test_threshold_adaptive_bradley();
void test_filter_range_compression_c();
void test_filter_range_compression_sse();
void test_filter_mean_shift_filter2();

int main(int argc, char *argv[])
{
    // test_image();
    // test_histogram();

    // test_threshold_adaptive_bradley();
    // test_filter_range_compression_c();
    // test_filter_range_compression_sse();
    // test_mean_shift_filter();
    test_filter_mean_shift_filter2();
    return 0;
}
