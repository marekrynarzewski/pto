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
    math::matrix<float> a_ij, window;
    math::matrix<float>* x_gradient = new math::matrix<float>(this->image->width(), this->image->height());

    for (int i = 0; i < this->image->width(); i++)
    {
        for (int j = 0; j < this->image->height(); j++)
        {
            window = this->getWindow(i, j, 3, LChannel, RepeatEdge);
            a_ij = this->join(this->g_x, window);
            (*x_gradient)[i][j] = a_ij.sum();
        }
    }
    return x_gradient;
}

math::matrix<float>* EdgeSobel::rawVerticalDetection()
{
    math::matrix<float> a_ij, window;
    math::matrix<float>* y_gradient = new  math::matrix<float>(this->image->width(), this->image->height());

    for (int i = 0; i < this->image->width(); i++)
    {
        for (int j = 0; j < this->image->height(); j++)
        {
            window = this->getWindow(i, j, 3, LChannel, RepeatEdge);
            a_ij = this->join(this->g_y, window);
            (*y_gradient)[i][j] = a_ij.sum();
        }
    }

    return y_gradient;
}
