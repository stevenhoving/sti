#pragma once

#include "core/kernel/kernel.hpp"

#if 0
#include "filter.hpp"

namespace sti
{
namespace filter
{
template<typename T>
class lowpass : public ifilter<T>
{
public:

};
#endif

namespace sti
{
namespace core
{
namespace kernel
{
namespace lowpass
{

/*!
 * This lowpass filter is nothing more than just a averaging filter.
 * \note these are actually 'kernel generators' or something like that...
 */
template<typename T, int Size>
static kernel<T, Size> make_kernel()
{
    auto kern = core::kernel::kernel<T, Size>();
    for (auto &itr : kern)
        itr = static_cast<T>(1.0 / static_cast<T>(Size * Size));
    return kern;
}

} // namespace lowpass
} // namespace kernel
} // namespace core
} // namespace sti
