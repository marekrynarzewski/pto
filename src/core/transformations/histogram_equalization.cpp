#include "histogram_equalization.h"

#include "../histogram.h"

HistogramEqualization::HistogramEqualization(PNM* img) :
    Transformation(img)
{
}

HistogramEqualization::HistogramEqualization(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}
QMap<int, double> HistogramEqualization::probability(QHash<int, int> dh){
    double sum = (double)image->width()*(double)image->height();
    QMap<int, double> res;
    for (QHash<int, int>::iterator it = dh.begin(); it != dh.end(); it++){
        int k = it.key();
        int v = it.value();
        double f = (double)v/sum;
        res.insert(k, f);
    }
    return res;
}

QMap<int, double> HistogramEqualization::distrib(QMap<int, double> prob){
    QMap<int, double> res;
    QList<int> keys = prob.keys();
    res[0] = prob[0];
    for (int i = 1; i < keys.size(); i++){
        int k = keys[i];
        double p1 = prob[k];
        double p2 = res[keys[i-1]];
        res.insert(k, p1+p2);
    }
    return res;
}

QMap<int, double> HistogramEqualization::evaluate_channel(Histogram::Channel channel){
    Histogram* hist = image->getHistogram();
    QHash<int, int>* chan = hist->get(channel);
    QMap<int, double> prob = probability(*chan);
    QMap<int, double> dist = distrib(prob);
    return dist;
}
PNM* HistogramEqualization::transform()
{
    int width = image->width();
    int height = image->height();

    PNM* newImage = new PNM(width, height, image->format());
    bool formatIndexed8 = image->format() == QImage::Format_Indexed8;
    QMap<int, double> lec, rec, gec, bec;
    if (formatIndexed8){
        lec = evaluate_channel(Histogram::LChannel);
     } else {
        rec = evaluate_channel(Histogram::RChannel);
        gec = evaluate_channel(Histogram::GChannel);
        bec = evaluate_channel(Histogram::BChannel);
    }
    bool p1 = true;
    for (int w = 0; w < width; w++){
        for (int h = 0; h < height; h++){
            QRgb pxl = image->pixel(w, h);

            if (formatIndexed8){
                newImage->setPixel(w, h, lec[qGray(pxl)]*255);

             } else {
                int r = rec[qRed(pxl)]*255;
                int g = gec[qGreen(pxl)]*255;
                int b = bec[qBlue(pxl)]*255;
                QColor newPixel = QColor(r,g,b);

                newImage->setPixel(w,h, newPixel.rgb());
            }

        }
    }
    return newImage;
}

/*void HistogramEqualization::updateLUT(double *D, double *LUT, int series)
{
    int i;
    double D0min;
    //znajdz pierwszą niezerową wartosc dystrybuanty
    i = 0 ;
    while (D[i] == 0)
    {
        i++;
    }
    D0min = D[i];

    for (i=0; i<256; i++)
    {
        LUT[i] = (((D[i] - D0min) / (1 - D0min)) * (256 - 1)) ;
    }

}*/

/*void HistogramEqualization::bib()
{
    int i, j, rvalue, gvalue, bvalue, grayvalue;
    double sumR, sumG, sumB, sumGray, numberOfPixels;
    double r[256], g[256], b[256], gray[256];
    double Dr[256], Dg[256], Db[256], Dgray[256];
    QRgb color;

    for (i=0; i<256; i++)
    {
        r[i] = 0;
        g[i] = 0;
        b[i] = 0;
        gray[i] = 0;
    }
    for (i=0; i<width; i++)
    {   for (j=0; j<height; j++)
        {
            color = image->pixel(i, j);
            r[qRed(color)]++;
            g[qGreen(color)]++;
            b[qBlue(color)]++;
            color = image->pixel(i, j);
            gray[GetRValue(color)]++;
        }
    }

    numberOfPixels = width*height;
    sumR = 0;
    sumG = 0;
    sumB = 0;
    sumGray = 0;
    for (i=0; i<256; i++)
    {
        sumR += (r[i]/numberOfPixels);
        sumG += (g[i]/numberOfPixels);
        sumB += (b[i]/numberOfPixels);
        sumGray += (gray[i]/numberOfPixels);
        Dr[i] += sumR;
        Dg[i] += sumG;
        Db[i] += sumB;
        Dgray[i] += sumGray;
    }

    updateLUT(Dr, LUTr, 1);
    updateLUT(Dg, LUTg, 2);
    updateLUT(Db, LUTb, 3);
    updateLUT(Dgray, LUTgray, 0);

}*/
