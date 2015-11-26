#include "edge_prewitt.h"

EdgePrewitt::EdgePrewitt(PNM*img) :
    EdgeGradient(img)
{
    prepareMatrices();
}

EdgePrewitt::EdgePrewitt(PNM*img, ImageViewer* iv) :
    EdgeGradient(img, iv)
{
    prepareMatrices();
}

void EdgePrewitt::prepareMatrices()
{
    this->g_x = math::matrix<float>(3, 3);
    this->g_y = math::matrix<float>(3, 3);
    this->g_x(0, 0) = -1;
    this->g_x(1, 0) = -1;
    this->g_x(2, 0) = -1;
    this->g_x(0, 1) = 0;
    this->g_x(1, 1) = 0;
    this->g_x(2, 1) = 0;
    this->g_x(0, 2) = 1;
    this->g_x(1, 2) = 1;
    this->g_x(2, 2) = 1;

    this->g_y(0, 0) = -1;
    this->g_y(0, 1) = -1;
    this->g_y(0, 2) = -1;
    this->g_y(1, 0) = 0;
    this->g_y(1, 1) = 0;
    this->g_y(1, 2) = 0;
    this->g_y(2, 0) = 1;
    this->g_y(2, 1) = 1;
    this->g_y(2, 2) = 1;
}

