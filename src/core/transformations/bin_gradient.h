#ifndef BIN_GRADIENT_H
#define BIN_GRADIENT_H

#include "transformation.h"

class BinarizationGradient : public Transformation
{
public:
    BinarizationGradient(PNM*);
    BinarizationGradient(PNM*, ImageViewer*);

    virtual PNM* transform();
private:
    int g1(int, int);
    int g2(int, int);
};

#endif // BIN_GRADIENT_H
