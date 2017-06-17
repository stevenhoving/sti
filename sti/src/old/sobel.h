#pragma once

/*
3x3
sx = 1  0 -1
     2  0 -2
     1  0 -1

sy =-1 -2 -1
     0  0  0
     1  2  1

5x5
sx = 1   2   0  -2  -1
     4   8   0  -8  -4
     6  12   0 -12  -6
     4   8   0  -8  -4
     1   2   0  -2  -1

sy =-1  -4  -6  -4  -1
    -2  -8 -12  -8  -2
     0   0   0   0   0
     2   8  12   8   2
     1   4   6   4   1
*/

namespace sti
{

template <typename T, int Size>
class kernelh
{
public:
    const T operator[](int i) const
    {
        return data[i];
    }
    std::array<T, Size> data;
};

template <typename T, int Size>
class kernelv
{
public:
    const T operator[](int i) const
    {
        return data[i];
    }
    std::array<T, Size> data;
};

template <typename T, int Size>
sti::kernel<T, Size> operator*(const sti::kernelv<T, Size> &left, const sti::kernelh<T, Size> &right)
{
    sti::kernel<T, Size> kern;
    for (int y = 0; y < Size; ++y)
    {
        for (int x = 0; x < Size; ++x)
        {
            kern[x][y] = left[x] * right[y];
        }
    }

    return kern;
}

template <typename T, int Size>
sti::kernel<T, Size> operator*(const sti::kernelh<T, Size> &left, const sti::kernelv<T, Size> &right)
{
    sti::kernel<T, Size> kern;
    for (int y = 0; y < Size; ++y)
    {
        for (int x = 0; x < Size; ++x)
        {
            kern[x][y] = left[x] * right[y];
        }
    }

    return kern;
}

namespace sobel
{
namespace edge
{
namespace horizontal
{

template <typename T, int Size>
kernel<T, Size> make_kernel()
{
    kernelh<T, Size> h;
    kernelv<T, Size> v;
    h.data = {{-1, 0, 1}};
    v.data = {{1, 2, 1}};

    // h.data = {-1, -2, 0, 2, 1};
    // v.data = {1, 4, 6, 4, 1};

    auto kern = v * h;
    // kern.factor = 1.0/8.0;
    kern.dump();
    return kern;

    // kernel<T, Size> kern;
    // for (auto &itr : kern.data)
    // itr = static_cast<T>(-1.0);

    // kern[Size / 2][Size / 2] = static_cast<T>(Size * Size) - static_cast<T>(1.0);
    // return kern;
}

} // namespace horizontal

namespace vertical
{

} // namespace vertical

} // namespace edge
} // namespace sobel
} // namespace sti
