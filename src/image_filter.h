#pragma once

#include "image.h"

#include <cstdint>
//#include <cstring>


namespace sti
{


namespace image_filter
{

//struct __attribute__((__packed__)) block
template <typename T>
struct __declspec(align(1)) block
{
    T p0;
    T p1;
    T p2;

    //inline unsigned int sum() const { return p0 + p1 + p2; }

    // fuck.... I want it to return a int when we deal with uint8_t but T when we deal with float or double.
    inline T sum() const { return p0 + p1 + p2; }
};

//static_assert(sizeof(block) == 3, "");


#if 0
void low_pass_3x3(const sti::image& src, sti::image& dst)
{
    int width = src.width();
    int height = src.height();

    for (int j = 0; j < height - 2; ++j)
    {
        int line_offset = j * width;
        const block * line0 = (block *)&src.data()[line_offset];
        const block * line1 = (block *)&src.data()[line_offset + width];
        const block * line2 = (block *)&src.data()[line_offset + width + width];

        for (int i = 0; i < (width - 2) / 3; ++i)
        {
            const block & l0 = line0[i];
            const block & l1 = line1[i];
            const block & l2 = line2[i];
            const unsigned int sum = l0.sum() + l1.sum() + l2.sum();

            dst.data()[(j * width) + i] = sum / 9;
        }
    }
}

#endif

#if 0


int low_pass_3x3(const bbyuv::image& src, bbyuv::image& dst)
{
    int width = src.width();
    int height = src.height();

    for (int j = 1; j < height - 1; ++j)
    {
        uint8_t * line0 = &src.data()[(j - 1) * width];
        uint8_t * line1 = &src.data()[(j - 0) * width];
        uint8_t * line2 = &src.data()[(j + 1) * width];

        uint8_t * dst_pixel = &dst.data()[(j - 0) * width];

        for (int i = 1; i < width - 1; ++i)
        {
            int line0_p0 = line0[i - 1];
            int line0_p1 = line0[i - 0];
            int line0_p2 = line0[i + 1];

            int line1_p0 = line1[i - 1];
            int line1_p1 = line1[i - 0];
            int line1_p2 = line1[i + 1];

            int line2_p0 = line2[i - 1];
            int line2_p1 = line2[i - 0];
            int line2_p2 = line2[i + 1];

            int sumtemp =
                line0_p0 + line0_p1 + line0_p2 +
                line1_p0 + line1_p1 + line1_p2 +
                line2_p0 + line2_p1 + line2_p2;

            dst_pixel[i] = sumtemp / 9;
        }
    }
    return 0;
}

#else

template<typename T>
void low_pass_3x3(const sti::image<T>& src, sti::image<T>& dst)
{
    int width = src.width();
    int height = src.height();

    memcpy(dst.data(), src.data(), width * height);

    for (int j = 0; j < height - 2; ++j)
    {
        T * line0 = &src.data()[(j) * width];
        T * line1 = &src.data()[(j + 1) * width];
        T * line2 = &src.data()[(j + 2) * width];

        //uint8_t * dst_pixel = &dst.data()[(j + 1) * width];

        for (int i = 0; i < width - 2; ++i)
        {
            const block<T> * l0 = (const block<T> *)&line0[i];
            const block<T> * l1 = (const block<T> *)&line1[i];
            const block<T> * l2 = (const block<T> *)&line2[i];

            auto sum = l0->sum() + l1->sum() + l2->sum();
            dst.data()[(j * width) + i] = sum / static_cast<T>(9.0);
        }
    }
}

#endif

#if 0
static int ApplyKernel5x5I420(uint8_t* src_argb, int width, int height, const float * kernel, const float kernel_div)
{
    static uint8_t * temp = nullptr;
    if (!temp)
    {
        temp = (uint8_t *)malloc(1280 * 960);
        memset(temp, 0, 1280 * 960);
    }

    if (!temp)
        return -1;

    memcpy(temp, src_argb, width * height);

    for (int j = 2; j < height - 2; ++j)
    {
        uint8_t * line0 = &temp[(j - 2) * width];
        uint8_t * line1 = &temp[(j - 1) * width];
        uint8_t * line2 = &temp[(j + 0) * width];
        uint8_t * line3 = &temp[(j + 1) * width];
        uint8_t * line4 = &temp[(j + 2) * width];

        uint8_t * dst = &src_argb[(j - 0) * width];

        for (int i = 2; i < width - 2; ++i)
        {
            double line0_p0 = double(line0[i - 2]) * kernel[0];
            double line0_p1 = double(line0[i - 1]) * kernel[1];
            double line0_p2 = double(line0[i + 0]) * kernel[2];
            double line0_p3 = double(line0[i + 1]) * kernel[3];
            double line0_p4 = double(line0[i + 2]) * kernel[4];

            double line1_p0 = double(line1[i - 2]) * kernel[5];
            double line1_p1 = double(line1[i - 1]) * kernel[6];
            double line1_p2 = double(line1[i + 0]) * kernel[7];
            double line1_p3 = double(line1[i + 1]) * kernel[8];
            double line1_p4 = double(line1[i + 2]) * kernel[9];

            double line2_p0 = double(line2[i - 2]) * kernel[10];
            double line2_p1 = double(line2[i - 1]) * kernel[11];
            double line2_p2 = double(line2[i + 0]) * kernel[12];
            double line2_p3 = double(line2[i + 1]) * kernel[13];
            double line2_p4 = double(line2[i + 2]) * kernel[14];

            double line3_p0 = double(line3[i - 2]) * kernel[15];
            double line3_p1 = double(line3[i - 1]) * kernel[16];
            double line3_p2 = double(line3[i + 0]) * kernel[17];
            double line3_p3 = double(line3[i + 1]) * kernel[18];
            double line3_p4 = double(line3[i + 2]) * kernel[19];

            double line4_p0 = double(line4[i - 2]) * kernel[20];
            double line4_p1 = double(line4[i - 1]) * kernel[21];
            double line4_p2 = double(line4[i + 0]) * kernel[22];
            double line4_p3 = double(line4[i + 1]) * kernel[23];
            double line4_p4 = double(line4[i + 2]) * kernel[24];

            double sumtemp =
                line0_p0 + line0_p1 + line0_p2 + line0_p3 + line0_p4 +
                line1_p0 + line1_p1 + line1_p2 + line1_p3 + line1_p4 +
                line2_p0 + line2_p1 + line2_p2 + line2_p3 + line2_p4 +
                line3_p0 + line3_p1 + line3_p2 + line3_p3 + line3_p4 +
                line4_p0 + line4_p1 + line4_p2 + line4_p3 + line4_p4;

            sumtemp /= kernel_div;

            if (sumtemp > 255)
                sumtemp = 255;
            if (sumtemp < 0)
                sumtemp = 0;

            dst[i] = (uint8_t)sumtemp;
        }
    }

    return 0;
}



template<typename T>
void kernel_filter_old(const sti::image<T>& src, sti::image& dst, T kernel)
{
    int width = src.width();
    int height = src.height();

    for (int j = 0; j < height - kernel->size(); ++j)
    {
        std::vector<uint8_t*> lines(kernel.size());
        for (int line = 0; line < kernel.size(); ++line)
            lines[line] = &src.data()[(j + line) * width];

        for (int i = 0; i < width - kernel->size(); ++i)
        {
            //const block * l0 = (const block *)&line0[i];
            //const block * l1 = (const block *)&line1[i];
            //const block * l2 = (const block *)&line2[i];

            //double line0_p0 = double(line0[i - 2]) * kernel[0];
            //double line0_p1 = double(line0[i - 1]) * kernel[1];
            //double line0_p2 = double(line0[i + 0]) * kernel[2];
            //double line0_p3 = double(line0[i + 1]) * kernel[3];
            //double line0_p4 = double(line0[i + 2]) * kernel[4];

            //const unsigned int sum = l0->sum() + l1->sum() + l2->sum();

            //dst.data()[(j * width) + i] = sum / 9;

            for (int kern_y = 0; kern_y < kernel->size(); ++kern_y)
                for (int kern_x = 0; kern_x < kernel->size(); ++kern_x)
                {

                }
        }
    }
}

#endif

template <typename T>
inline T clamp(const T value, const T min, const T max)
{
    return std::min<T>(std::max<T>(value, min), max);
}

template<typename T, typename K>
void apply_kernel(const sti::image<T>& src, sti::image<T>& dst, K kernel)
{
    const int half_kernel_size = kernel.size() / 2;
    for (int y = 0; y < src.height(); ++y)
    {
        for (int x = 0; x < src.width(); ++x)
        {
            double sum = 0.0;
            for (int k = -half_kernel_size; k <= half_kernel_size; k++)
            {
                for (int j = -half_kernel_size; j <= half_kernel_size; j++)
                {
                    //x1 = circular(src.cols, x - j);
                    //y1 = circular(src.rows, y - k);
                    //sum = sum + kernel[j + 1][k + 1] * src.data()[index];

                    // we only do clamp... atm... which is the worst option of all
                    int x1 = std::max(0, x - j);  x1 = std::min(x1, src.width()-1);
                    int y1 = std::max(0, y - k);  y1 = std::min(y1, src.height()-1);

                    //int index = ((y - k) * src.width()) + (x - j);
                    const int index = (y1 * src.width()) + x1;

                    sum = sum + kernel[j+kernel.size()/2][k+kernel.size()/2] * (double)src.data()[index];
                }
            }
            int index = (y * src.width()) + x;
            double pixel = (kernel.factor * sum) + kernel.offset;

            // use templated limit shit
            //pixel = std::min(std::max(pixel, 0.0), 255.0);
            //pixel = std::min<T>(std::max<T>(pixel, sti::pixel_min<T>()), sti::pixel_max<T>());
            pixel = clamp<T>(pixel, sti::pixel_min<T>(), sti::pixel_max<T>());
            dst.data()[index] = static_cast<T>(pixel);
        }
    }
}

template<typename T, typename K>
double convolution(const sti::image<T>& src, const int x, const int y, K kernel)
{
    const int half_kernel_size = kernel.size() / 2;
    
    double sum = 0.0;
    for (int k = -half_kernel_size; k <= half_kernel_size; ++k)
    {
        for (int j = -half_kernel_size; j <= half_kernel_size; ++j)
        {
            // we do clamp here... which is BADDDD
            int x1 = std::max(0, x - j); x1 = std::min(x1, src.width() - 1);
            int y1 = std::max(0, y - k); y1 = std::min(y1, src.height() - 1);

            const int index = (y1 * src.width()) + x1;

            sum += kernel[x + half_kernel_size][y + half_kernel_size] * (double)src.data()[index];
        }
    }
    return sum;
}

// hacky.. only applies for sobel edge detection
template<typename T, typename K>
void apply_kernels(const sti::image<T>& src, sti::image<T>& dst, K h_kernel, K v_kernel)
{
    for (int y = 0; y < src.height(); ++y)
    {
        for (int x = 0; x < src.width(); ++x)
        {
            const double sumx = convolution(src, x, y, h_kernel);
            const double sumy = convolution(src, x, y, v_kernel);

            double sum = std::sqrt(std::pow(sumx, 2) + std::pow(sumy, 2));
#if 0
            for (int k = -kernel.size() / 2; k <= kernel.size() / 2; ++k)
            {
                for (int j = -kernel.size() / 2; j <= kernel.size() / 2; ++j)
                {
                    //x1 = circular(src.cols, x - j);
                    //y1 = circular(src.rows, y - k);
                    //sum = sum + kernel[j + 1][k + 1] * src.data()[index];

                    // we only do clamp... atm... which is the worst option of all
                    int x1 = std::max(0, x - j);  x1 = std::min(x1, src.width() - 1);
                    int y1 = std::max(0, y - k);  y1 = std::min(y1, src.height() - 1);

                    const int index = (y1 * src.width()) + x1;
                    sum = sum + kernel[j + kernel.size() / 2][k + kernel.size() / 2] * (double)src.data()[index];
                }
            }
#endif
            
            int index = (y * src.width()) + x;
            //double pixel = (kernel.factor * sum) + kernel.offset;

            // have some sort of templated limit
            sum = std::min(std::max(sum, 0.0), 255.0);
            dst.data()[index] = 255 - sum;
        }
    }
}

#if 0


//#include "stdafx.h"
//#include <cmath>

//using namespace System::Drawing;
//using namespace System::Drawing::Imaging;

const int MASK_N = 2;
const int MASK_X = 3;
const int MASK_Y = 3;

// Convert RGB to gray level int
int colorToInt(Color &color)
{
    return (color.R + color.G + color.B) / 3;
}

void edgeDetector(Bitmap^ oriImg, Bitmap^ resImg, const int mask[MASK_N][MASK_X][MASK_Y], const double &threshold) {
    double val[MASK_N] = { 0.0 };

    for (int y = 0; y != oriImg->Height; ++y) {
        for (int x = 0; x != oriImg->Width; ++x) {
            for (int i = 0; i != MASK_N; ++i) {
                int adjustX = (MASK_X % 2) ? 1 : 0;
                int adjustY = (MASK_Y % 2) ? 1 : 0;
                int xBound = MASK_X / 2;
                int yBound = MASK_Y / 2;

                val[i] = 0.0;
                for (int v = -yBound; v != yBound + adjustY; ++v) {
                    for (int u = -xBound; u != xBound + adjustX; ++u) {
                        if (x + u >= 0 && x + u < oriImg->Width && y + v >= 0 && y + v < oriImg->Height) {
                            val[i] += colorToInt(oriImg->GetPixel(x + u, y + v)) * mask[i][u + xBound][v + yBound];
                        }
                    }
                }
            }

            double total = 0.0;
            for (int i = 0; i != MASK_N; ++i) {
                total += val[i] * val[i];
            }

            total = sqrt(total);

            if (total - threshold >= 0)
                resImg->SetPixel(x, y, Color::Black);
            else
                resImg->SetPixel(x, y, Color::White);
        }
    }
}

#endif

#if 0
int main() {
    const int mask[MASK_N][MASK_X][MASK_Y] = {
        { { -1, -2, -1 },
        { 0, 0, 0 },
        { 1, 2, 1 } },

        { { -1, 0, 1 },
        { -2, 0, 2 },
        { -1, 0, 1 } }
    };

    Bitmap^ oriImg = gcnew Bitmap("lena.bmp");
    Bitmap^ resImg = gcnew Bitmap(oriImg->Width, oriImg->Height);

    const double threshold = 90.0;
    edgeDetector(oriImg, resImg, mask, threshold);

    resImg->Save("lena_sobel.bmp");

    return 0;
}

#endif

} // namespace image_filter
} // namespace sti
