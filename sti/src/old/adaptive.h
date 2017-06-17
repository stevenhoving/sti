#pragma once

#include "image.h"
#include "histogram.h"
#include "stop_watch.h"

#include <cmath>
#include <climits>
#include <cfloat>

#include <xmmintrin.h>
#include <emmintrin.h>
#include <ammintrin.h>
#include <immintrin.h>

namespace sti
{
namespace image_filter
{

/*
Original function for calculating the mean threshold for a block.

inline
int calculate_mean_thresh(uint8_t *src, int width, int height, int x, int y, int size, int con)
{
    int mean = 0;
    int count = 0;
    int half_size = size / 2;

    for (int k = 0; k < size; ++k)
    {
        for (int l = 0; l < size; ++l)
        {
            int box_x = (x - half_size) + k;
            int box_y = (y - half_size) + l;
            if (box_x < 0 || box_y < 0)
                continue;

            int index = (box_y * width) + box_x;
            mean = mean + src[index];
            count++;
        }
    }
    //Find the mean value
    mean = (int)(mean / count) - con;
    return mean;
}
*/

/*
Experimental function for calculating the mean threshold for a block. The trick I tried here is to
only have a single loop instead of 2 loops. The goal was to improve performance by not having
2 loops. Todo this I was kinda forced to use a modulo, which still are slow. It also had todo
more calculations per pixel, and because its not 'standard' the compiler was unable to vectorize it.


inline
int calculate_mean_thresh_new(const uint8_t *src, const int width, const int height, const int x,
                              const int y, const int size, const int con)
{
    int mean = 0;

    const int half_size = size / 2;
    const int x_offset = x - half_size;
    const int y_offset = y - half_size;

    const int box_x_min = x_offset < 0 ? 0 : x_offset;
    const int box_y_min = y_offset < 0 ? 0 : y_offset;
    const int box_x_max = std::min(width, size + x_offset);
    const int box_y_max = std::min(height, size + y_offset);

    const int count = (box_y_max - box_y_min) * (box_x_max - box_x_min);
    const int box_width = box_x_max - box_x_min;
    for (int i = 0; i < count; ++i)
    {
        const int box_x = (i % box_width) + box_x_min;
        const int box_y = (i / box_width) + box_y_min;
        const int index = (box_y * width) + box_x;

        mean = mean + src[index];
    }

    //Find the mean value
    mean = (int)(mean / count) - con;
    return mean;
}
*/

template <typename T>
T get_con_norm()
{
    return 255.0;
}

template <>
uint8_t get_con_norm()
{
    return 1;
}

/* \note experimental, optimized to not have any conditions in the for loops
 * this more or less behaves as a box filter.
 */
template <typename T>
// inline int calculate_mean_thresh(const T *src, const int width, const int height, const int x,
inline T calculate_mean_thresh(const T *src, const int width, const int height, const int x, const int y,
                               const int size, const int con)
{
    const int half_size = size / 2;
    const int x_offset = x - half_size;
    const int y_offset = y - half_size;

    const int box_x_min = x_offset < 0 ? 0 : x_offset;
    const int box_y_min = y_offset < 0 ? 0 : y_offset;
    const int box_x_max = std::min(width, size + x_offset);
    const int box_y_max = std::min(height, size + y_offset);

    // const int count = (box_y_max - box_y_min) * (box_x_max - box_x_min);
    const double count = (box_y_max - box_y_min) * (box_x_max - box_x_min);

    // int mean = 0;
    double mean = 0.0;

    for (int box_y = box_y_min; box_y < box_y_max; ++box_y)
        for (int box_x = box_x_min; box_x < box_x_max; ++box_x)
        {
            const int index = (box_y * width) + box_x;
            mean = mean + src[index];
        }

    // Find the mean value
    mean = (mean / count) - (con / get_con_norm<T>());
    return mean;
}

template <typename T>
void adaptive_threshold_mean(sti::image<T> &src, sti::image<T> &dst, int size, int con)
{
    const int width = src.width();
    const int height = src.height();

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            const int index = (y * width) + x;
            const auto pix = src.data()[index];
            // const int threshold = calculate_mean_thresh<T>(src.data(), width, height, x, y, size, con);
            const double threshold = calculate_mean_thresh<T>(src.data(), width, height, x, y, size, con);
            if (pix >= threshold)
                dst.data()[index] = pixel_max<T>();
            if (pix < threshold)
                dst.data()[index] = pixel_min<T>();
        }
    }
}

