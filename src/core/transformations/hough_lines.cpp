#include "hough_lines.h"

#include "bin_gradient.h"
#include "edge_laplacian.h"
#include "hough.h"

#include <QPainter>

HoughLines::HoughLines(PNM* img) :
    Transformation(img)
{
}

HoughLines::HoughLines(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

PNM* Hough::binarize(PNM* img)
{
    BinarizationGradient* bg;
    PNM* result;

    bg = new BinarizationGradient(img);
    result = bg->transform();

    return result;
}

PNM* Hough::detectEdges(PNM* img)
{
    EdgeLaplacian* el;
    PNM* result;

    el = new EdgeLaplacian(img);
    el->setParameter("size", 3);
    result = el->transform();

    return result;
}

PNM* Hough::houghTransform(PNM* img)
{
    Hough* h;
    PNM* result;

    h = new Hough(img);
    h->setParameter("theta_density", 3);
    h->setParameter("skip_edge_detection", true);
    result = h->transform();

    return result;
}



PNM* HoughLines::transform()
{
    // Cut of value from the image;
    int  threshold      = getParameter("threshold").toInt();
    bool drawWholeLines = getParameter("draw_whole_lines").toBool();

    PNM* newImage = new PNM(image->copy());

    int width = image->width();
    int height = image->height();
    PNM* img;

    img = this->detectEdges(this->image);
    img = this->binarize(img);
    img = this->houghTransform(img);

    int theta = img->width();
    int p = img->height();

    QPainter* qPainter = new QPainter(newImage);
    qPainter->setPen(Qt::red);

    for (int x = 0; x < theta; x++)
        for (int y = 0; y < p; y++) {
            QRgb pixel = houghImage->pixel(x,y);
            int v = qGray(pixel);
            if (v > threshold) {
                int r = y - p/2;
                float rTheta = (x/3.0)*M_PI / 180.0;
                float sinTheta = sin(rTheta);
                float cosTheta = cos(rTheta);

                int x1 = 0;
                int y1 = r/sinTheta;
                int x2 = width - 1;
                int y2 = (r - (width-1) * cosTheta)/sinTheta;
                qPainter->drawLine(x1, y1, x2, y2);
            }
        }

    if (!drawWholeLines) {
        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++) {
                if (qGray(binaryImage->pixel(x,y)) == 0)
                    newImage->setPixel(x,y,image->pixel(x,y));
            }
    }

    return newImage;
}
