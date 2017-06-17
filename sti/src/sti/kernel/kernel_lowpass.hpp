#pragma once

#include <sti/kernel/kernel.hpp>

namespace sti
{
namespace kernel
{
namespace lowpass
{

/*!
 * This lowpass filter is nothing more than just a averaging filter.
 * \note these are actually 'kernel generators' or something like that...
 */
template <typename T, int Size>
static kernel<T, Size> make_kernel()
{
    auto kern = sti::kernel::kernel<T, Size>();
    for (auto &itr : kern)
        itr = static_cast<T>(1.0 / static_cast<T>(Size * Size));
    return kern;
}

} // namespace lowpass
} // namespace kernel
} // namespace sti
