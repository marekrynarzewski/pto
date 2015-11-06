#include "bin_iterbimodal.h"

#include "conversion_grayscale.h"
#include "histogram_equalization.h"
#include "bin_manual.h"
#include "../histogram.h"

BinarizationIterBimodal::BinarizationIterBimodal(PNM* img) :
    Transformation(img)
{
}

BinarizationIterBimodal::BinarizationIterBimodal(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* BinarizationIterBimodal::transform()
{
    int thresholdNew, width, height;
    PNM* newImage;
    HistogramEqualization* he;
    BinarizationManual* bm;
    int iter = 1;

    width = image->width();
    height = image->height();
    newImage = new PNM(width, height, QImage::Format_Mono);
    he = new HistogramEqualization(this->image);
    this->image = he->transform();
    this->hg = this->image->getHistogram()->get(Histogram::LChannel);
    this->threshold = 128;
    thresholdNew = (this->avgSat(0, this->threshold-1)+
                    this->avgSat(this->threshold, 255))/2;
    while (thresholdNew != this->threshold)
    {
        this->threshold = thresholdNew;
        thresholdNew = (this->avgSat(0, this->threshold-1)+
                    this->avgSat(this->threshold, 255))/2;
        iter ++;
        qDebug() << thresholdNew;
    }
    bm = new BinarizationManual(this->image);
    bm->setParameter("threshold", this->threshold);
    newImage = bm->transform();

    return newImage;
}

int BinarizationIterBimodal::avgSat(int start, int end)
{
    int iter, val, sum, licz, result;

    sum = 1;
    licz = 1;
    for (iter = start; iter <= end; iter++)
    {
        val = this->hg->value(iter);
        sum += val;
        licz += val * iter;
    }
    result = licz/sum;

    return result;
}



