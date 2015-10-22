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
    QHash<int, int> rh,gh,bh,lh;
    int rv, gv, bv, lv;
    QRgb pxl;
    if (formatIndexed8){
        lh = HistogramStretching::stretch_histogram_by_channel(Histogram::LChannel);
    } else {
        rh = HistogramStretching::stretch_histogram_by_channel(Histogram::RChannel);
        gh = HistogramStretching::stretch_histogram_by_channel(Histogram::GChannel);
        bh = HistogramStretching::stretch_histogram_by_channel(Histogram::BChannel);
    }

    PNM* newImage = new PNM(width, height, image->format());
    for (int w = 0; w < width; w++){
        for (int h = 0; h < height; h++){
            pxl = image->pixel(w, h);
            if (formatIndexed8){
                lv = qGray(pxl);
                lv = lh[lv];
                newImage->setPixel(w, h, lv);
            } else {
                rv = qRed(pxl);
                gv = qGreen(pxl);
                bv = qBlue(pxl);
                rv = rh[rv];
                bv = bh[bv];
                gv = gh[gv];
                QColor newPixel = QColor(rv,gv,bv);
                newImage->setPixel(w,h, newPixel.rgb());
            }
        }
    }

    return newImage;
}

QHash<int, int> HistogramStretching::stretch_histogram_by_channel(
        Histogram::Channel channel){
   QHash<int, int>* histogram, wynik;
   int min, chnl, diff, con;
   QHash<int, int>::iterator itp;
   QMap<int, int> map;

   histogram = image->getHistogram()->get(channel);
   for (itp = histogram->begin(); itp != histogram->end(); itp++){
       map.insert(itp.key(), itp.value());
   }
   min = map.begin().key();
   con = 255/(map.lastKey() - min);
   for (chnl = 0; chnl < 256; chnl++){
       diff = chnl-min;
       int res = con*diff;
       wynik.insert(chnl, res);
       //qDebug() << "chnl" << chnl << "con" << con << "diff" << diff << endl;
   }
   return wynik;
}

