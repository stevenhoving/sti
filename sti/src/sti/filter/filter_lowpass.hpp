#pragma once

#include <sti/filter/filter.hpp>
#include <sti/kernel/kernel_lowpass.hpp>

namespace sti
{
namespace filter
{

template <typename T>
void filter_lowpass(const sti::slice<T> &src, sti::slice<T> &dst)
{
    // default to float kernels for now
    auto kernel = kernel::lowpass::make_kernel<float, 5>();
    filter::apply_kernel(src, dst, kernel);
}

template <typename T>
sti::slice<T> filter_lowpass_copy(const sti::slice<T> &src)
{
    auto dst = sti::slice<T>(src.width(), src.height());
    filter_lowpass(src, dst);
    return dst;
}

} // namespace filter
} // namespace sti
