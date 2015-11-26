#include "edge_zero.h"

#include "edge_laplacian_of_gauss.h"

EdgeZeroCrossing::EdgeZeroCrossing(PNM* img) :
    Convolution(img)
{
}

EdgeZeroCrossing::EdgeZeroCrossing(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* EdgeZeroCrossing::transform()
{
    int width = image->width(),
        height = image->height();
    math::matrix<float> windowR, windowG, windowB, windowL;
    float min, max;
    int    size  = getParameter("size").toInt();
    double sigma = getParameter("sigma").toDouble();
    int    t     = getParameter("threshold").toInt();

    PNM* newImage = new PNM(width, height, QImage::Format_Indexed8);
    EdgeLaplaceOfGauss* elog = new EdgeLaplaceOfGauss(this->image, this->supervisor);
    math::matrix<float> mask = elog->getMask(size, RepeatEdge);
    this->image = this->convolute(mask, RepeatEdge);
    int v_0 = 128;
    for (int i = 0; i < this->image->width(); i++)
    {
        for (int j = 0; j < this->image->height(); j++)
        {
            windowR = this->getWindow(i, j, size, RChannel, RepeatEdge);
            //windowG = this->getWindow(i, j, size, GChannel, RepeatEdge);
            //windowB = this->getWindow(i, j, size, BChannel, RepeatEdge);
            //windowL = this->getWindow(i, j, size, LChannel, RepeatEdge);
            max = windowR.max();
            min = windowR.min();
            if (min < (v_0 - t) && max > (v_0 + t))
            {
                newImage->setPixel(i, j, qGray(this->image->pixel(i, j)));
            }
            else
            {
                newImage->setPixel(i, j, 0);
            }
        }
    }

    return newImage;
}

