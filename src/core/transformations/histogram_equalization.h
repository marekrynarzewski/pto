#ifndef HISTOGRAM_EQUALIZATION_H
#define HISTOGRAM_EQUALIZATION_H

#include "transformation.h"
#include "histogram.h"

class HistogramEqualization : public Transformation
{
public:
    HistogramEqualization(PNM*);
    HistogramEqualization(PNM*, ImageViewer*);

    virtual PNM* transform();
private:
    QMap<int, double> probability(QHash<int, int>);
    QMap<int, double> distrib(QMap<int, double> prob);
    QMap<int, double> evaluate_channel(Histogram::Channel channel);
};


#endif // HISTOGRAM_EQUALIZATION_H
