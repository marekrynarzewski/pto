#include "map_horizon.h"

#include "map_height.h"
#include "map_normal.h"

MapHorizon::MapHorizon(PNM* img) :
    Transformation(img)
{
}

MapHorizon::MapHorizon(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

double distance(int x1, int y1, int x2, int y2)
{
    int dif1, dif2;
    int pd1, pd2;
    int sum = 0;
    double result;

    dif1 = y1 - x1;
    dif2 = y2 - x2;
    pd1 = dif1 * dif1;
    pd2 = dif2 * dif2;
    sum = pd1 + pd2;
    result = std::sqrt(sum);

    return result;
}


PNM* MapHorizon::transform()
{
    MapHeight *mh;
    PNM* tmpImg;
    int width  = image->width(),
        height = image->height();

    double scale     = getParameter("scale").toDouble();
    int    sun_alpha = getParameter("alpha").toInt();
    int dx, dy;

    switch (getParameter("direction").toInt())
    {
    case NORTH: dx = 0; dy = -1; break;
    case SOUTH: dx = 0; dy = 1; break;
    case EAST:  dx = 1; dy = 0; break;
    case WEST:  dx = -1; dy = 0; break;
    default:
        qWarning() << "Unknown direction!";
        dx =  0;
        dy = -1;
    }

    PNM* newImage = new PNM(width, height, QImage::Format_Indexed8);

    mh = new MapHeight(this->image);
    tmpImg = mh->transform();
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            double alpha = 0;
            double current_h = qGray(tmpImg->pixel(i, j));
            for (int l = i+dx; ((dx > 0)?(l < width):(l >= 0)); ((dx > 0)?(l++):(l--)))
            {
                for (int k = j+dy; ((dy > 0)?(k < height):(k >= 0)); ((dy > 0)?(k++):(k--)))
                {
                    int ray_h = qGray(tmpImg->pixel(l, k));
                    if (current_h < ray_h)
                    {
                        double dist = distance(i, j, k, l)*scale;
                        double ray_a = std::tan((ray_h - current_h)/dist);
                        if (ray_a > alpha)
                        {
                            alpha = ray_a;
                        }
                    }
                }
            }
            double delta = alpha - sun_alpha * M_PI / 180;
           // qDebug() << delta;
            if (delta > 0)
            {
                if (in_range(0.0, delta, 255.0))
                {
                    newImage->setPixel(i, j, std::cos(delta)*255);
                }
                else
                {
                    qDebug() << delta << std::cos(delta);
                }
            }
            else
            {
                newImage->setPixel(i, j, 255);
            }
        }
    }

    return newImage;
}

