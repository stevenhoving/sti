#pragma once

#include <array>

namespace sti
{
template<typename T, int Size>
class kernel
{
public:
    const int size() const {return Size;}
    // fucked up.....!!!!!
    T* operator[](int y) {return &data[y * Size];}
    std::array<T, Size * Size> data;
    T factor = static_cast<T>(1.0);
    T offset = static_cast<T>(0.0);

    void dump()
    {
        for (int y = 0; y < Size; ++y)
        {
            for (int x = 0; x < Size; ++x)
            {
                printf("%f ", this->operator[](y)[x]);
            }
            printf("\n");
        }
    }
};

} // namespace sti
