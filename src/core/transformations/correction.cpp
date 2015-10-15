#include "correction.h"
#include <math.h>
#include <algorithm>
using namespace std;
Correction::Correction(PNM* img) :
    Transformation(img)
{
}

Correction::Correction(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

template <typename T>
T range(T min, T val, T max)
{
    T result;
    if (val < min)
    {
        result = min;
    }
    else if (min < val && val < max)
    {
        result = val;
    }
    else
    {
        result = max;
    }
    return result;
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
        LUT[i] = range((float)0, i+shift, (float)255);
        LUT[i] = range((float)0,  LUT[i]*factor, (float)255);
        LUT[i] = range((float)0,  (float)pow(LUT[i], gamma), (float)255);
    }

    for (int x=0; x<width; x++)
    {
        for (int y=0; y<height; y++)
        {
            QRgb pixel = image->pixel(x,y); // Getting the pixel(x,y) value

            int r = qRed(pixel);    // Get the 0-255 value of the R channel
            int g = qGreen(pixel);  // Get the 0-255 value of the G channel
            int b = qBlue(pixel);
			r = LUT[r];
			g = LUT[g];
			b = LUT[b];
			QColor newPixel = QColor(r,g,b);
            newImage->setPixel(x,y, newPixel.rgb());
        }
    }

    return newImage;
}
