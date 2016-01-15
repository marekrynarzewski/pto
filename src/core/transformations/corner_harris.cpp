#include "corner_harris.h"

#include "blur_gaussian.h"
#include "conversion_grayscale.h"
#include "edge_sobel.h"

CornerHarris::CornerHarris(PNM* img) :
    Convolution(img)
{
    this->H = math::matrix<float>(2, 2);
}

CornerHarris::CornerHarris(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
    this->H = math::matrix<float>(2, 2);
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

 void CornerHarris::complete_i_matrices(int width, int height)
 {
     int i, j;
     for (i = 0; i < width; i++)
     {
         for (j = 0; j < height; j++)
         {
             this->I_xx(i, j) = (*this->Gx)(i, j)*(*this->Gx)(i, j);
             this->I_yy(i, j) = (*this->Gy)(i, j)*(*this->Gy)(i, j);
             this->I_xy(i, j) = (*this->Gx)(i, j)*(*this->Gy)(i, j);
         }
     }
 }

 void CornerHarris::calculate_gauss(int i, int j)
 {
     int k, l;
     float gauss;
     for (k = -1; k <= 1; k++)
     {
         for (l = -1; l <= 1; l++)
         {
             gauss = BlurGaussian::getGauss(k,l,this->sigma);
             this->Sxx = this->Sxx + this->I_xx(i+k, j+l)*gauss;
             this->Syy = this->Syy + this->I_yy(i+k, j+l)*gauss;
             this->Sxy = this->Sxy + this->I_xy(i+k, j+l)*gauss;
         }
     }
 }

 void CornerHarris::divide_s_by_sigma_weight()
 {
     this->Sxx = this->Sxx/this->sigma_weight;
     this->Syy = this->Syy/this->sigma_weight;
     this->Sxy = this->Sxy/this->sigma_weight;
 }

 float CornerHarris::fill_H_matrix()
 {
     float detH, trH, result;

     this->H(0,0) = this->Sxx;
     this->H(0,1) = this->H(1,0) = this->Sxy;
     this->H(1,1) = this->Syy;
     detH = this->H.det();
     trH = this->H.trace();
     result = detH - this->k_param * pow((double) trH, 2.0);

     return result;
 }

 void CornerHarris::calculate_for_pixel(int i, int j)
 {
     float r;
     this->calculate_gauss(i, j);
     this->divide_s_by_sigma_weight();
     r = this->fill_H_matrix();
     if (r > this->threshold)
     {
         this->candids(i,j) = r;
     }
 }


PNM* CornerHarris::transform()
{
    this->threshold    = getParameter("threshold").toInt();
    this->sigma        = getParameter("sigma").toDouble();
    this->sigma_weight = getParameter("sigma_weight").toDouble();
    this->k_param      = getParameter("k").toDouble();

    int width  = image->width(),
        height = image->height();
    int i, j;

    PNM* img;


    PNM* newImage = new PNM(width, height, QImage::Format_Mono);

    this->fill_matrices(width, height);
    img = this->toGrayScale(this->image);
    this->getGradients(img);
    this->complete_i_matrices(width, height);
    for (i = 1; i < width - 1; i++)
    {
           for (j = 1; j < height - 1; j++)
           {
               this->calculate_for_pixel(i, j);
           }
    }

    return newImage;
}

