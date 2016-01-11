#ifndef HOUGH_H
#define HOUGH_H

#include "transformation.h"

class Hough : public Transformation
{
public:
    explicit Hough(PNM*);
    explicit Hough(PNM*, ImageViewer*);

    virtual PNM* transform();
private:
    PNM* toGrayScale(PNM*);
    math::matrix<int> zero(int width, int height);
    template<typename T>
    math::matrix<T> matrix(int width, int height, T value);
    template<typename T>
    void normalize(T min, T max, math::matrix<T>& m, T n_min, T n_max);
    PNM* toEdgeLaplacian(PNM* img);
    template<typename T>
    void normalize(math::matrix<T>& m, T max);
};

#endif // HOUGH_H
