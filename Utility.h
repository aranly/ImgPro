#pragma once
#include <string.h>
#include <stdlib.h>  
#include <math.h>
#include <Windows.h>

#include "MyImage.h"


#define clamp255(a) ((a) > 255 ? 255:((a) < 0? 0: (a)))
#define ImageStart(pcData, offsetX, offsetY, xStride) ((pcData) + (offsetY) * (xStride) + (offsetX))

#define Pixel(x, y, xStride, offsetX, offsetY, image)	image[((y) + (offsetY)) * (xStride) + (x) + (offsetX)]

typedef struct Point
{
	int iX;
	int iY;
} Point;

typedef struct Line
{
	Point p0;
	Point p1;
} Line;

extern unsigned char CalculateAverage(int width, int height, unsigned char *pcData);
unsigned char CalculateAverageWithStride(int width, int height, int offsetX, int offsetY, int xStride, unsigned char *pcData);
extern void AddPadding(int width, int height, int offsetX, int offsetY, int xStride, int padWidth, unsigned char* pcData);
extern void AddPaddingInt(int width, int height, int offsetX, int offsetY, int xStride, int padWidth, int* piData);
extern void AddPadding255(int width, int height, int offsetX, int offsetY, int xStride, int padWidth, unsigned char* pcData);
extern void AddPadding0(int width, int height, int offsetX, int offsetY, int xStride, int padWidth, unsigned char* pcData);
extern void FilterImage(int* filter, int dimensions, int divisor, int width, int height, int xStride, int offsetX, int offsetY, unsigned char* pcImageSource, unsigned char* PcImageDest);
extern void FilterImage(int* filter, int dimensions, int divisor, int width, int height, int xStride, int offsetX, int offsetY, unsigned char* pcImageSource, int* PcImageDest);
extern void ErodeImageWithElement(int* element, int dimensions, int divisor, int width, int height, int xStride, int offsetX, int offsetY, unsigned char* pcImageSource, unsigned char* pcImageDest);
extern void ErodeImageWithElementWithoutPadding(int* element, int dimensions, int divisor, int width, int height, unsigned char* pcImageSource, unsigned char* pcImageDest);
extern void DilateImageWithElement(int* element, int dimensions, int divisor, int width, int height, int xStride, int offsetX, int offsetY, unsigned char* pcImageSource, unsigned char* pcImageDest);
extern void DilateImageWithElementWithoutPadding(int* element, int dimensions, int divisor, int width, int height, unsigned char* pcImageSource, unsigned char* pcImageDest);
extern int* CreateHistogram(int buckets, int width, int height, int xStride, int offsetX, int offsetY, unsigned char*pcImageData);
extern void NormalizeMinMax(int width, int height, int offsetX, int offsetY, int xStride, int* GradientData, unsigned char* pcNew);
extern int LocalMean(int dimensions, int widthposition, int heightposition, int xStride, int offsetX, int offsetY, unsigned char* pcData);
extern float LocalStandardDeviation(int dimensions, int widthposition, int heightposition, int xStride, int offsetX, int offsetY, unsigned char* pcData);
extern unsigned char *ImageGetRegionsWithHighBlockContrast4QRCode(int iBlockSize, int iRegionWidth, int iRegionHeight, int iRegionThreshold, int iContrastThreshold,
	int width, int height, int offsetX, int offsetY, int xStride, int padWidth, unsigned char* pcData);
extern void ImageCalBlockContrast(unsigned char *pucContrast, int iBlockSize, int iThreshold, int width, int height, int offsetX, int offsetY, int xStride, int padWidth, unsigned char* pcData);
extern void ImageFindContainingQuadrilateral(Point *pQuadrilateral, unsigned char *pucMask, int iMaskBlockSize, MyImage &image);
extern int IfWhiteLine(Point start, Point end, int iWidth, int iHeight, int iStride, int iStartX, int iStartY, unsigned char *pucImage);
extern void ImageCalPerspectiveTransformSquareToQuadrilateral(Point *pPoints, Transform *pTransform);
void ImageTransform(Transform *pTransform, MyImage &dstImage, MyImage &srcImage);
void ImageRegionGrowing(Point seed, int width, int height, int xStride, int offsetX, int offsetY, unsigned char* pucSrc, unsigned char* pucDest);
