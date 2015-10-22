#include "histogram_stretching.h"

#include "../histogram.h"

HistogramStretching::HistogramStretching(PNM* img) :
    Transformation(img)
{
}

HistogramStretching::HistogramStretching(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* HistogramStretching::transform()
{
    int width  = image->width();
    int height = image->height();
    bool formatIndexed8 = image->format() == QImage::Format_Indexed8;
    if (formatIndexed8){
        HistogramStretching::stretch_histogram_by_channel(Histogram::LChannel);

    }
    PNM* newImage = new PNM(width, height, image->format());
    for (int w = 0; w < width; w++){
        for (int h = 0; h < height; h++){
            if (formatIndexed8){

            }
        }
    }

    return newImage;
}

static void HistogramStretching::stretch_histogram_by_channel(Histogram::Channel channel){
   QHash<int, int>* histogram;
   int min, max, key;
   QHash<int, int>::iterator it;

   min = 255;
   max = 0;
   histogram = image->getHistogram()->get(channel);
   for (it = histogram->begin(); it != histogram->end(); it++){
       key = it.key();
       if (key < min){
           min = key;
       }
       if (key > max){
           max = key;
       }
   }
}

