#ifndef HOUGH_LINES_H
#define HOUGH_LINES_H

#include "transformation.h"

class HoughLines : public Transformation
{
public:
    explicit HoughLines(PNM*);
    explicit HoughLines(PNM*, ImageViewer*);

    virtual PNM* transform();
private:
    PNM* detectEdges(PNM* img);
    PNM* binarize(PNM* img);
    PNM* houghTransform(PNM* img);
};

#endif // HOUGH_LINES_H
