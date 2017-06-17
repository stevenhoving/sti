#pragma once

#include <sti/slice.hpp>
#include <sti/utility.hpp>
#include <sti/image_detail.hpp>

#include <algorithm>

namespace sti
{
namespace filter
{

template <typename pixel_type_t>
class ifilter
{
public:
    virtual double operator()(const slice<pixel_type_t> &src, const int y, const int x) const = 0;
};

template <typename pixel_type_t, typename kernel_t>
static void apply_kernel(const slice<pixel_type_t> &src, slice<pixel_type_t> &dst, const kernel_t &kernel)
{
    const auto half_kernel_size = kernel.size() / 2;
    for (auto y = 0; y < src.height(); ++y)
    {
        for (auto x = 0; x < src.width(); ++x)
        {
            auto sum = 0.0f;
            for (auto k = -half_kernel_size; k <= half_kernel_size; ++k)
            {
                for (auto j = -half_kernel_size; j <= half_kernel_size; ++j)
                {
                    // x1 = circular(src.cols, x - j);
                    // y1 = circular(src.rows, y - k);
                    // sum = sum + kernel[j + 1][k + 1] * src.data()[index];

                    // \note we only do clamp... atm... which seems to me as the worst option of all

                    // int x1 = std::max(0, x - j);
                    // x1 = std::min(x1, src.width()-1);
                    // int y1 = std::max(0, y - k);
                    // y1 = std::min(y1, src.height()-1);

                    const auto x1 = utility::clamp(x - j, 0, src.width() - 1);
                    const auto y1 = utility::clamp(y - k, 0, src.height() - 1);
                    const auto index = (y1 * src.width()) + x1;

                    sum = sum + kernel[j + kernel.size() / 2][k + kernel.size() / 2] * static_cast<float>(src.data()[index]);
                }
            }
            const int index = (y * src.width()) + x;
            float pixel = (kernel.factor * sum) + kernel.offset;

            pixel = utility::clamp<pixel_type_t>(pixel, detail::pixel_min_value<pixel_type_t>(), detail::pixel_max_value<pixel_type_t>());
            dst.data()[index] = static_cast<pixel_type_t>(pixel);
        }
    }
}

} // namespace filter
} // namespace sti
