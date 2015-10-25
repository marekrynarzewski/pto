#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QHash>
#include <QImage>

/** Class containing a histogram in form of four QHashes */
class Histogram
{
public:
    enum Channel {RGB, RChannel, GChannel, BChannel, LChannel};

    explicit Histogram(QImage *parent);
    ~Histogram();

    QHash<int, int>* get(Histogram::Channel);
    QImage           getImage(Histogram::Channel, QBrush);
    int              maximumValue(Channel);

private:
    void generate(QImage*); // iterates all parent image pixels and set the Hashes
    void fill_zeros(QHash<int, int>* map, int k);
    QHash<int, int>* R;
    QHash<int, int>* G;
    QHash<int, int>* B;
    QHash<int, int>* L;
    void increment(QHash<int, int>* map, int key);
};

#endif // HISTOGRAM_H
