#include "noise_median.h"

NoiseMedian::NoiseMedian(PNM* img) :
    Convolution(img)
{
}

NoiseMedian::NoiseMedian(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

template<typename T>
QString toString(T array[], int size)
{
    QString ss;
    for(size_t i = 0; i < size; ++i)
    {
      if(i != 0)
        ss.append(",");
      ss.append(QString::number(array[i]));
    }
    return ss;
}

PNM* NoiseMedian::transform()
{
    int width  = image->width();
    int height = image->height();
    int i, j;
    PNM* newImage = new PNM(width, height, image->format());

    for (i = 0; i < width; i++)
    {
        for (j = 0; j < height; j++)
        {
            if (image->format() == QImage::Format_Indexed8)
            {
                newImage->setPixel(i, j, this->getMedian(i, j, LChannel));
            }
            else
            {
                int r = this->getMedian(i, j, RChannel);
                int g = this->getMedian(i, j, GChannel);
                int b = this->getMedian(i, j, BChannel);
                QColor color = QColor(r, g, b);
                newImage->setPixel(i, j, color.rgb());
            }
        }
    }

    return newImage;
}

int NoiseMedian::getMedian(int x, int y, Channel channel)
{
    int radius = getParameter("radius").toInt();
    int size = 2 *radius+1;
    int powSize = size*size;
    int window[powSize];
    int sr, sc, lp = 0;
    for (sr = x-radius; sr <= x+radius; sr++)
    {
        for (sc = y-radius; sc <= y+radius; sc++)
        {
            QRgb color = this->getPixel(sr, sc, CyclicEdge);
            float value;
            switch(channel)
            {
                case LChannel: value = qGray (color);  break;
                case RChannel: value = qRed  (color); break;
                case GChannel: value = qGreen(color); break;
                case BChannel: value = qBlue (color); break;
            }
            window[lp++] = value;
        }
    }
    //qDebug() <<toString(window, powSize);
    std::sort(window, window+powSize);
    //sqDebug() << toString(window, powSize);
    int center = powSize/2;
    int result =  window[center];
    //qDebug() << result;
    return result;
}



