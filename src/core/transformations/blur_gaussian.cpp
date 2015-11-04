#include "blur_gaussian.h"

BlurGaussian::BlurGaussian(PNM* img) :
    Convolution(img)
{
}

BlurGaussian::BlurGaussian(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* BlurGaussian::transform()
{
    emit message("Blurring...");

    int size = getParameter("size").toInt();
    radius = (size/2)+1;
    sigma = getParameter("sigma").toDouble();

    return convolute(getMask(size, Normalize), RepeatEdge);
}

math::matrix<float> BlurGaussian::getMask(int size, Mode mode)
{
    math::matrix<float> mask(size, size);
    int sri, sci, dri, dci;
    for (dri = 0, sri = size-(size+1); dri < size; dri++, sri++)
    {
        for (dci = 0, sci = size-(size+1); dci < size; dci++, sci++)
        {
            mask[dri][dci] = this->getGauss(sri, sci, this->sigma);
        }
    }

    return mask;
}

float BlurGaussian::getGauss(int x, int y, float sigma)
{
    float mian1, licz2, mian2, ulam2, valExp, result;

    mian1 = 2*M_PI*sigma*sigma;
    licz2 = x*x+y*y;
    mian2 = 2*sigma*sigma;
    ulam2 = -licz2/mian2;
    valExp = exp(ulam2);
    result = 1/mian1*valExp;

    return result;
}

