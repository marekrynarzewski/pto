#ifndef EDGECANNY_H
#define EDGECANNY_H

#include "convolution.h"

class EdgeCanny : public Convolution
{
public:
    EdgeCanny(PNM*);
    EdgeCanny(PNM*, ImageViewer*);

    virtual PNM* transform();
    double grad_pow(int, int);
    double grad_dir(int, int);
    PNM* toGrayScale(PNM*);
    PNM* toBlurGaussian(PNM*);
    void getGradients(PNM*);
    QList<QPoint> edgesList;
    void classifyPixels();
    void thresholding_with_histeresis();



private:
    math::matrix<float> *v, *h;
};

#endif // EDGECANNY_H
