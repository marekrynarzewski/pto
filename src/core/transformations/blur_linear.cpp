#include "blur_linear.h"

BlurLinear::BlurLinear(PNM* img) :
    Convolution(img)
{
}

BlurLinear::BlurLinear(PNM* img, ImageViewer* iv) :
    Convolution(img, iv)
{
}

PNM* BlurLinear::transform()
{
    int maskSize = getParameter("size").toInt();
    QList<QVariant> tmpMask = getParameter("mask").toList();
    bool normalize = getParameter("normalize").toBool();

    math::matrix<float> mask(maskSize, maskSize);
	int r, c, i, s, e;
	i = 0, s = 0;
    for (r = 0; r < maskSize; r++)
	{
        for (c = 0; c < maskSize; c++)
		{
			e = tmpMask.at(i).toDouble();
			s += e;
			mask[r][c] = e;
			i++;
		}
	}
	
	if (normalize){
        for (r = 0; r < maskSize; r++)
		{
            for (c = 0; c < maskSize; c++)
			{
				mask[r][c] /= s;
			}
		}
	}

    return convolute(mask, RepeatEdge);
}
