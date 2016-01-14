#include "corner_harris.h"

#include "blur_gaussian.h"
#include "conversion_grayscale.h"
#include "edge_sobel.h"

CornerHarris::CornerHarris(PNM* img) :
    Convolution(img)
{
}

CornerHarris::CornerHarris(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

 void CornerHarris::fill_matrices(int width, int height)
 {
    I_xx = math::matrix<float>(width, height);
    I_xy = math::matrix<float>(width, height);
    I_yy = math::matrix<float>(width, height);
    candids = math::matrix<float>(width, height);
    nonmax_sup = math::matrix<float>(width, height);
 }

 PNM* CornerHarris::toGrayScale(PNM* img)
 {
     ConversionGrayscale* cg;
     PNM* result;

     cg = new ConversionGrayscale(img);
     result = cg->transform();

     return result;
 }

 PNM* CornerHarris::toBlurGaussian(PNM* img)
 {
     BlurGaussian* bg;
     PNM* result;

     bg = new BlurGaussian(img);
     bg->setParameter("size", 3);
     bg->setParameter("sigma", 1.6);
     result = bg->transform();

     return result;
 }

 void CornerHarris::getGradients(PNM* img)
 {
     EdgeSobel* es;

     es = new EdgeSobel(img);
     this->Gx = es->rawHorizontalDetection();
     this->Gy = es->rawVerticalDetection();
 }

PNM* CornerHarris::transform()
{
    int    threshold    = getParameter("threshold").toInt();
    double sigma        = getParameter("sigma").toDouble(),
           sigma_weight = getParameter("sigma_weight").toDouble(),
           k_param      = getParameter("k").toDouble();

    int width  = image->width(),
        height = image->height();
    PNM* img;

    PNM* newImage = new PNM(width, height, QImage::Format_Mono);

    this->fill_matrices(width, height);
    img = this->toGrayScale(this->image);
    this->getGradients(img);
    return newImage;
}
