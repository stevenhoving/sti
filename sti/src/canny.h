#pragma once

#include "image.h"

#include <cassert>
#include <cmath>
#include <cstdint>
#include <vector>

//#define MAX_BRIGHTNESS 255

// C99 doesn't define M_PI (GNU-C99 does)
//#define M_PI 3.14159265358979323846264338327

namespace sti
{
namespace canny
{
#define pixel_t uint8_t
//#define pixel_t float

// if normalize is true, map pixels to range 0..MAX_BRIGHTNESS
template <typename T>
void convolution(const T *in, T *out, const float *kernel,
                 const int nx, const int ny, const int kn,
                 const bool normalize)
{
    assert(kn % 2 == 1);
    assert(nx > kn && ny > kn);
    const int khalf = kn / 2;
    float min = FLT_MAX, max = -FLT_MAX;
 
    if (normalize)
        for (int m = khalf; m < nx - khalf; m++)
            for (int n = khalf; n < ny - khalf; n++) {
                float pixel = 0.0;
                size_t c = 0;
                for (int j = -khalf; j <= khalf; j++)
                    for (int i = -khalf; i <= khalf; i++) {
                        pixel += in[(n - j) * nx + m - i] * kernel[c];
                        c++;
                    }
                if (pixel < min)
                    min = pixel;
                if (pixel > max)
                    max = pixel;
                }
 
    for (int m = khalf; m < nx - khalf; m++)
        for (int n = khalf; n < ny - khalf; n++) {
            float pixel = 0.0;
            size_t c = 0;
            for (int j = -khalf; j <= khalf; j++)
                for (int i = -khalf; i <= khalf; i++) {
                    pixel += in[(n - j) * nx + m - i] * kernel[c];
                    c++;
                }
 
            if (normalize)
                //pixel = MAX_BRIGHTNESS * (pixel - min) / (max - min);
                pixel = sti::pixel_max<T>() * (pixel - min) / (max - min);
            out[n * nx + m] = (pixel_t)pixel;
        }
}
 
/*
 * gaussianFilter:
 * http://www.songho.ca/dsp/cannyedge/cannyedge.html
 * determine size of kernel (odd #)
 * 0.0 <= sigma < 0.5 : 3
 * 0.5 <= sigma < 1.0 : 5
 * 1.0 <= sigma < 1.5 : 7
 * 1.5 <= sigma < 2.0 : 9
 * 2.0 <= sigma < 2.5 : 11
 * 2.5 <= sigma < 3.0 : 13 ...
 * kernelSize = 2 * int(2*sigma) + 3;
 */
template <typename T>
void gaussian_filter(const T *in, T *out,
                     const int nx, const int ny, const float sigma)
{
    const int n = 2 * (int)(2 * sigma) + 3;
    const float mean = (float)floor(n / 2.0);
    //float kernel[n * n]; // variable length array
    std::vector<float> kernel(n * n);
 
    fprintf(stderr, "gaussian_filter: kernel size %d, sigma=%g\n", n, sigma);
    size_t c = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            kernel[c] = std::exp(-0.5 * (std::pow((i - mean) / sigma, 2.0) +
                                         std::pow((j - mean) / sigma, 2.0)))
                        / (2.0 * M_PI * sigma * sigma);
            c++;
        }
 
