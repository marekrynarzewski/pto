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

 void CornerHarris::complete_i_matrices(PNM* img, int width, int height)
 {
     EdgeSobel* es;
     math::matrix<float> *Gx, *Gy;

     es = new EdgeSobel(img);
     Gx = es->rawHorizontalDetection();
     Gy = es->rawVerticalDetection();
     int i, j;
     for (i = 0; i < width; i++)
     {
         for (j = 0; j < height; j++)
         {
             this->I_xx(i, j) = (*Gx)(i, j)*(*Gx)(i, j);
             this->I_yy(i, j) = (*Gy)(i, j)*(*Gy)(i, j);
             this->I_xy(i, j) = (*Gx)(i, j)*(*Gy)(i, j);
         }
     }
 }

 void CornerHarris::calculate_gauss(int i, int j, float& Sxx, float& Sxy, float& Syy)
 {
     int k, l;
     float gauss;
     Sxx = 0, Sxy = 0, Syy = 0;
     for (k = -1; k <= 1; k++)
     {
         for (l = -1; l <= 1; l++)
         {
             gauss = BlurGaussian::getGauss(k,l,this->sigma);
             Sxx += this->I_xx(i+k, j+l)*gauss;
             Syy += this->I_yy(i+k, j+l)*gauss;
             Sxy += this->I_xy(i+k, j+l)*gauss;

         }
     }
     Sxx /= this->sigma_weight;
     Syy /= this->sigma_weight;
     Sxy /= this->sigma_weight;
 }

 float CornerHarris::fill_H_matrix(float Sxx, float Sxy, float Syy)
 {
     math::matrix<float> H(2, 2);
     float detH, trH, result;

     H(0,0) = Sxx;
     H(0,1) = H(1,0) = Sxy;
     H(1,1) = Syy;
     detH = H.det();
     trH = H.trace();
     result = detH - this->k_param * pow((double) trH, 2.0);

     return result;
 }

 void CornerHarris::calculate_for_pixel(int i, int j)
 {
     float r, Sxx, Sxy, Syy;

     this->calculate_gauss(i, j, Sxx, Sxy, Syy);
     r = this->fill_H_matrix(Sxx, Sxy, Syy);
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
    bool search;


    PNM* newImage = new PNM(width, height, QImage::Format_Mono);

    this->fill_matrices(width, height);
    img = this->toGrayScale(this->image);
    this->complete_i_matrices(img, width, height);
    for (i = 1; i < width - 1; i++)
    {
           for (j = 1; j < height - 1; j++)
           {
               this->calculate_for_pixel(i, j);
           }
    }
    search = true;
    while (search)
    {
        search = false;
        for (i = 1; i < width - 1; i++)
        {
            for (j = 1; j < height - 1; j++)
            {
               if (this->candids(i, j) > this->candids(i-1, j-1)
                && this->candids(i, j) > this->candids(i-1, j)
                && this->candids(i, j) > this->candids(i-1, j+1)
                && this->candids(i, j) > this->candids(i,   j-1)
                && this->candids(i, j) > this->candids(i,   j+1)
                && this->candids(i, j) > this->candids(i+1, j-1)
                && this->candids(i, j) > this->candids(i+1, j)
                && this->candids(i, j) > this->candids(i+1, j+1))
               {
                   this->nonmax_sup(i, j) = this->candids(i, j);
               }
               else
               {
                   if (this->candids(i, j) > 0)
                   {
                        search = true;
                   }
                   this->nonmax_sup(i, j) = 0;
               }
            }
        }
        this->candids = this->nonmax_sup;
    }
    for (i = 0; i < width; i++)
    {
        for (j = 0; j < height; j++)
        {
            if (this->candids(i, j) == 0)
            {
                newImage->setPixel(i, j, Qt::color0);
            }
            else
            {
                 newImage->setPixel(i, j, Qt::color1);
            }
        }
    }

    return newImage;
}

