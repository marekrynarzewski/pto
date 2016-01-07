#include "hough.h"

#include "conversion_grayscale.h"
#include "edge_laplacian.h"

Hough::Hough(PNM* img) :
    Transformation(img)
{
}

Hough::Hough(PNM* img, ImageViewer* super) :
    Transformation(img, super)
{
}

PNM* Hough::toGrayScale(PNM* img)
{
    ConversionGrayscale* cg;
    PNM* result;

    cg = new ConversionGrayscale(img);
    result = cg->transform();

    return result;
}

math::matrix<int> Hough::zero(int width, int height)
{
    math::matrix<int> result(width, height);

    result = this->matrix(width, height, 0);

    return result;
}

template<typename T>
void Hough::normalize(T min, T max, math::matrix<T>& m, T n_min, T n_max)
{
    T value, licz, mian, ulam, wynik;
    for (int i = 0; i < m.rowno(); i++)
    {
        for (int j = 0; j < m.colno(); j++)
        {
            value = m(i, j);
            licz = value - min;
            mian = max - min;
            ulam = licz/mian;
            wynik = ulam*(n_max-n_min)+n_min;
            m(i, j) = wynik;
        }
    }
}

template<typename T>
math::matrix<T> Hough::matrix(int width, int height, T value)
{
    math::matrix<T> result(width, height);

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            result(i, j) = value;
        }
    }

    return result;
}

PNM* Hough::transform()
{   
    PNM *gsImg, *result;
    int thetaDensity, theta_size, width, height;
    int i, j, k, theta, ro, newWidth, newHeight;
    int max, ro_max, value;
    EdgeLaplacian* el;
    math::matrix<int> hough;
    QRgb pixel;

    thetaDensity = getParameter("theta_density").toInt();
    gsImg = this->toGrayScale(this->image);
    if (this->getParameter("skip_edge_detection").toBool() == false)
    {
        el = new EdgeLaplacian(gsImg);
        gsImg = el->transform();
    }
    width = gsImg->width();
    height = gsImg->height();
    ro_max = std::sqrt(std::pow(width, 2)+std::pow(height, 2));
    theta_size = 180 * thetaDensity;
    newWidth = theta_size;
    newHeight = ro_max*2+1;
    result = new PNM(newWidth, newHeight, QImage::Format_Indexed8);
    hough = this->zero(newWidth, newHeight);
    for (i = 0; i < width; i++)
    {
        for (j = 0; j < height; j++)
        {
            pixel = gsImg->pixel(i, j);
            value = qGray(pixel);
            if (value > 0)
            {
                for (k = 0; k < theta_size; k++)
                {
                    theta = (k * M_PI)/(thetaDensity*180);
                    ro = i*std::cos(theta)+j*sin(theta);
                    hough(k, ro+ro_max)++;
                }
            }
        }
    }
    max = hough.max();
    this->normalize(0, max, hough, 0, 255);
    for (i = 0; i < newWidth; i++)
    {
        for (j = 0; j < newHeight; j++)
        {
            result->setPixel(i, j, hough(i, j));
        }
    }

    return result;
}
