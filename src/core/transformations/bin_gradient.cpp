#include "bin_gradient.h"
#include "bin_manual.h"
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
    int width, height;
    PNM* newImage;
    int suma, licznik, threshold;
    int w, h;
    BinarizationManual* bm;
    int vg1, vg2,vmax;

    width = image->width();
    height = image->height();

    newImage = new PNM(width, height, QImage::Format_Mono);

    suma = 0, licznik = 0;

    for (w = 0; w < width; w++)
    {
        for (h = 0; h < height; h++)
        {
            vg1 = this->g1(w, h);
            vg2 = this->g2(w, h);
            vmax = std::max(vg1, vg2);
            suma += vmax;
            licznik += vmax * qGray(this->image->pixel(w, h));
        }
    }
    threshold = licznik/suma;

    bm = new BinarizationManual(this->image);
    bm->setParameter("threshold", threshold);
    newImage = bm->transform();

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

