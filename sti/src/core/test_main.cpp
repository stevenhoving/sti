#include "image.hpp"
#include "histogram.hpp"

void test_image();
void test_histogram();
void test_mean_shift_filter();
void test_threshold_adaptive_bradley();

int main(int argc, char* argv[])
{
    //test_image();
    //test_histogram();
    //test_mean_shift_filter();
    test_threshold_adaptive_bradley();
    return 0;
}
