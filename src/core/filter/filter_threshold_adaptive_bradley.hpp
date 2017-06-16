#pragma once

#include <cstdint>
#include <vector>

namespace sti {
namespace core {

/* promote/demote any integral type to int, for the rest... just use T
 *
 * char and short will get promoted to int and float/double will just pass through.
 * \todo will fail when we use 64 bit pixels.
 */
template<typename T>
using bradley_intergral_type = typename std::conditional<std::is_integral<T>::value, int, T>::type;

template<typename T>
using bradley_intergral_vector_type = std::vector<bradley_intergral_type<T>>;

//https://github.com/rmtheis/bradley-adaptive-thresholding/raw/master/gerh-50002.pdf
// \todo parameterize this function: 'block size' and 'threshold'. But to be sure... read the paper.
template <typename T>
void filter_threshold_adaptive_bradley(const sti::core::image<T> &src, sti::core::image<T> &dst, int size, float threshold)
{
    /* create integral image (aka summed-area table) */
    auto intergral_img = bradley_intergral_vector_type<T>();
    intergral_img.resize(src.width() * src.height() * 4);

    for (int x = 0; x < src.width(); ++x)
    {
        auto sum = bradley_intergral_type<T>();
        for (int y = 0; y < src.height(); ++y)
        {
            int index = y * src.width() + x;

            sum += src.data()[index];
            if (x == 0)
                intergral_img[index] = sum;
            else
                intergral_img[index] = intergral_img[index - 1] + sum;
        }
    }

    //int s2 = (src.width() / 8) / 2;
    //const int s2 = (src.width() / 32) / 2; // 32 -> x is block size
    const int s2 = (src.width() / size) / 2; // 32 -> x is block size
                                             //const float T = 0.15f; // threshold
    const float Tr = threshold;

    // perform thresholding
    for (int x = 0; x < src.width(); x++)
    {
        for (int j = 0; j < src.height(); j++)
        {
            int index = j*src.width() + x;

            // set the SxS region
            int x1 = x - s2;
            int x2 = x + s2;
            int y1 = j - s2;
            int y2 = j + s2;

            // check the border
            if (x1 < 0) x1 = 0;
            if (x2 >= src.width()) x2 = src.width() - 1;
            if (y1 < 0) y1 = 0;
            if (y2 >= src.width()) y2 = src.width() - 1;

            const auto count = (x2 - x1)*(y2 - y1);

            // I(x,y)=s(x2,y2)-s(x1,y2)-s(x2,y1)+s(x1,x1)
            //int sum =
            const double sum =
                intergral_img[y2*src.width() + x2] - intergral_img[y1*src.width() + x2] -
                intergral_img[y2*src.width() + x1] + intergral_img[y1*src.width() + x1];

            // \todo this is gonna suck for floats and doubles...
            //if ((long)(src.data()[index] * count) < (long)(sum * (1.0 - Tr)))
            if ((double)(src.data()[index] * count) < (double)(sum * (1.0 - Tr)))
                dst.data()[index] = detail::image::pixel_min_value<T>();
            else
                dst.data()[index] = detail::image::pixel_max_value<T>();
        }
    }
}

template <typename T>
sti::core::image<T> filter_threshold_adaptive_bradley_copy(const sti::core::image<T> &src, int size, float threshold)
{
    auto dst = sti::core::image<T>(src.width(), src.height());
    filter_threshold_adaptive_bradley(src, dst, size, threshold);
    return dst;
}

} // namespace core
} // namespace sti
