#include "edge_canny.h"

#include "blur_gaussian.h"
#include "conversion_grayscale.h"
#include "edge_sobel.h"

EdgeCanny::EdgeCanny(PNM* img) :
    Convolution(img)
{
}

EdgeCanny::EdgeCanny(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* EdgeCanny::transform()
{
    int width, height;
    int upper_thresh, lower_thresh;
    ConversionGrayscale* cg;
    PNM* newImage;
    BlurGaussian* bg;
    width = image->width();
    height = image->height();

    upper_thresh = getParameter("upper_threshold").toInt();
    lower_thresh = getParameter("lower_threshold").toInt();

    cg = new ConversionGrayscale(this->image);
    newImage = cg->transform();
    bg = new BlurGaussian(this->image);
    bg->setParameter("size", 3);

    qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    return newImage;
}
