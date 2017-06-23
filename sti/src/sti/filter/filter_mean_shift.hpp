#pragma once

#include <sti/filter/filter.hpp>
#include <sti/utility.hpp>

namespace sti
{
namespace filter
{

// maybe make these parameters
#define MEAN_SHIFT_PIXEL_DISTANCE 3
#define MEAN_SHIFT_MAX_ITERATIONS 10

template <typename T>
void filter_mean_shift(const sti::plane<T> &src, sti::plane<T> &dst, const int spatial_radius,
                       const float color_distance)
{
    const auto rad = spatial_radius;
    const auto rad2 = rad * rad;
    const auto rad_col = color_distance;
    const auto rad_col2 = rad_col * rad_col;

    const int width = src.width();
    const int height = src.height();

    const auto src_pixels = src.data();
    const auto dst_pixels = dst.data();

    float shift = 0;
    int iters = 0;
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int xc_old;
            int yc_old;
            float Yc_old;

            int xc = x;
            int yc = y;

            const int pos = y * width + x;
            float Yc = src_pixels[pos];

            iters = 0;
            do
            {
                xc_old = xc;
                yc_old = yc;
                Yc_old = Yc;

                float mx = 0.f;
                float my = 0.f;
                float mY = 0.f;
                int num = 0;

                for (int ry = -rad; ry <= rad; ++ry)
                {
                    const int y2 = yc + ry;
                    if (y2 >= 0 && y2 < height)
                    {
                        for (int rx = -rad; rx <= rad; ++rx)
                        {
                            const int x2 = xc + rx;
                            if (x2 >= 0 && x2 < width)
                            {
                                if (ry * ry + rx * rx <= rad2)
                                {
                                    float Y2 = src_pixels[y2 * width + x2];
                                    float dY = Yc - Y2;

                                    if (dY * dY <= rad_col2)
                                    {
                                        mx += x2;
                                        my += y2;
                                        mY += Y2;
                                        num++;
                                    }
                                }
                            }
                        }
                    }
                }
                float num_ = 1.f / num;
                Yc = mY * num_;
                xc = (int)(mx * num_ + 0.5f);
                yc = (int)(my * num_ + 0.5f);
                const int dx = xc - xc_old;
                const int dy = yc - yc_old;
                const float dY = Yc - Yc_old;

                shift = dx * dx + dy * dy + dY * dY;
                // when the shift (the distance in pixel color is less or
                // equal to 3 we assume they are equal.
            } while (shift > MEAN_SHIFT_PIXEL_DISTANCE && ++iters < MEAN_SHIFT_MAX_ITERATIONS);

            // \todo do clamp + narrow_cast
            dst_pixels[pos] = (uint8_t)Yc;
        }
    }
}

template <typename T>
sti::plane<T> filter_mean_shift_copy(const sti::plane<T> &src, const int spatial_radius, const float color_distance)
{
    auto dst = sti::plane<T>(src.width(), src.height());
    filter_mean_shift(src, dst, spatial_radius, color_distance);
    return dst;
}

template <typename T>
class mean_shift_filter final : public sti::filter::ifilter<T>
{
public:
    mean_shift_filter(const int spatial_radius, const float color_distance)
        : spatial_radius_(spatial_radius)
        , color_distance_(color_distance)
    {
        rad = spatial_radius_;
        rad2 = rad * rad;
        rad_col = color_distance_;
        rad_col2 = rad_col * rad_col;
    }

    virtual T operator()(const sti::plane<T> &src, const int y, const int x) const override final
    {
        int width = src.width();
        int height = src.height();

        using value_type = sti::filter::clamp_type<T>;

        value_type xc_old;
        value_type yc_old;
        float Yc_old;

        int xc = x;
        int yc = y;

        const int pos = (y * width) + x;
        float Yc = src.data()[pos];

        // hack so we can actually have some valid results
        // value_type mean_shift_pixel_distance = static_cast<value_type>(MEAN_SHIFT_PIXEL_DISTANCE) / 255.f;
        value_type mean_shift_pixel_distance = static_cast<value_type>(MEAN_SHIFT_PIXEL_DISTANCE);

        float shift;
        int iters = 0;
        do
        {
            xc_old = xc;
            yc_old = yc;
            Yc_old = Yc;

            float mx = 0.f;
            float my = 0.f;
            float mY = 0.f;
            int num = 0;

            for (int ry = -rad; ry <= rad; ++ry)
            {
                const int y2 = yc + ry;
                if (y2 >= 0 && y2 < height)
                {
                    for (int rx = -rad; rx <= rad; ++rx)
                    {
                        const int x2 = xc + rx;
                        if (x2 >= 0 && x2 < width)
                        {
                            if (ry * ry + rx * rx <= rad2)
                            {
                                float Y2 = src.data()[y2 * width + x2];
                                float dY = Yc - Y2;

                                if (dY * dY <= rad_col2)
                                {
                                    mx += x2;
                                    my += y2;
                                    mY += Y2;
                                    num++;
                                }
                            }
                        }
                    }
                }
            }
            auto num_ = 1.f / num;
            Yc = mY * num_;
            xc = (value_type)(mx * num_ + 0.5f); // rough int round...
            yc = (value_type)(my * num_ + 0.5f);
            const value_type dx = xc - xc_old;
            const value_type dy = yc - yc_old;
            const float dY = Yc - Yc_old;

            shift = dx * dx + dy * dy + dY * dY;
            // when the shift (the distance in pixel color) is less or
            // equal to 3 we assume they are equal.
        } while (shift > mean_shift_pixel_distance && ++iters < MEAN_SHIFT_MAX_ITERATIONS);

        // \todo do clamp + narrow_cast if needed.
        return (uint8_t)Yc;
        // return Yc;
    }

    // filter parameters
    const int spatial_radius_;
    const float color_distance_;

    // helper variables
    int rad, rad2;
    float rad_col, rad_col2;
};

} // namespace filter
} // namespace sti
