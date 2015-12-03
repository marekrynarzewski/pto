#ifndef TEXTURE_NORMAL_H
#define TEXTURE_NORMAL_H

#include "convolution.h"

class MapNormal : public Convolution
{
public:
    MapNormal(PNM*);
    MapNormal(PNM*, ImageViewer*);

    virtual PNM* transform();
};
template <typename T>
bool in_range(T min, T a, T max);

template <typename T>
bool in_range3(T min, T a, T b, T c, T max);

#endif // TEXTURE_NORMAL_H
