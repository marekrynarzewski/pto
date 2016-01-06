#ifndef EDGECANNY_H
#define EDGECANNY_H

#include "convolution.h"

class EdgeCanny : public Convolution
{
public:
    EdgeCanny(PNM*);
    EdgeCanny(PNM*, ImageViewer*);

    virtual PNM* transform();
private:
    PNM* toGrayScale(PNM*);
    PNM* toBlurGaussian(PNM*);
    void getGradients(int, int);
    QList<QPoint> initialEdges;
    math::matrix<float> magnitude;
    math::matrix<float> direction;
    void calcGradAndDir();
    void classifyPixels();
    bool hysteresis(int, int, int, int, int);
    PNM* newImage;
    math::matrix<float> *Gx, *Gy;
    int upper_thresh, lower_thresh;
};

#endif // EDGECANNY_H
