#pragma once

namespace sti {
namespace core {

// maybe make these parameters
#define MEAN_SHIFT_PIXEL_DISTANCE 3
#define MEAN_SHIFT_MAX_ITERATIONS 10

template<typename T>
void filter_mean_shift(const sti::core::image<T> &src, sti::core::image<T> &dst,
                       const int spatial_radius, const float color_distance)
{
    int rad, rad2;
    float rad_col, rad_col2;

    rad = spatial_radius;
    rad2 = rad*rad;
    rad_col = color_distance;
    rad_col2 = rad_col*rad_col;

    const int width = src.width();
    const int height = src.height();

    const auto src_pixels = src.data();
    const auto dst_pixels = dst.data();

    float num_;

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
            
            const int pos = y*width + x;
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
                                if (ry*ry + rx*rx <= rad2)
                                {
                                    float Y2 = src_pixels[y2*width + x2];
                                    float dY = Yc - Y2;

                                    if (dY*dY <= rad_col2) {
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
                num_ = 1.f / num;
                Yc = mY*num_;
                xc = (int)(mx * num_ + 0.5f);
                yc = (int)(my * num_ + 0.5f);
                const int dx = xc - xc_old;
                const int dy = yc - yc_old;
                const float dY = Yc - Yc_old;

                shift = dx*dx + dy*dy + dY*dY;
                // when the shift (the distance in pixel color is less or
                // equal to 3 we assume they are equal.
            } while (shift > MEAN_SHIFT_PIXEL_DISTANCE && ++iters < 10);

            // \todo do clamp + narrow_cast
            dst_pixels[pos] = (uint8_t)Yc;
        }
    }
}

template<typename T>
sti::core::image<T> filter_mean_shift_copy(const sti::core::image<T> &src,
                                           const int spatial_radius,
                                           const float color_distance)
{
    auto dst = sti::core::image<T>(src.width(), src.height());
    filter_mean_shift(src, dst, spatial_radius, color_distance);
    return dst;
}

} // namespace core
} // namespace sti
