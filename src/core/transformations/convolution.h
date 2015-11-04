#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include "transformation.h"
#include "matrix/cmatrix"

class Convolution : public Transformation
{
public:
    Convolution(PNM*);
    Convolution(PNM*, ImageViewer*);

    virtual math::matrix<float> getMask(int, Mode);
    virtual PNM*                convolute(math::matrix<float>, Mode);
    virtual PNM*                transform();

protected:
    const math::matrix<float> join(math::matrix<float>, math::matrix<float>);
    const float               sum(math::matrix<float>);
    const math::matrix<float> reflection(math::matrix<float>);
    const void iteratePixelsByChannel(PNM*,int, Channel, Mode, float);
    const void convulatePixel(PNM*, int, int, int, Channel, Mode, float);
};

#endif // CONVOLUTION_H
