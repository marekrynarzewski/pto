#include "map_normal.h"

#include "edge_sobel.h"
#include "map_height.h"

template <typename T>
bool in_range(T min, T x, T max)
{
    bool b1 = (x >= min);
    bool b2 = (x <= max);
    bool result = b1 && b2;
    return result;
}

template <typename T>
bool in_range3(T min, T a, T b, T c, T max)
{
    bool b1 = in_range(min, a, max);
    bool b2 = in_range(min, b, max);
    bool b3 = in_range(min, c, max);
    bool result = b1 && b2 && b3;
    return result;
}


MapNormal::MapNormal(PNM* img) :
    Convolution(img)
{
}

MapNormal::MapNormal(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* MapNormal::transform()
{
    MapHeight* mh;
    EdgeSobel* es;
    PNM* tmpImg;
    math::matrix<float> *gkx, *gky;
    int width  = image->width(),
        height = image->height();
    double dx, dy, dz;
    double norm;
    double strength = getParameter("strength").toDouble();

    PNM* newImage = new PNM(width, height, image->format());

    mh = new MapHeight(this->image);
    tmpImg = mh->transform();
    es = new EdgeSobel(tmpImg);
    gkx = es->rawHorizontalDetection();
    gky = es->rawVerticalDetection();
    for (int i = 0; i < this->image->width(); i++)
    {
        for (int j = 0; j < this->image->height(); j++)
        {
            qDebug() << (*gkx)[i][j];
            qDebug() << (*gky)[i][j];
            dx = (double)(*gkx)[i][j]/255.0;
            dy = (double)(*gky)[i][j]/255.0;
            dz = 1.0/strength;
            //qDebug () << dx << dy << dz;
            norm = 1/(std::sqrt(dx*dx+dy*dy+dz*dz));
            dx = dx/norm;
            dy = dy/norm;
            dz = dz/norm;
            //qDebug () << dx << dy << dz;
            dx = std::abs(((dx + 1.0)/2.0)*255.0);
            dy = std::abs(((dy + 1.0)/2.0)*255.0);
            dz = std::abs(((dz + 1.0)/2.0)*255.0);
            if (in_range3(0.0, dx, dy, dz, 255.0))
            {

                QColor color = QColor((int)dx, (int)dy, (int)dz);
                newImage->setPixel(i, j, color.rgb());
            }
            else
            {
                //qDebug () << dx << dy << dz;
            }


        }
    }
    return newImage;
}
