#include "histogram.h"

#include <QDebug>
#include <QPainter>

#include <cmath>
using namespace std;

Histogram::Histogram(QImage* image)
{
    R = new QHash<int, int>;
    G = new QHash<int, int>;
    B = new QHash<int, int>;
    L = new QHash<int, int>;
    generate(image);
}

Histogram::~Histogram()
{
    delete R;
    delete G;
    delete B;
    delete L;
}

void Histogram::generate(QImage* image)
{
    //qDebug() << Q_FUNC_INFO << "Not implemented yet!";
    for (int i = 0; i < image->width(); i++){
		for (int j = 0; j < image->height(); j++){
			int r, g, b, l;
			QRgb pixel = image->pixel(i, j);
			r = qRed(pixel);    // Get the 0-255 value of the R channel
            g = qGreen(pixel);  // Get the 0-255 value of the G channel
            b = qBlue(pixel);
			l = qGray(pixel);
			this->increment(this->R, r);
			this->increment(this->G, g);
			this->increment(this->B, b);
			this->increment(this->L, l);
		}
	}
}

void Histogram::increment(QHash<int, int>* map, int key)
{
	int value;
	
    value = map->value(key, 0);
	value += 1;
	map->insert(key, value);
}


/** Returns the maximal value of the histogram in the given channel */
int Histogram::maximumValue(Channel selectedChannel = RGB)
{
    QHash<int, int>* map;
    QHash<int, int>::iterator it;
    int result;
    int r, g, b, value;

    if (selectedChannel == RGB){
        r = this->maximumValue(RChannel);
        g  = this->maximumValue(GChannel);
        b = this->maximumValue(BChannel);
        return max(r, max(g, b));
    }

    result = 0;
    map = this->get(selectedChannel);
    for (it = map->begin(); it != map->end(); it++){
        value = it.value();
        if (value > result){
            result = value;
        }
    }



    return result;
}


/** Returns a pointer to the given channel QHash<int, int> */
QHash<int, int>* Histogram::get(Channel channel = LChannel)
{
    if (channel==LChannel) return L;
    if (channel==RChannel) return R;
    if (channel==GChannel) return G;
    if (channel==BChannel) return B;

    return 0;
}

/**
 *  Returns a 255 by 100 QImage containing a Histogram for the given channel.
 *  The background is transparent (Alpha 0, RGB=255) */
QImage Histogram::getImage(Channel channel = LChannel, QBrush pen = Qt::gray)
{
    // Create blank QImage and fill it with transparent background:
    QImage histImage(255, 100, QImage::Format_ARGB32);
    histImage.fill(0);
    QPainter painter(&histImage);
    painter.setBrush(Qt::transparent); 
    painter.setPen(Qt::transparent);
    painter.drawRect(0,0,255,100);

    // Calculate the aspect ratio using the maximal value of the color histograms
    int maximum = (channel == LChannel ? maximumValue(LChannel) :  maximumValue(RGB));
    float ratio = 100.0/float(maximum);

    // Preparing the painter:
    painter.setBrush(pen);
    painter.setPen(pen.color());

    int h;
    // Draw histogram
    QHash<int, int>* hist = get(channel);
    QHash<int, int>::const_iterator cit = hist->begin();

    while (cit != hist->end())
    {
        h = 100 - floor(ratio*cit.value());
        painter.drawLine(cit.key(), h, cit.key(), 100);
        ++cit;
    }

    return histImage;
}
