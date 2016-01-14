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
    math::matrix<float> I_xx, I_xy, I_yy, candids, nonmax_sup, *Gx, *Gy;
    void fill_matrices(int w, int h);
    PNM* toGrayScale(PNM* img);
    PNM* toBlurGaussian(PNM* img);
    void getGradients(PNM* img);
};

#endif // CORNER_HARRIS_H
