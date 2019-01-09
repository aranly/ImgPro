#include "qrcode.h"

void QRCode::Init()
{
}

void QRCode::Process(MyImage& image) {
	unsigned char *pcData, *pcNew;
	int i, j, k, l;
	int piFilterCoeffs0[] = {
							1, 2, 1,
							2, 4, 2,
							1, 2, 1
	};
	Filter filter0 = { 3, 3, piFilterCoeffs0, 16 };
	unsigned char *pucBlockContrast;
	int iBlockSize = 8;
	ImageConfig config;
	MyImage newImage;
	Histogram histogram;
	image.RGBtoYUV();
	HistogramInit(256, &histogram);
	image.CalHistogram(&histogram);
	//image.Kernel();
	image.CustomBinarization(128);

	//image.GlobalBinarization(histogram);
	//image.OtsuBinarization(histogram);
#if 0
	return;
#endif
	pucBlockContrast = ImageGetRegionsWithHighBlockContrast4QRCode(iBlockSize, 168, 160, 5230, 30, image.width, image.height, image.offsetX, image.offsetY, image.xStride, 0, image.pucData[0]);
#if 0
	pcNew = pucBlockContrast;
	image.GenBlockImage(pucBlockContrast, image.width/ iBlockSize, image.height/ iBlockSize, iBlockSize, iBlockSize);
	return;
#endif

	ImageFindContainingQuadrilateral(pPoints, pucBlockContrast, iBlockSize, image);

#if 0
	{
		unsigned char *pucStart = image.pucData[0] + image.xStride * image.offsetY + image.offsetX;
		pucStart++;
	}
	return;
#endif


	ImageCalPerspectiveTransformSquareToQuadrilateral(pPoints, &transform);
	transform.iMode = IMAGE_TRANSFORM_PERSPECTIVE;

	config.format = YUV;
	config.subformat = 1;
	config.offsetX = 0;
	config.offsetY = 0;
	config.width = QRCODE_NORMALIZED_WIDTH;
	config.height = QRCODE_NORMALIZED_WIDTH;
	config.xStride = QRCODE_NORMALIZED_WIDTH;
	config.ifClear = 1;
	newImage.Init(config);
	ImageTransform(&transform, newImage, image);
	image.Copy(newImage);
}


