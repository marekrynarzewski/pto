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
    int cen = size/2;
    mask[cen][cen] = 1.0;
    for (int i = 0; i < size && i != cen; i++)
    {
        for (int j = 0; j < size && j != cen; j++)
        {
            mask[i][j] = 0.0;
        }
    }

    return mask;
}

/** Does the convolution process for all pixels using the given mask. */
PNM* Convolution::convolute(math::matrix<float> mask, Mode mode = RepeatEdge)
{
    int width  = image->width(),
        height = image->height();
    PNM* newImage = new PNM(width, height, image->format());
    float weight = sum(mask);
    int size = mask.colsize()/2;
    iteratePixelsByChannel(newImage, size, LChannel, mode, weight);
    iteratePixelsByChannel(newImage, size, RChannel, mode, weight);
    iteratePixelsByChannel(newImage, size, GChannel, mode, weight);
    iteratePixelsByChannel(newImage, size, BChannel, mode, weight);

    return newImage;

}

const void Convolution::iteratePixelsByChannel(PNM* newImage, int size, Channel channel, Mode mode, float weight)
{
    int width  = image->width(),
        height = image->height();
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
             convulatePixel(newImage, i, j, size, channel, mode, weight);
        }
    }
}

 const void Convolution::convulatePixel(PNM* newImage, int row, int col, int size, Channel channel, Mode mode, float weight)
 {
     math::matrix<float> window = getWindow(row, col, size, channel, mode);
     math::matrix<float> reflect = this->reflection(window);
     math::matrix<float> accumulator = this->join(window, reflect);
     float sumOfAccumulator = this->sum(accumulator);
     if (weight > 0)
     {
         sumOfAccumulator = sumOfAccumulator/weight;
     }
     if (sumOfAccumulator > 0 && sumOfAccumulator < 255)
     {
         newImage->setPixel(row, col, (int)sumOfAccumulator);
     }
     else if (sumOfAccumulator < 0)
     {
         newImage->setPixel(row, col, 0);
     }
     else
     {
         newImage->setPixel(row, col, 255);
     }
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
