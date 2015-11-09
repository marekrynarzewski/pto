#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include "transformation.h"
#include "matrix/cmatrix"
#include "math.h"

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
    const void iteratePixelsByChannel(Channel, Mode);
    const int convolutePixel(int, int, Channel, Mode);
    math::matrix<float> mask;
    int maskSize;
    float maskWeight;
};

#endif // CONVOLUTION_H
