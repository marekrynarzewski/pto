#include "convolution.h"

/** Overloaded constructor */
Convolution::Convolution(PNM* img) :
    Transformation(img)
{
}

Convolution::Convolution(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

/** Returns a convoluted form of the image */
PNM* Convolution::transform()
{
    return convolute(getMask(3, Normalize), RepeatEdge);
}

/** Returns a sizeXsize matrix with the center point equal 1.0 */
math::matrix<float> Convolution::getMask(int size, Mode mode = Normalize)
{
    math::matrix<float> mask(size, size);
    int center = size/2;

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            mask[i][j] = 0.0;
        }
    }
    mask[center][center] = 1.0;

    return mask;
}

/** Does the convolution process for all pixels using the given mask. */
PNM* Convolution::convolute(math::matrix<float> mask, Mode mode = RepeatEdge)
{
    int width  = image->width(),
        height = image->height();
    QHash<int, QHash<int, int> > R, G, B, L;
    PNM* newImage = new PNM(width, height, image->format());
    this->reflectMask = this->reflection(mask);
    this->maskSize = mask.colsize();
    this->maskWeight = this->sum(mask);
    if (this->image->format() == QImage::Format_Indexed8)
    {
        L = iteratePixelsByChannel(LChannel, mode);
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                newImage->setPixel(i, j, L.value(i).value(j));
            }
        }
    }
    else
    {
        R = iteratePixelsByChannel(RChannel, mode);
        G = iteratePixelsByChannel(GChannel, mode);
        B = iteratePixelsByChannel(BChannel, mode);
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                int r = R.value(i).value(j);
                //qDebug() << i << j << r;
                int g = G.value(i).value(j);
                int b = B.value(i).value(j);
                QColor newPixel = QColor(r,g,b);
                newImage->setPixel(i,j, newPixel.rgb());
             }
        }
    }

    return newImage;

}

const QHash<int, QHash<int, int> > Convolution::iteratePixelsByChannel(Channel channel, Mode mode)
{
    QHash<int, QHash<int, int> > result;
    int width  = image->width(),
        height = image->height();
    int res;
    for (int i = 0; i < width; i++)
    {
        QHash<int, int> tmp;
        for (int j = 0; j < height; j++)
        {
            res = this->convolutePixel(i, j,channel, mode);
            if (channel == RChannel)
                //qDebug()<< i << j << res;
             tmp.insert(j, res);
        }
        result.insert(i, tmp);
    }

    return result;
}

int check_range(int myValue, int min, int max)
{
    if (myValue < min)
    {
        return min;
    }
    if (myValue > max)
    {
        return max;
    }
    return myValue;
}
 const int Convolution::convolutePixel(int row, int col, Channel channel, Mode mode)
 {
     math::matrix<float> window, accumulator;

     window = getWindow(row, col, this->maskSize, channel, mode);
     accumulator = this->join(window, this->reflectMask);
     float sumOfAccumulator = this->sum(accumulator);
     if (this->maskWeight != 0)
     {
         sumOfAccumulator = sumOfAccumulator/this->maskWeight;
     }
 //   qDebug()<< (int) sumOfAccumulator;

    return check_range((int)sumOfAccumulator, 0, 255);
 }

/** Joins to matrices by multiplying the A[i,j] with B[i,j].
  * Warning! Both Matrices must be squares with the same size!
  */
const math::matrix<float> Convolution::join(math::matrix<float> A, math::matrix<float> B)
{
    int size = A.rowno();
    math::matrix<float> C(size, size);

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            C[i][j] = A[i][j]*B[i][j];
        }
    }

    return C;
}

/** Sums all of the matrixes elements */
const float Convolution::sum(const math::matrix<float> A)
{
    float sum = 0.0;
    int size = A.rowno();
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            sum += A[i][j];
        }
    }

    return sum;

}


/** Returns reflected version of a matrix */
const math::matrix<float> Convolution::reflection(const math::matrix<float> A)
{
    int size = A.rowno();
    math::matrix<float> C(size, size);
    int sri, sci, dri, dci;
    for (dri = 0, sri = size-1; dri < size; dri++, sri--)
    {
        for (dci = 0, sci = size-1; dci < size; dci++, sci--)
        {
            C[dri][dci] = A[sri][sci];
        }
    }

    return C;
}
