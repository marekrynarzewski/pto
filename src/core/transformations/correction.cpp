#include "correction.h"

Correction::Correction(PNM* img) :
    Transformation(img)
{
}

Correction::Correction(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* Correction::transform()
{
    float shift  = getParameter("shift").toFloat();
    float factor = getParameter("factor").toFloat();
    float gamma  = getParameter("gamma").toFloat();

    int width  = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, image->format());

    for (int i = PIXEL_VAL_MIN; i <= PIXEL_VAL_MAX; i++)
    {
        LUT[i] = i;
    }

    for (int x=0; x<width; x++)
    {
        for (int y=0; y<height; y++)
        {
            QRgb pixel = image->pixel(x,y); // Getting the pixel(x,y) value

            int r = qRed(pixel);    // Get the 0-255 value of the R channel
            int g = qGreen(pixel);  // Get the 0-255 value of the G channel
            int b = qBlue(pixel);
        }
    }

    return newImage;
}
