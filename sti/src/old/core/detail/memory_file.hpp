#pragma once

#include <cstdint>
#include <cstdio>
#include <vector>
#include <string>

namespace sti
{
namespace detail
{

class memory_file
{
public:
    explicit memory_file(const std::string &filename)
        : filename_(filename)
    {
    }

    void load()
    {
        FILE *fp = fopen(filename_.c_str(), "rb");
        if (!fp)
        {
            // \todo throw something
            return;
        }

        fseek(fp, 0, SEEK_END);
        long fsize = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        buffer_.resize(fsize);

        fread(buffer_.data(), fsize, 1, fp);
        fclose(fp);
    }

    void save()
    {
        FILE *fp = fopen(filename_.c_str(), "wb");
        if (!fp)
        {
            // \todo throw up something
            return;
        }

        fwrite(buffer_.data(), buffer_.size(), 1, fp);
        fclose(fp);
    }

    inline uint8_t *data()
    {
        return const_cast<uint8_t *>(buffer_.data());
    }
    inline size_t size()
    {
        return buffer_.size();
    }

private:
    std::vector<uint8_t> buffer_;
    std::string filename_;
};

} // namespace detail
} // namespace sti
