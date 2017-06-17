#pragma once

#include "core/filter/filter.hpp"
#include "core/kernel/kernel_lowpass.hpp"

namespace sti {
namespace core {

template<typename T>
void filter_lowpass(const sti::core::image<T> &src, sti::core::image<T> &dst)
{
    // default to float kernels for now
    auto kernel = sti::core::kernel::lowpass::make_kernel<float, 5>();
    sti::core::filter::apply_kernel(src, dst, kernel);
}

template<typename T>
sti::core::image<T> filter_lowpass_copy(const sti::core::image<T> &src)
{
    auto dst = sti::core::image<T>(src.width(), src.height());
    filter_lowpass(src, dst);
    return dst;
}

} // namespace core
} // namespace sti
