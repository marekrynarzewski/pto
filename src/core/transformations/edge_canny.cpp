#include "edge_canny.h"

#include "blur_gaussian.h"
#include "conversion_grayscale.h"
#include "edge_sobel.h"
#include <math.h>
#include "map_normal.h"

EdgeCanny::EdgeCanny(PNM* img) :
    Convolution(img)
{
}

EdgeCanny::EdgeCanny(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* EdgeCanny::toGrayScale(PNM* img)
{
    ConversionGrayscale* cg;
    PNM* result;

    cg = new ConversionGrayscale(img);
    result = cg->transform();

    return result;
}

PNM* EdgeCanny::toBlurGaussian(PNM* img)
{
    BlurGaussian* bg;
    PNM* result;

    bg = new BlurGaussian(img);
    bg->setParameter("size", 3);
    bg->setParameter("sigma", 1.6);
    result = bg->transform();

    return result;
}

void EdgeCanny::getGradients(PNM* img)
{
    EdgeSobel* es;

    es = new EdgeSobel(img);
    this->h = es->rawHorizontalDetection();
    this->v = es->rawVerticalDetection();
}

int getDirection(float d)
{
    float degrees = d*100;
    if ((degrees > 0 && degrees <= 22.5) || (degrees > 157.5 && degrees <= 180))
        return 0;
    if (degrees > 22.5 && degrees <= 67.5)
        return 1;
    if (degrees > 67.5 && degrees <= 112.5)
        return 2;
    if (degrees > 112.5 && degrees <= 157.5)
        return 3;

    return 0;
}

void EdgeCanny::classifyPixels()
{
    int width = newImage->width();
    int height = newImage->height();
    int i, j;
    for (i = 0; i < width; i++)
    {
        for (j = 0; j < height; j++)
        {
            this->newImage->setPixel(i, j, PIXEL_VAL_MIN);
            this->magnitude(i,j) = sqrt(pow((*this->h)(i,j),2) + pow((*this->v)(i,j),2));
            this->direction(i,j) = atan(abs((*this->v)(i,j))/abs((this->h)(i,j)));
        }
    }
}

PNM* EdgeCanny::classifyPixels()
{
    int i, j;
    for (i = 0; i < width; i++)
    {
        for (j = 0; j < height; j++)
        {

        }
    }
}


PNM* EdgeCanny::hysteresis()
{

}

PNM* EdgeCanny::transform()
{
    int upper_thresh, lower_thresh;

    upper_thresh = getParameter("upper_threshold").toInt();
    lower_thresh = getParameter("lower_threshold").toInt();

    newImage = this->toGrayScale(this->image);
    newImage = this->toBlurGaussian(newImage);
    this->getGradients(newImage);
    this->calcGradAndDir();
    this->classifyPixels();


    delete cg;
    delete bg;
    delete es;

    return newImage;
}