template <typename T>
auto adaptive_threshold_bradley_intergral()
{
    return std::vector<T>();
}

template <>
auto adaptive_threshold_bradley_intergral<uint8_t>()
{
    return std::vector<int>();
}

template <typename T>
auto adaptive_threshold_bradley_sum()
{
    return 0.0;
}

template <>
auto adaptive_threshold_bradley_sum<uint8_t>()
{
    return 0;
}

// https://github.com/rmtheis/bradley-adaptive-thresholding/raw/master/gerh-50002.pdf
// \todo parameterize this function: 'block size' and 'threshold'. But to be sure... read the paper.
template <typename T>
void adaptive_threshold_bradley(const sti::image<T> &src, sti::image<T> &dst, int size, float threshold)
{
    /* create integral image (aka summed-area table) */
    // std::vector<int> intergral_img;
    // std::vector<double> intergral_img;
    auto intergral_img = adaptive_threshold_bradley_intergral<T>();
    intergral_img.resize(src.width() * src.height() * 4);

    for (int x = 0; x < src.width(); ++x)
    {
        // int sum = 0;
        // double sum = 0;
        auto sum = adaptive_threshold_bradley_sum<T>();
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

    // int s2 = (src.width() / 8) / 2;
    // const int s2 = (src.width() / 32) / 2; // 32 -> x is block size
    const int s2 = (src.width() / size) / 2; // 32 -> x is block size
    // const float T = 0.15f; // threshold
    const float Tr = threshold;

    int count = 0;

    // perform thresholding
    for (int x = 0; x < src.width(); x++)
    {
        for (int j = 0; j < src.height(); j++)
        {
            int index = j * src.width() + x;

            // set the SxS region
            int x1 = x - s2;
            int x2 = x + s2;
            int y1 = j - s2;
            int y2 = j + s2;

            // check the border
            if (x1 < 0)
                x1 = 0;
            if (x2 >= src.width())
                x2 = src.width() - 1;
            if (y1 < 0)
                y1 = 0;
            if (y2 >= src.width())
                y2 = src.width() - 1;

            count = (x2 - x1) * (y2 - y1);

            // I(x,y)=s(x2,y2)-s(x1,y2)-s(x2,y1)+s(x1,x1)
            // int sum =
            const double sum = intergral_img[y2 * src.width() + x2] - intergral_img[y1 * src.width() + x2] -
                               intergral_img[y2 * src.width() + x1] + intergral_img[y1 * src.width() + x1];

            // \note this is gonna suck for floats and doubles... todo... fix it...
            // if ((long)(src.data()[index] * count) < (long)(sum * (1.0 - Tr)))
            if ((double)(src.data()[index] * count) < (double)(sum * (1.0 - Tr)))
                dst.data()[index] = sti::pixel_min<T>();
            else
                dst.data()[index] = sti::pixel_max<T>();
        }
    }
}

/*!
 * Segments the image using the computed threshold
 *
 * \param[in] input Input image
 * \param[out] output Output segmented image
 * \param[in] threshold Threshold used for segmentation
 */
template <typename T>
// inline void segment_image(const sti::image<T> &src, sti::image<T> &dst, const int threshold)
inline void segment_image(const sti::image<T> &src, sti::image<T> &dst, const double threshold)
{
    auto N = src.width() * src.height();
    auto in = src.data();
    auto out = dst.data();
    for (int i = 0; i < N; i++)
    {
        const auto value = in[i];
        if (value > threshold)
            out[i] = sti::pixel_max<T>();
        else
            out[i] = sti::pixel_min<T>();
    }
}

static const int OTSUS_MAX_INTENSITY = 255;

/*!
 * Computes Otsus segmentation
 * \param input Input image
 * \param hist Image histogram
 * \param output Output segmented image
 * \param overrided_threshold Input param that overrides threshold
*/
template <typename T>
void otsu_compute_segmentation(const sti::image<T> &input, const sti::histogram &hist, sti::image<T> &output)
{
    // Compute number of pixels
    auto N = input.width() * input.height();
    int threshold = 0;

    // Compute threshold
    // Init variables
    float sum = 0;
    float sumB = 0;
    int q1 = 0;
    int q2 = 0;
    float varMax = 0;

    // Auxiliary value for computing m2
    for (int i = 0; i <= OTSUS_MAX_INTENSITY; i++)
        sum += i * ((int)hist[i]);

    for (int i = 0; i <= OTSUS_MAX_INTENSITY; i++)
    {
        // Update q1
        q1 += hist[i];
        if (q1 == 0)
            continue;

        // Update q2
        q2 = N - q1;
        if (q2 == 0)
            break;

        // Update m1 and m2
        sumB += (float)(i * ((int)hist[i]));
        float m1 = sumB / q1;
        float m2 = (sum - sumB) / q2;

        // Update the between class variance
        float varBetween = (float)q1 * (float)q2 * (m1 - m2) * (m1 - m2);

        // Update the threshold if necessary
        if (varBetween > varMax)
        {
            varMax = varBetween;
            threshold = i;
        }
    }

    // Perform the segmentation
    segment_image<T>(input, output, (double)threshold / 255.f);
}

// http://www.ipol.im/pub/art/2016/158/article.pdf
template <typename T>
void adaptive_threshold_otsu_segmented(const sti::image<T> &src, sti::image<T> &dst)
{
    auto histogram = sti::to_histogram(src);
    otsu_compute_segmentation<T>(src, histogram, dst);
}

// Shanhbag A.G. (1994) "Utilization of Information Measure as a Means of
//  Image Thresholding" Graphical Models and Image Processing, 56(5): 414-419
// Ported to ImageJ plugin by G.Landini from E Celebi's fourier_0.8 routines
template <typename T>
int get_threshold_shanbhag(const sti::image<T> &src)
{
    auto histogram = sti::to_histogram(src);

    int threshold;
    int ih, it;
    int first_bin;
    int last_bin;
    double term;
    double tot_ent;                                  /* total entropy */
    double min_ent;                                  /* max entropy */
    double ent_back;                                 /* entropy of the background pixels at a given threshold */
    double ent_obj;                                  /* entropy of the object pixels at a given threshold */
    auto norm_histo = sti::histogram_base<double>(); /* normalized histogram */
    auto P1 = sti::histogram_base<double>();         /* cumulative normalized histogram */
    auto P2 = sti::histogram_base<double>();

    int total = 0;
    for (ih = 0; ih < 256; ih++)
        total += histogram[ih];

    for (ih = 0; ih < 256; ih++)
        norm_histo[ih] = (double)histogram[ih] / total;

    P1[0] = norm_histo[0];
    P2[0] = 1.0 - P1[0];
    for (ih = 1; ih < 256; ih++)
    {
        P1[ih] = P1[ih - 1] + norm_histo[ih];
        P2[ih] = 1.0 - P1[ih];
    }

    /* Determine the first non-zero bin */
    first_bin = 0;
    for (ih = 0; ih < 256; ih++)
    {
        if (!(std::abs(P1[ih]) < 2.220446049250313E-16))
        {
            first_bin = ih;
            break;
        }
    }

    /* Determine the last non-zero bin */
    last_bin = 255;
    for (ih = 255; ih >= first_bin; ih--)
    {
        if (!(std::abs(P2[ih]) < 2.220446049250313E-16))
        {
            last_bin = ih;
            break;
        }
    }

    // Calculate the total entropy each gray-level
    // and find the threshold that maximizes it
    threshold = -1;
    min_ent = DBL_MAX;

    for (it = first_bin; it <= last_bin; it++)
    {
        /* Entropy of the background pixels */
        ent_back = 0.0;
        term = 0.5 / P1[it];
        for (ih = 1; ih <= it; ih++)
        { // 0+1?
            ent_back -= norm_histo[ih] * std::log(1.0 - term * P1[ih - 1]);
        }
        ent_back *= term;

        /* Entropy of the object pixels */
        ent_obj = 0.0;
        term = 0.5 / P2[it];
        for (ih = it + 1; ih < 256; ih++)
        {
            ent_obj -= norm_histo[ih] * std::log(1.0 - term * P2[ih]);
        }
        ent_obj *= term;

        /* Total entropy */
        tot_ent = std::abs(ent_back - ent_obj);

        if (tot_ent < min_ent)
        {
            min_ent = tot_ent;
            threshold = it;
        }
    }
    return threshold;
}

template <typename T>
void adaptive_threshold_shanbhag(const sti::image<T> &src, sti::image<T> &dst)
{
    auto threshold = get_threshold_shanbhag(src);
    segment_image(src, dst, threshold);
}

/*!
 * Zack, G. W., Rogers, W. E. and Latt, S. A., 1977,
 * Automatic Measurement of Sister Chromatid Exchange Frequency,
 * Journal of Histochemistry and Cytochemistry 25 (7), pp. 741-753
 *
 * modified from Johannes Schindelin plugin
 *
 *  find min and max
 */
template <typename T>
int get_threshold_triangle(const sti::image<T> &src)
{
    auto histogram = sti::to_histogram(src);

    int min = 0;
    unsigned int dmax = 0;
    int max = 0;
    int min2 = 0;
    for (int i = 0; i < 256; i++)
    {
        if (histogram[i] > 0)
        {
            min = i;
            break;
        }
    }
    if (min > 0)
        min--; // line to the (p==0) point, not to data[min]

    // The Triangle algorithm cannot tell whether the data is skewed to one side or another.
    // This causes a problem as there are 2 possible thresholds between the max and the 2 extremes
    // of the histogram.
    // Here I propose to find out to which side of the max point the data is furthest, and use that as
    //  the other extreme.
    for (int i = 255; i > 0; i--)
    {
        if (histogram[i] > 0)
        {
            min2 = i;
            break;
        }
    }
    if (min2 < 255)
        min2++; // line to the (p==0) point, not to data[min]

    for (int i = 0; i < 256; i++)
    {
        if (histogram[i] > dmax)
        {
            max = i;
            dmax = histogram[i];
        }
    }
    // find which is the furthest side
    // IJ.log(""+min+" "+max+" "+min2);
    bool inverted = false;
    if ((max - min) < (min2 - max))
    {
        // reverse the histogram
        // IJ.log("Reversing histogram.");
        inverted = true;
        int left = 0;    // index of leftmost element
        int right = 255; // index of rightmost element
        while (left < right)
        {
            // exchange the left and right elements
            int temp = histogram[left];
            histogram[left] = histogram[right];
            histogram[right] = temp;
            // move the bounds toward the center
            left++;
            right--;
        }
        min = sti::pixel_max<T>() - min2;
        max = sti::pixel_max<T>() - max;
    }

    if (min == max)
    {
        // IJ.log("Triangle:  min == max.");
        return min;
    }

    // describe line by nx * x + ny * y - d = 0
    double nx, ny, d;
    // nx is just the max frequency as the other point has freq=0
    nx = histogram[max]; //-min; // data[min]; //  lowest value bmin = (p=0)% in the image
    ny = min - max;
    d = std::sqrt(nx * nx + ny * ny);
    nx /= d;
    ny /= d;
    d = nx * min + ny * histogram[min];

    // find split point
    int split = min;
    double splitDistance = 0;
    for (int i = min + 1; i <= max; i++)
    {
        double newDistance = nx * i + ny * histogram[i] - d;
        if (newDistance > splitDistance)
        {
            split = i;
            splitDistance = newDistance;
        }
    }
    split--;

    if (inverted)
    {
        // The histogram might be used for something else, so let's reverse it back
        int left = 0;
        int right = 255;
        while (left < right)
        {
            int temp = histogram[left];
            histogram[left] = histogram[right];
            histogram[right] = temp;
            left++;
            right--;
        }
        return (255 - split);
    }
    else
        return split;
}

template <typename T>
void adaptive_threshold_triangle(const sti::image<T> &src, sti::image<T> &dst)
{
    auto threshold = get_threshold_triangle(src);
    segment_image(src, dst, threshold);
}

/*!
 * Implements Yen  thresholding method
 * 1) Yen J.C., Chang F.J., and Chang S. (1995) "A New Criterion
 *    for Automatic Multilevel Thresholding" IEEE Trans. on Image
 *    Processing, 4(3): 370-378
 * 2) Sezgin M. and Sankur B. (2004) "Survey over Image Thresholding
 *    Techniques and Quantitative Performance Evaluation" Journal of
 *    Electronic Imaging, 13(1): 146-165
 *    http://citeseer.ist.psu.edu/sezgin04survey.html
 *
 * M. Emre Celebi
 * 06.15.2007
 * Ported to ImageJ plugin by G.Landini from E Celebi's fourier_0.8 routines
*/
template <typename T>
int get_threshold_yen(const sti::image<T> &src)
{
    int threshold;
    int it;
    double crit;
    double max_crit;

    auto histogram = sti::to_histogram(src);
    auto norm_histo = sti::histogram_base<double>(); /* normalized histogram */
    auto P1 = sti::histogram_base<double>();         /* cumulative normalized histogram */
    auto P1_sq = sti::histogram_base<double>();
    auto P2_sq = sti::histogram_base<double>();

    int total = 0;

    for (int ih = 0; ih < 256; ih++)
        total += histogram[ih];

    for (int ih = 0; ih < 256; ih++)
        norm_histo[ih] = (double)histogram[ih] / (double)total;

    P1[0] = norm_histo[0];
    for (int ih = 1; ih < 256; ih++)
        P1[ih] = P1[ih - 1] + norm_histo[ih];

    P1_sq[0] = norm_histo[0] * norm_histo[0];
    for (int ih = 1; ih < 256; ih++)
        P1_sq[ih] = P1_sq[ih - 1] + norm_histo[ih] * norm_histo[ih];

    P2_sq[255] = 0.0;
    for (int ih = 254; ih >= 0; ih--)
        P2_sq[ih] = P2_sq[ih + 1] + norm_histo[ih + 1] * norm_histo[ih + 1];

    /* Find the threshold that maximizes the criterion */
    threshold = -1;
    max_crit = DBL_MIN; // Double.MIN_VALUE;
    for (it = 0; it < 256; it++)
    {
        // crit = -1.0 * ((P1_sq[it] * P2_sq[it])> 0.0 ? log(P1_sq[it] * P2_sq[it]) : 0.0) + 2.0 * ((P1[it] * (1.0 -
        // P1[it]))>0.0 ? log(P1[it] * (1.0 - P1[it])) : 0.0);

        crit = -1.0;
        if (P1_sq[it] * P2_sq[it] > 0.0)
        {
            crit *= std::log(P1_sq[it] * P2_sq[it]);
        }

        if (P1[it] * (1.0 - P1[it]) > 0.0)
        {
            crit += 2.0 * std::log(P1[it] * (1.0 - P1[it]));
        }

        if (crit > max_crit)
        {
            max_crit = crit;
            threshold = it;
        }
    }
    return threshold;
}

template <typename T>
void adaptive_threshold_yen(const sti::image<T> &src, sti::image<T> &dst)
{
    auto threshold = get_threshold_yen(src);
    segment_image(src, dst, threshold);
}

} // namespace image_filter
} // namespace sti
