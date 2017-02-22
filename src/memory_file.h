#pragma once

#include "memory_view.h"

#include <cstdint>
#include <cstdio>
#include <vector>
#include <string>


namespace sti
{
class memory_file
{
public:
    memory_file(const char *filename)
        : filename_(filename)
    {
    }

    void load()
    {
        FILE *fp = fopen(filename_.c_str(), "rb");
        if (!fp)
            return;

        fseek(fp, 0, SEEK_END);
        long fsize = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        buffer_.resize(fsize);

        fread(buffer_.data(), fsize, 1, fp);
        fclose(fp);
    }

    void save()
    {

    }

    uint8_t *data() {return buffer_.data();}
    size_t size() {return buffer_.size();}
    sti::memory_view view() {return sti::memory_view(buffer_.data(), buffer_.size());}

private:
    std::vector<uint8_t> buffer_;
    std::string filename_;
};

} // namespace sti
