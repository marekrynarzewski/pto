#ifndef HISTOGRAM_STRETCHING_H
#define HISTOGRAM_STRETCHING_H

#include "transformation.h"
#include "../histogram.h"

class HistogramStretching : public Transformation
{
public:
    HistogramStretching(PNM*);
    HistogramStretching(PNM*, ImageViewer* iv);

    virtual PNM* transform();

    QHash<int, int> stretch_histogram_by_channel(Histogram::Channel);
};

#endif // HISTOGRAM_STRETCHING_H