    convolution(in, out, kernel.data(), nx, ny, n, true);
}
 
/*
 * Links:
 * http://en.wikipedia.org/wiki/Canny_edge_detector
 * http://www.tomgibara.com/computer-vision/CannyEdgeDetector.java
 * http://fourier.eng.hmc.edu/e161/lectures/canny/node1.html
 * http://www.songho.ca/dsp/cannyedge/cannyedge.html
 *
 * Note: T1 and T2 are lower and upper thresholds.
 */
//void canny_edge_detection(const pixel_t *in,
                              //const bitmap_info_header_t *bmp_ih,
                              //const int tmin, const int tmax,
                              //const float sigma)

template <typename T>
void canny_edge_detection(const sti::image<T> &in, sti::image<T> &out, const int tmin, const int tmax, const float sigma)
{
    const int nx = in.width();
    const int ny = in.height();
 
    pixel_t *G = (pixel_t*)calloc(nx * ny * sizeof(pixel_t), 1);
    pixel_t *after_Gx = (pixel_t*)calloc(nx * ny * sizeof(pixel_t), 1);
    pixel_t *after_Gy = (pixel_t*)calloc(nx * ny * sizeof(pixel_t), 1);
    pixel_t *nms = (pixel_t*)calloc(nx * ny * sizeof(pixel_t), 1);
 
    if (G == NULL || after_Gx == NULL || after_Gy == NULL ||
        nms == NULL) {
        fprintf(stderr, "canny_edge_detection:"
                " Failed memory allocation(s).\n");
        exit(1);
    }
 
    gaussian_filter(in.data(), out.data(), nx, ny, sigma);
 
    const float Gx[] = {-1, 0, 1,
                        -2, 0, 2,
                        -1, 0, 1};
 
    convolution(out.data(), after_Gx, Gx, nx, ny, 3, false);
 
    const float Gy[] = { 1, 2, 1,
                         0, 0, 0,
                        -1,-2,-1};
 
    convolution(out.data(), after_Gy, Gy, nx, ny, 3, false);
 
    for (int i = 1; i < nx - 1; i++)
        for (int j = 1; j < ny - 1; j++) {
            const int c = i + nx * j;
            // G[c] = abs(after_Gx[c]) + abs(after_Gy[c]);
            G[c] = (pixel_t)hypot(after_Gx[c], after_Gy[c]);
        }
 
    // Non-maximum suppression, straightforward implementation.
    for (int i = 1; i < nx - 1; i++)
        for (int j = 1; j < ny - 1; j++) {
            const int c = i + nx * j;
            const int nn = c - nx;
            const int ss = c + nx;
            const int ww = c + 1;
            const int ee = c - 1;
            const int nw = nn + 1;
            const int ne = nn - 1;
            const int sw = ss + 1;
            const int se = ss - 1;
 
            const float dir = (float)(std::fmod(std::atan2(after_Gy[c],
                                                           after_Gx[c]) + M_PI,
                                                M_PI) / M_PI) * 8;
 
            if (((dir <= 1 || dir > 7) && G[c] > G[ee] &&
                 G[c] > G[ww]) || // 0 deg
                ((dir > 1 && dir <= 3) && G[c] > G[nw] &&
                 G[c] > G[se]) || // 45 deg
                ((dir > 3 && dir <= 5) && G[c] > G[nn] &&
                 G[c] > G[ss]) || // 90 deg
                ((dir > 5 && dir <= 7) && G[c] > G[ne] &&
                 G[c] > G[sw]))   // 135 deg
                nms[c] = G[c];
            else
                nms[c] = 0;
        }
 
    // Reuse array
    // used as a stack. nx*ny/2 elements should be enough.
    int *edges = (int*) after_Gy;
    memset(out.data(), 0, sizeof(pixel_t) * nx * ny);
    memset(edges, 0, sizeof(pixel_t) * nx * ny);
 
    // Tracing edges with hysteresis . Non-recursive implementation.
    size_t c = 1;
    for (int j = 1; j < ny - 1; j++)
        for (int i = 1; i < nx - 1; i++) {
            if (nms[c] >= tmax && out.data()[c] == 0) { // trace edges
                //out.data()[c] = MAX_BRIGHTNESS;
                out.data()[c] = sti::pixel_max<T>();
                int nedges = 1;
                edges[0] = c;
 
                do {
                    nedges--;
                    const int t = edges[nedges];
 
                    int nbs[8]; // neighbours
                    nbs[0] = t - nx;     // nn
                    nbs[1] = t + nx;     // ss
                    nbs[2] = t + 1;      // ww
                    nbs[3] = t - 1;      // ee
                    nbs[4] = nbs[0] + 1; // nw
                    nbs[5] = nbs[0] - 1; // ne
                    nbs[6] = nbs[1] + 1; // sw
                    nbs[7] = nbs[1] - 1; // se
 
                    for (int k = 0; k < 8; k++)
                        if (nms[nbs[k]] >= tmin && out.data()[nbs[k]] == 0) {
                            out.data()[nbs[k]] = sti::pixel_max<T>();
                            edges[nedges] = nbs[k];
                            nedges++;
                        }
                } while (nedges > 0);
            }
            c++;
        }
 
    free(after_Gx);
    free(after_Gy);
    free(G);
    free(nms);
}

} // namespace canny
} // namespace sti