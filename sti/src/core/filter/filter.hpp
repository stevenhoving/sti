#pragma once

#include "core/image.hpp"
#include "core/detail/utility_detail.hpp"
#include "core/detail/image_detail.hpp"

#include <algorithm>

namespace sti {
namespace core {
namespace filter {

template<typename T>
class ifilter
{
public:
    virtual double operator()(const sti::core::image<T> &src, const int y, const int x) const = 0;
};

template<typename T, typename K>
static void apply_kernel(const sti::core::image<T>& src,
                         sti::core::image<T>& dst, const K& kernel)
{
    const int half_kernel_size = kernel.size() / 2;
    for (int y = 0; y < src.height(); ++y)
    {
        for (int x = 0; x < src.width(); ++x)
        {
            float sum = 0.f;
            for (int k = -half_kernel_size; k <= half_kernel_size; k++)
            {
                for (int j = -half_kernel_size; j <= half_kernel_size; j++)
                {
                    //x1 = circular(src.cols, x - j);
                    //y1 = circular(src.rows, y - k);
                    //sum = sum + kernel[j + 1][k + 1] * src.data()[index];

                    // \note we only do clamp... atm... which seems to me as the worst option of all

                    //int x1 = std::max(0, x - j);
                    //x1 = std::min(x1, src.width()-1);
                    //int y1 = std::max(0, y - k);
                    //y1 = std::min(y1, src.height()-1);

                    const auto x1 = detail::clamp(x - j, 0, src.width() - 1);
                    const auto y1 = detail::clamp(y - k, 0, src.height() - 1);
                    const auto index = (y1 * src.width()) + x1;

                    sum = sum + kernel[j+kernel.size()/2][k+kernel.size()/2] *
                        (float)src.data()[index];
                }
            }
            const int index = (y * src.width()) + x;
            float pixel = (kernel.factor * sum) + kernel.offset;

            pixel = detail::clamp<T>(pixel, detail::image::pixel_min_value<T>(),
                                            detail::image::pixel_max_value<T>());
            dst.data()[index] = static_cast<T>(pixel);
        }
    }
}

} // namespace filter
} // namespace core
} // namespace sti
