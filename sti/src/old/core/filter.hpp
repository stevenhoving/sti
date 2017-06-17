#pragma once

#include "core/image.hpp"
#include "core/filter/filter.hpp"
#include "core/kernel/kernel.hpp"
#include "core/kernel/kernel_lowpass.hpp"

namespace sti
{
template <typename T, typename F>
sti::core::image<T> apply_filter(const sti::core::image<T> &src, F filter)
{
    const auto y_max = src.height();
    const auto x_max = src.width();
    auto image = sti::core::image<T>(x_max, y_max);

    for (int y = 0; y < y_max; ++y)
    {
        for (int x = 0; x < x_max; ++x)
        {
            image[y][x] = filter(src, y, x);
        }
    }
    return image;
}

/*template<typename T, int Size>
sti::core::filter::kernel<T, Size> make_kernel()
{

}
*/

/*namespace filter
{
namespace kernel
{
namespace lowpass = sti::core::filter::lowpass;
} // namespace kernel
} // namespace filter
*/

#if 0
template<typename T, typename K>
void apply_kernel(const sti::image<T>& src, sti::image<T>& dst, K kernel)
{
    const int half_kernel_size = kernel.size() / 2;
    const int x_max = src.width();
    const int y_max = src.height();

    for (int y = 0; y < y_max; ++y)
    {
        for (int x = 0; x < x_max; ++x)
        {
            double sum = 0.0;
            for (int k = -half_kernel_size; k <= half_kernel_size; ++k)
            {
                for (int j = -half_kernel_size; j <= half_kernel_size; ++j)
                {
                    //x1 = circular(src.cols, x - j);
                    //y1 = circular(src.rows, y - k);
                    //sum = sum + kernel[j + 1][k + 1] * src.data()[index];

                    // we only do clamp... atm... which is the worst option of all
                    int x1 = std::max(0, x - j);  x1 = std::min(x1, src.width() - 1);
                    int y1 = std::max(0, y - k);  y1 = std::min(y1, src.height() - 1);

                    //int index = ((y - k) * src.width()) + (x - j);
                    const int index = (y1 * src.width()) + x1;

                    sum = sum + kernel[j + kernel.size() / 2][k + kernel.size() / 2] * (double)src[index];
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

#endif

} // namespace sti
