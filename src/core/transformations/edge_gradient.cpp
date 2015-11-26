#include "edge_gradient.h"

EdgeGradient::EdgeGradient(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

EdgeGradient::EdgeGradient(PNM* img) :
    Convolution(img)
{
}

PNM* EdgeGradient::verticalDetection()
{
    return convolute(g_y, RepeatEdge);
}

PNM* EdgeGradient::horizontalDetection()
{
    return convolute(g_x, RepeatEdge);
}

PNM* EdgeGradient::transform()
{
    int width = image->width();
    int height = image->height();
    PNM* newImage = new PNM(width, height, image->format());

    PNM* img_x = this->horizontalDetection();
    PNM* img_y = this->verticalDetection();


    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            if (image->format() == QImage::Format_Indexed8)
            {
                int l = this->calcValue(i, j, LChannel, img_x, img_y);
                newImage->setPixel(i, j, l);
            }
            else
            {
                int r = this->calcValue(i, j, RChannel, img_x, img_y);
                int g = this->calcValue(i, j, GChannel, img_x, img_y);
                int b = this->calcValue(i, j, BChannel, img_x, img_y);
                QColor color = QColor(r, g, b);
                newImage->setPixel(i, j, color.rgb());
            }
        }
    }

    return newImage;
}

int EdgeGradient::calcValue(int i, int j, Channel channel, PNM* img_x, PNM* img_y)
{
    int value_x, value_y;
    double power_x, power_y;
    double sum;
    int result;

    value_x = this->getValue(img_x, i, j, channel);
    value_y = this->getValue(img_y, i, j, channel);
    power_x = value_x * value_x;
    power_y = value_y * value_y;
    sum = power_x + power_y;
    result = (int) sqrt(sum);
    result = std::max(0, std::min(255, result));
    //qDebug() << result;

    return result;

}

int EdgeGradient::getValue(PNM* img, int i, int j, Channel channel)
{
    QRgb pixel;
    int value;

    pixel = img->pixel(i, j);
    switch(channel)
    {
        case RChannel:
            value = qRed(pixel);
        break;
        case GChannel:
            value = qGreen(pixel);
        break;
        case BChannel:
            value = qBlue(pixel);
        break;
        case LChannel:
            value = qGray(pixel);
        break;
    }

   return value;

}

