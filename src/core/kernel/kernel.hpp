#pragma once

namespace sti
{
namespace core
{
namespace kernel
{
template<typename T, int Size>
class kernel
{
public:
    using value_type = std::array<T, Size * Size>;
    using iterator = typename value_type::iterator;
    using const_iterator = typename value_type::const_iterator;

    //kernel() = default;

    inline const int size() const { return Size; }

    inline iterator begin() { return data_.begin(); }
    inline iterator end() { return data_.end(); }
    inline const_iterator begin() const { return data_.begin(); }
    inline const_iterator end() const { return data_.end(); }

    // \note this is kinda bad...., we should return a memory_view here :P
    //       but only string_view exists atm.. !!!!!
    inline const T* operator[](const int y) const { return &data_[y * Size]; }

    // not const because kernel factories will change these values
    T factor = static_cast<T>(1.0);
    T offset = static_cast<T>(0.0);

    /* debug function to log the contents of the kernel to a file pointer */
    void dump(FILE *fp)
    {
        if (!fp)
            return; // \todo throw error or something...

        for (int y = 0; y < Size; ++y)
        {
            for (int x = 0; x < Size; ++x)
            {
                fprintf(fp, "%f ", static_cast<double>(this->operator[](y)[x]));
            }
            fprintf(fp, "\n");
        }
    }
private:
    value_type data_;
};

} // namespace kernel
} // namespace core
} // namespace sti
