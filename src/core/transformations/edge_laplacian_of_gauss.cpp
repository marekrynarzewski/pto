#include "edge_laplacian_of_gauss.h"

#include "blur_gaussian.h"

EdgeLaplaceOfGauss::EdgeLaplaceOfGauss(PNM* img) :
    Convolution(img)
{
}

EdgeLaplaceOfGauss::EdgeLaplaceOfGauss(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

math::matrix<float> EdgeLaplaceOfGauss::getMask(int, Mode)
{
    size = getParameter("size").toInt();
    double sigma = getParameter("sigma").toDouble();
    math::matrix<float> mask(size, size);
    int radius = (size/2)+1;
    int i, j;
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            mask(i, j) = this->getLoG(i/radius, j/radius, sigma);
        }
    }

    return mask;
}

float EdgeLaplaceOfGauss::getLoG(int x, int y, float s)
{
    int px, py;
    float ps;
    int numer;
    int gauss;
    int result;

    px = x * x;
    py = y * y;
    ps = s * s;
    numer = px + py - 2;
    gauss = BlurGaussian::getGauss(x, y, s);
    result = (numer/ps) * gauss;

    return result;
}

int EdgeLaplaceOfGauss::getSize()
{
    return size;
}
