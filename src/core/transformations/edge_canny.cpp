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

double EdgeCanny::grad_pow(int i, int j)
{
    double vg = (*v)[i][j];
    double hg = (*h)[i][j];
    return std::sqrt(vg*vg+hg*hg);
}

double EdgeCanny::grad_dir(int i, int j)
{
    double vg = (*v)[i][j];
    double hg = (*h)[i][j];
    return atan(vg/hg);
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

void EdgeCanny::classifyPixels()
{
    int width = newImage->width();
    int height = newImage->height();
    int i, j;
    double pg, dg, ng;
    for (i = 0; i < width; i++)
    {
        for (j = 0; j < height; j++)
        {
            pg = grad_pow(i, j);
            dg = grad_dir(i, j);
            if (in_range(22.5, dg, 67.5))
            {
                ng = grad_pow(i-1, j-1)+grad_pow(i+1, j+1);
            }
            else if (in_range(67.5, dg, 112.5))
            {
                ng = grad_pow(i, j-1)+grad_pow(i, j+1);
            }
            else if (in_range(112.5, dg, 157.5))
            {
                ng = grad_pow(i+1, j-1)+grad_pow(i-1, j+1);
            }
            else
            {
                ng = grad_pow(i-1, j)+grad_pow(i+1, j);
            }
            if (pg > ng && pg > upper_thresh)
            {
                edgesList->append(QPoint(i, j));
            }
        }
    }
}

PNM* EdgeCanny::thresholding_with_histeresis()
{
    for( int i=0; i<edgesList->count(); ++i )
    {
        QPoint p = ise->at(i);
        int px = p.x(), py = p.y();
        dg = grad_dir(px, py);
        if (in_range(22.5, dg, 67.5))
        {
            if (grad_pow(px-1, py-1) > lower_thresh)
            {
                if (grad_dir(px, py) == dg)
                {

                }
            }
            +grad_pow(px+1, py+1);
        }
        else if (in_range(67.5, dg, 112.5))
        {
            ng = grad_pow(px, py-1)+grad_pow(px, py+1);
        }
        else if (in_range(112.5, dg, 157.5))
        {
            ng = grad_pow(px+1, py-1)+grad_pow(px-1, py+1);
        }
        else
        {
            ng = grad_pow(px-1, py)+grad_pow(px+1, py);
        }
    }
}

PNM* EdgeCanny::transform()
{
    int upper_thresh, lower_thresh;

   int i, j;


    upper_thresh = getParameter("upper_threshold").toInt();
    lower_thresh = getParameter("lower_threshold").toInt();

    newImage = this->toGrayScale(this->image);
    newImage = this->toBlurGaussian(newImage);
    this->getGradients(newImage);
    this->classifyPixels();
    this->thresholding_with_histeresis();


    }

    delete cg;
    delete bg;
    delete es;

    return newImage;
}
