#include "edge_sobel.h"

EdgeSobel::EdgeSobel(PNM* img, ImageViewer* iv) :
    EdgeGradient(img, iv)
{
    prepareMatrices();
}

EdgeSobel::EdgeSobel(PNM* img) :
    EdgeGradient(img)
{
    prepareMatrices();
}

void EdgeSobel::prepareMatrices()
{
    this->g_x = math::matrix<float>(3, 3);
    this->g_y = math::matrix<float>(3, 3);
    this->g_x(0, 0) = -1;
    this->g_x(0, 1) = 0;
    this->g_x(0, 2) = 1;
    this->g_x(1, 0) = -2;
    this->g_x(1, 1) = 0;
    this->g_x(1, 2) = 2;
    this->g_x(2, 0) = -1;
    this->g_x(2, 1) = 0;
    this->g_x(2, 2) = 1;

    this->g_y(0, 0) = -1;
    this->g_y(0, 1) = -2;
    this->g_y(0, 2) = -1;
    this->g_y(1, 2) = 0;
    this->g_y(1, 2) = 0;
    this->g_y(1, 2) = 0;
    this->g_y(2, 0) = 1;
    this->g_y(2, 1) = 2;
    this->g_y(2, 2) = 1;

}

math::matrix<float>* EdgeSobel::rawHorizontalDetection()
{
    math::matrix<float>* x_gradient = new math::matrix<float>(this->image->width(), this->image->height());

    qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    return x_gradient;
}

math::matrix<float>* EdgeSobel::rawVerticalDetection()
{
    math::matrix<float>* y_gradient = new  math::matrix<float>(this->image->width(), this->image->height());

    qDebug() << Q_FUNC_INFO << "Not implemented yet!";

    return y_gradient;
}
