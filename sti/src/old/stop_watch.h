#pragma once

#include <cstdint>
//#include <inttypes.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace sti
{

class stop_watch
{
public:
    stop_watch()
        : m_resolution(0.0)
        , m_sampled_time(0.0)
        , m_start_time(0)
    {
        std::int64_t freq;

        // Check if we have a performance counter
        if (QueryPerformanceFrequency((LARGE_INTEGER *)&freq))
        {
            // Counter resolution is 1 / counter frequency
            m_resolution = 1.0 / (double)freq;

            // Set start time for timer
            (void)QueryPerformanceCounter((LARGE_INTEGER *)&m_start_time);
        }
    }

    void time_start()
    {
        // store so we can easily do something like delta time
        m_sampled_time = _get_time();
    }

    double time_since()
    {
        return _get_time() - m_sampled_time;
    }

protected:
    double _get_time()
    {
        double t;
        int64_t t_64;

        (void)QueryPerformanceCounter((LARGE_INTEGER *)&t_64);

        t = (double)(t_64 - m_start_time);

        /* Calculate the current time */
        return t * m_resolution;
    }

private:
    double m_resolution;
    double m_sampled_time;
    int64_t m_start_time;
};

} // namespace sti
