#pragma once

#include <sti/plane.hpp>
#include <sti/pixel_values.hpp>
#include <sti/utility.hpp>
#include <algorithm>

namespace sti
{
namespace filter
{

/* One of my ideas was that as a functional building we would write these
 * 'filter' objects that we use to get the filter result of a single
 * destination pixel */
template <typename T>
class ifilter
{
public:
    virtual T operator()(const sti::plane<T> &src, const int y, const int x) const = 0;

private:
    ~ifilter() = default;
};

// \note this will get our selfs in a bad situation if somebody starts using 64 bit gray pixels
template <typename T>
using clamp_type = typename std::conditional<std::is_integral<T>::value, int, T>::type;

template <typename T, typename K>
static void apply_kernel(const sti::plane<T> &src, sti::plane<T> &dst, const K &kernel)
{
    const auto half_kernel_size = kernel.size() / 2;
    for (int y = 0; y < src.height(); ++y)
    {
        for (int x = 0; x < src.width(); ++x)
        {
            float sum = 0.f;
            for (auto k = -half_kernel_size; k <= half_kernel_size; ++k)
            {
                for (auto j = -half_kernel_size; j <= half_kernel_size; ++j)
                {
                    /* \note we only do clamp... which seems to me a bad option,
                     *       The reason for this is that pixels that are
                     *       clamped can become to dominant. Which can result in
                     *       weird looking pixels at the side.
                     *       Another way to handle this is to 'wrap' around but
                     *       I doubt that it would yield better results.
                     *       The last option would be to expand the source image
                     *       this would mean that we increase the width and height
                     *       by half the kernel size on every side. This has the
                     *       advantage that we would not need to check boundaries
                     *       while applying the filter. And the resulting pixels
                     *       on the border would look less weird because they
                     *       are actually generated with the correct amount of
                     *       source pixels.
                     */
                    const auto x1 = utility::clamp(x - j, 0, src.width() - 1);
                    const auto y1 = utility::clamp(y - k, 0, src.height() - 1);
                    const auto index = (y1 * src.width()) + x1;

                    sum += kernel[j + half_kernel_size][k + half_kernel_size] * static_cast<float>(src.data()[index]);
                }
            }
            const int index = (y * src.width()) + x;
            const float pixel = (kernel.factor * sum) + kernel.offset;

            // \todo rework this so we don't have todo weird things with clamp_type
            const auto pixel_temp = utility::clamp<clamp_type<T>>(
                static_cast<clamp_type<T>>(pixel), pixel_values<T>::min_value(), pixel_values<T>::max_value());
            dst.data()[index] = static_cast<T>(pixel_temp);
        }
    }
}

} // namespace filter
} // namespace sti
