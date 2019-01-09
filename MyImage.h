#ifndef _MYIMAGE_H_
#define _MYIMAGE_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <complex>
#include <vector>
#include <list>
#include <deque>
#include <set>
#include <map>
#include <utility>
#include <algorithm>
#include <cstdlib>
#include <cassert>
using namespace std;

#include "Histogram.h"

#define IMAGE_MAX_WIDTH			4096
#define IMAGE_MAX_HEIGHT		4096

#define SUBFORMAT_NONPLANAR	0
#define SUBFORMAT_PLANAR	1
#define SUBFORMAT_RGBA      2
#define OFFSET 32
#define IMAGE_SCALE_FACTOR 50
#define PI 3.1415926535
#define IMAGE_MODE_DISPLAY		0
#define IMAGE_MODE_HISTOGRAM	1
#define IMAGE_MODE_QRCODE		2

typedef enum ColorFormat{
	UNDEFINED_FORMAT,
	RGB,
	YUV,
	BAYER
}ColorFormat;

typedef struct ImageConfig
{
	ColorFormat format;
	char subformat;
	int width;
	int height;
	int xStride;
	int offsetX;
	int offsetY;
	int ifClear;
} ImageConfig;

typedef struct Filter
{
	int width;
	int height;
	int *piValue;
	int iDivisor;
} Filter;

#define IMAGE_TRANSFORM_SCALE		0
#define IMAGE_TRANSFORM_ROTATE		1
#define IMAGE_TRANSFORM_PERSPECTIVE	2

typedef struct Transform
{
	int iMode;		// rotate/scale/perspective
	float pfCoefs[4][4];
} Transform;

class MyImage
{
public:
	ColorFormat format; //RGB or YUV, or other, see ColorFormat
	char subformat; // nonplanar(0) or planar(1), see SUBFORMAT_xxx
	int width;
	int height;
	int xStride;	//  The amount of bytes used to hold a scanline(single line of the image including extra space on the left and/or the right). We use this space for padding 
	int offsetX;	//  The starting place in the memory line where the image data begins in pixels
	int offsetY;	//	number of memory lines from the beginning of the buffer to where the image data begins 
	unsigned char *pucData[3]; // memory that stores pixel data. in the case of rgb/rgba nonplanar data is stored in pucData[0], in the case of planar RGB and yuv the data is stored in the different indices.

	void Init();
	void Init(ImageConfig &config);
	void CloneFormat(MyImage& image);
	bool isInited();
	bool isFormatEqual(MyImage& image);
	void Deinitialize();
	void LoadImageFromFile(char* filename);
	void StoreImageToFile(char* filename);
	void imageCopyConfig(ImageConfig &config);
	void Copy(MyImage &srcImage);

	// histogram
	void CalHistogram(Histogram *pHistogram);

	// transformation
	void FlipX();
	void FlipX(MyImage &srcImage);
	void FlipY();
	void FlipY(MyImage &srcImage);
	void Scale(int scale);
	void Scale(int scale, MyImage &srcImage);

	void Rotate(int degree);
	void Rotate(int degree, MyImage &srcImage);

	//conversion
	void RGBtoYUV();
	void RGBtoYUV(MyImage &destImage);
	void RGBtoYUV(bool AddSpace);
	void RGBtoYUV(bool AddSpace, MyImage &srcImage);

	void ConvertToRGBA(MyImage &srcImage);

	void GenBlockImage(unsigned char* pucBlockData, int iDataWidth, int iDataHeight, int iBlockWidth, int iBlockHeight);

	// enhancement
	void AdjustBrightness(float brightness);
	void AdjustBrightness(float brightness, MyImage &srcImage);
	void AdjustContrast(float contrast);
	void AdjustContrast(float contrast, MyImage &srcImage);
	void MakeGreyscale();
	void MakeGreyscale(MyImage &srcImage);
	void Sharpen();
	void Sharpen(MyImage &srcImage);
	void Blur();
	void Blur(MyImage &srcImage);
	void Kernel();
	void Kernel(MyImage &srcImage);

	void Canny();
	void SobelVisualization();
	void Segmentation();
	void CustomBinarization(int threshold);
	void GlobalBinarization(Histogram histogram);
	void OtsuBinarization(Histogram histogram);
	void ZxingBinarization();
	void SauvolaBinarization();
	void NiblackBinarization();
	void Erode(int dimension);
	void Dilate(int dimension);
	MyImage();
	MyImage(MyImage &image);

	~MyImage();
};

#endif


