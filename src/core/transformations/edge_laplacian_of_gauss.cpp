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

math::matrix<float> EdgeLaplaceOfGauss::getMask(int size, Mode mode)
{
    //size = getParameter("size").toInt();
    double sigma = getParameter("sigma").toDouble();
    math::matrix<float> mask(size, size);
    qDebug() << size;
    QString s = "";
    int i, j, k, l;
    for (i = 0, k = -size/2; i < size; i++, k++)
    {
        QString ln = "";
        for (j = 0, l = -size/2; j < size; j++, l++)
        {
            mask(i, j) = this->getLoG(k, l, sigma);
            ln += QString::number(mask(i, j));
        }
        s += ln+"\n";
    }
    qDebug() << s;

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
