#include "morphological_operator.h"

MorphologicalOperator::MorphologicalOperator(PNM* img) :
    Transformation(img)
{
}

MorphologicalOperator::MorphologicalOperator(PNM* img, ImageViewer* iv) :
    Transformation(img, iv)
{
}

// abstract
const int MorphologicalOperator::morph(math::matrix<float>, math::matrix<bool>)
{
    return 0;
}

math::matrix<bool> MorphologicalOperator::getSE(int size, SE shape)
{
    switch (shape)
    {
    case Square:    return seSquare(size);
    case Cross:     return seCross(size);
    case XCross:    return seXCross(size);
    case VLine:     return seVLine(size);
    case HLine:     return seHLine(size);
    default:        return seSquare(size);
    }
}


math::matrix<bool> MorphologicalOperator::seSquare(int size)
{
    math::matrix<bool> ret(size, size);

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            ret(i, j) = true;
        }
    }

    return ret;
}

math::matrix<bool> MorphologicalOperator::seCross(int size)
{
    math::matrix<bool> ret(size, size);

    int center = size/2;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (i == center || j == center)
            {
                ret(i, j) = true;
            }
            else
            {
                ret(i, j) = false;
            }
        }
    }


    return ret;
}

math::matrix<bool> MorphologicalOperator::seXCross(int size)
{
    math::matrix<bool> ret(size, size);

    int row, col;
    row = 0;
    col = size-1;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if ((i == j) || (i == row && j == col))
            {
                ret(i, j) = true;
            }
            else
            {
                ret(i, j) = false;
            }

        }
        row += 1;
        col -= 1;
    }


    return ret;
}

math::matrix<bool> MorphologicalOperator::seVLine(int size)
{
    math::matrix<bool> ret(size, size);

    int center = size/2;
    for (int i = 0; i < size; i++)
    {
        ret(i, center) = true;
    }
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (j != center)
            {
                ret(i, j) = false;
            }
        }
    }


    return ret;
}

math::matrix<bool> MorphologicalOperator::seHLine(int size)
{
    math::matrix<bool> ret(size, size);

    int center = size/2;
    for (int i = 0; i < size; i++)
    {
        ret(center, i) = true;
    }
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (i != center)
            {
                ret(i, j) = false;
            }
        }
    }


    return ret;
}

void show_matrix(math::matrix<bool> m)
{
    int size = m.colno();
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
             qDebug() << i << j << m(i, j);
        }
    }

}

PNM* MorphologicalOperator::transform()
{  
    int size  = getParameter("size").toInt();
    SE  shape = (MorphologicalOperator::SE) getParameter("shape").toInt();

    PNM* newImage = new PNM(image->width(), image->height(), QImage::Format_RGB32);

    math::matrix<bool> m = this->getSE(size, shape);
    for (int i = 0; i < this->image->width(); i++)
    {
        for (int j = 0; j < this->image->height(); j++)
        {
            math::matrix<float> windowR = this->getWindow(i, j, size, RChannel, RepeatEdge);
            math::matrix<float> windowG = this->getWindow(i, j, size, GChannel, RepeatEdge);
            math::matrix<float> windowB = this->getWindow(i, j, size, BChannel, RepeatEdge);
            int r = this->morph(windowR, m);
            int g = this->morph(windowG, m);
            int b = this->morph(windowB, m);
            QColor color = QColor(r, g, b);
            newImage->setPixel(i, j, color.rgb());
        }
    }

    return newImage;
}
