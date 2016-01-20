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
    math::matrix<float> I_xx, I_xy, I_yy, candids, nonmax_sup, *Gx, *Gy, H;
    int    threshold;
    double sigma, sigma_weight, k_param;
    void fill_matrices(int w, int h);
    PNM* toGrayScale(PNM* img);
    PNM* toBlurGaussian(PNM* img);
    void getGradients(PNM* img);
    void complete_i_matrices(int width, int height);
<<<<<<< HEAD
    float Sxx = 0, Syy = 0, Sxy = 0;
    void calculate_for_pixel(int i, int j);
    void calculate_gauss(int i, int j);
    float fill_H_matrix();
    void divide_s_by_sigma_weight();
=======
>>>>>>> 7ab0f80f63e7fd048822150824e4e79ed8782d37
};

#endif // CORNER_HARRIS_H
