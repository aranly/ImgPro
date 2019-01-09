#include "Utility.h"

extern void AdjustImageBrightness(float brightness, int width, int height, int offsetX, int offsetY, int xStride, unsigned char* pucDst, unsigned char* pucSrc);
extern void AdjustImageContrast(float Contrast, int width, int height, int offsetX, int offsetY, int xStride, unsigned char* pucDst, unsigned char* pucSrc);

extern void SharpenImage(int width, int height, int offsetX, int offsetY, int xStride, unsigned char* pucDst, unsigned char* pucSrc);
extern void BlurImage(int width, int height, int offsetX, int offsetY, int xStride, unsigned char* pucDst, unsigned char* pucSrc);
extern void KernelImage(int width, int height, int offsetX, int offsetY, int xStride, unsigned char* pucDst, unsigned char* pucSrc);
extern void SobelImage(int width, int height, int offsetX, int offsetY, int xStride, int* pigradientMagnitude, float* pfgradientAngle, unsigned char* pucSrc);
extern void CannyImage(int width, int height, int offsetX, int offsetY, int xStride, int iThreshold1, int iThreshold2, unsigned char* pucDst, unsigned char* pucSrc);
extern void Greyscale4Y(int width, int height, int xStride, int offsetX, int offsetY, unsigned char*pcDst, unsigned char* pucSrc);
extern void Greyscale4UV(int width, int height, int xStride, int offsetX, int offsetY, unsigned char*pcData);

extern void BinarizeImage(int width, int height, int xStride, int offsetX, int offsetY, unsigned char*pcData);
extern void CustomBinarizeImage(int width, int height, int xStride, int offsetX, int offsetY, int threshold, unsigned char*pcData);
extern void OtsuBinarizeImage(int width, int height, int xStride, int offsetX, int offsetY, int*histogram, unsigned char*pcData);
extern void SauvolaBinarizeImage(int width, int height, int xStride, int offsetX, int offsetY, int windowSize, unsigned char*pcSource, unsigned char* pcDest);
extern void NiblackBinarizeImage(int width, int height, int xStride, int offsetX, int offsetY, int windowSize, unsigned char*pcSource, unsigned char* pcDest);
extern void ZxingBinarizeImage(int xStride, int width, int height, unsigned char*pcSource, unsigned char* pcDest);
extern void GlobalBinarizeImage(int width, int height, int xStride, int offsetX, int offsetY, int* histogram, unsigned char*pcData);

extern void GreyscaleRGB(int width, int height, unsigned char*pcData);
extern void ErodeImage(int dimension, int width, int height, int offsetX, int offsetY, int xStride, unsigned char* pcImageSource, unsigned char* pcImageDest);
extern void DilateImage(int dimension, int width, int height, int offsetX, int offsetY, int xStride, unsigned char* pcImageSource, unsigned char* pcImageDest);
extern void CalculateBlackPoints(int subWidth, int subHeight, int xStride, int width, int height, unsigned char*pcData, unsigned char* BlackPoints);
extern void calculateThresholdForBlock(int subWidth, int subHeight, int xStride, int width, int height, unsigned char* blackPoints, unsigned char* pcSource, unsigned char* pcDest);


