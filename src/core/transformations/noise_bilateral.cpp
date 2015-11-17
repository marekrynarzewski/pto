#include "noise_bilateral.h"

NoiseBilateral::NoiseBilateral(PNM* img) :
    Convolution(img)
{
}

NoiseBilateral::NoiseBilateral(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* NoiseBilateral::transform()
{
    int width  = image->width();
    int height = image->height();
    int i, j;
    PNM* newImage = new PNM(width, height, image->format());

    sigma_d = getParameter("sigma_d").toInt();
    sigma_r = getParameter("sigma_r").toInt();
    radius = sigma_d;

    for (i = 0; i < width; i++)
    {
        for (j = 0; j < height; j++)
        {
            if (image->format() == QImage::Format_Indexed8)
            {
                newImage->setPixel(i, j,this->calcVal(i, j, LChannel));
            }
            else
            {
               int r = this->calcVal(i, j, RChannel);
               int g = this->calcVal(i, j, GChannel);
               int b = this->calcVal(i, j, BChannel);
               QColor color = QColor(r, g, b);
               newImage->setPixel(i, j, color.rgb());
            }
        }
    }

    return newImage;
}

int NoiseBilateral::calcVal(int x, int y, Channel channel)
{
    double sum;
    int i, j;
    int I_ij, I_xy;
    double cc, sc;
    QPoint P_ij, P_xy;
    double mul, mul2, numer;
    math::matrix<float> window;
    int result;
    int srednica;

    srednica = 2 * this->radius + 1;

    window = this->getWindow(x, y, srednica, channel, RepeatEdge);
    sum = 0.0;
    numer = 0.0;
    I_xy = window[this->radius][this->radius];
    P_xy = QPoint(this->radius, this->radius);
    for (i = 0; i < srednica; i++)
    {
        for (j = 0; j < srednica; j++)
        {
            I_ij = window[i][j];
            cc = this->colorCloseness(I_ij, I_xy);
            P_ij = QPoint(i, j);
            sc = this->spatialCloseness(P_ij, P_xy);
            mul = cc * sc;
            mul2 = mul * I_ij;
            sum += mul;
            numer += mul2;
        }
    }

    result = numer/sum;
    return std::max(0, std::min(255, result));
}

float NoiseBilateral::colorCloseness(int val1, int val2)
{
    int dif, pw, denom;

    dif = val1 - val2;
    pw = dif*dif;
    denom = 2*this->sigma_r*this->sigma_r;

    return exp(-pw/denom);
}

float NoiseBilateral::spatialCloseness(QPoint point1, QPoint point2)
{
    int x1, y1, x2, y2;
    int dx, dy;
    int px, py;
    int sum;
    int denom;

    x1 = point1.x();
    x2 = point2.x();
    y1 = point1.y();
    y2 = point2.y();
    dx = math::abs(x1 - x2);
    dy = math::abs(y1 - y2);
    px = dx*dx;
    py = dy*dy;
    sum = px + py;
    denom = 2 * this->sigma_d*this->sigma_d;

    return exp(-(float)sum/(float)denom);
}
