#include "bin_manual.h"

BinarizationManual::BinarizationManual(PNM* img) :
    Transformation(img)
{
}

BinarizationManual::BinarizationManual(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* BinarizationManual::transform()
{
    int threshold = getParameter("threshold").toInt();


    int width  = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, QImage::Format_Mono);

    for (int w = 0; w < width; w++)
    {
        for (int h = 0; h < height; h++)
        {
            QRgb pxl = this->image->pixel(w, h);
            int val = qGray(pxl);
            newImage->setPixel(w, h, ((val > threshold)?(1):(0)));
        }
    }

    return newImage;
}




