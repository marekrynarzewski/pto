#include "bin_gradient.h"

using namespace std;
BinarizationGradient::BinarizationGradient(PNM* img) :
    Transformation(img)
{
}

BinarizationGradient::BinarizationGradient(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* BinarizationGradient::transform()
{
    int width = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, QImage::Format_Mono);

    int suma = 0, licznik = 0, threshold;

    for (int w = 0; w < width; w++)
    {
        for (int h = 0; h < height; h++)
        {
           suma += max(this->g1(w, h), this->g2(w, h));
           licznik += suma * qGray(this->image->pixel(w, h));
        }
    }
    qDebug() << licznik;
    qDebug() << suma;
    threshold = licznik/suma;
    qDebug() << threshold;

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

int BinarizationGradient::g1(int x, int y)
{
    QRgb px, py;
    int vx, vy, result;

    px = this->getPixel(x+1, y, RepeatEdge);
    py = this->getPixel(x-1, y, RepeatEdge);
    vx = qGray(px);
    vy = qGray(py);
    result = vx - vy;

    return result;
}

int BinarizationGradient::g2(int x, int y)
{
    QRgb px, py;
    int vx, vy, result;

    px = this->getPixel(x, y+1, RepeatEdge);
    py = this->getPixel(x, y-1, RepeatEdge);
    vx = qGray(px);
    vy = qGray(py);
    result = vx - vy;

    return result;
}

