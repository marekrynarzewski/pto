#include "hough_rectangles.h"
#include "hough_lines.h"
#include "corner_harris.h"

HoughRectangles::HoughRectangles(PNM* img) :
    Transformation(img)
{
}

HoughRectangles::HoughRectangles(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* HoughRectangles::detect_corners(PNM* img)
{
    CornerHarris * ch;
    PNM* result;

    ch = new CornerHarris(img);
    ch->setParameter("threshold", 30000000);
    ch->setParameter("sigma", 1.0);
    ch->setParameter("sigma_weight", 0.78);
    ch->setParameter("k_param", 0.05);
    result = ch->transform();

    return result;
}

PNM* HoughRectangles::transform()
{
    PNM *img, *result;
    int width, height;
    QImage::Format f;

    width = this->image->width();
    height = this->image->height();
    f = this->image->format();
    result = new PNM(width, height, f);
    img = this->detect_corners(this->image);

    return result;
}

