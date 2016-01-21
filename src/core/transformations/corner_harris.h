#ifndef CORNER_HARRIS_H
#define CORNER_HARRIS_H

#include "convolution.h"

class CornerHarris : public Convolution
{
public:
    CornerHarris(PNM*);
    CornerHarris(PNM*, ImageViewer*);

    PNM* transform();
private:
    math::matrix<float> I_xx, I_xy, I_yy, candids, nonmax_sup;
    int    threshold;
    double sigma, sigma_weight, k_param;

    void fill_matrices(int w, int h);
    PNM* toGrayScale(PNM* img);
    PNM* toBlurGaussian(PNM* img);
    void complete_i_matrices(PNM* img, int width, int height);

    void calculate_for_pixel(int i, int j);
    void calculate_gauss(int i, int j, float& Sxx, float& Sxy, float& Syy);
    float fill_H_matrix(float Sxx, float Sxy, float Syy);
};

#endif // CORNER_HARRIS_H
