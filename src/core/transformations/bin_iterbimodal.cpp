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
    this->threshold_current = 128;
    thresholdNew = (this->avgSat(0, this->threshold_current-1)*
                    this->avgSat(this->threshold_current, 255))/2;
    /*while (iter < 20 || thresholdNew != this->threshold_current )
    {
        this->threshold_current = thresholdNew;
        thresholdNew = (this->avgSat(0)*this->avgSat(1))/2;
        iter ++;
    }*/
    bm = new BinarizationManual(this->image);
    bm->setParameter("threshold", this->threshold_current);
    newImage = bm->transform();

    return newImage;
}

int BinarizationIterBimodal::avgSat(int start, int end)
{
    int start, end, iter, val, sum, licz, result;

    if (_class == 0)
    {
        start = 0;
        end = this->threshold_current-1;
    }
    else
    {
        start = this->threshold_current;
        end = 255;
    }
    sum = 0;
    licz = 0;
    for (iter = start; iter <= end; iter++)
    {
        val = this->hg->value(iter);
        sum += val;
        licz += val * iter;
    }
    result = licz/sum;

    return result;
}



