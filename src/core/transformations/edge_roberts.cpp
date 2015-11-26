#include "edge_roberts.h"

EdgeRoberts::EdgeRoberts(PNM* img) :
    EdgeGradient(img)
{
    prepareMatrices();
}

EdgeRoberts::EdgeRoberts(PNM* img, ImageViewer* iv) :
    EdgeGradient(img, iv)
{
    prepareMatrices();
}

void EdgeRoberts::prepareMatrices()
{
    this->g_x = math::matrix<float>(2, 2);
    this->g_y = math::matrix<float>(2, 2);
    this->g_x(0, 0) = 1;
    this->g_x(0, 1) = 0;
    this->g_x(1, 0) = 0;
    this->g_x(1, 1) = -1;

    this->g_y(0, 0) = 0;
    this->g_y(0, 1) = 1;
    this->g_y(1, 0) = -1;
    this->g_y(1, 1) = 0;
}
