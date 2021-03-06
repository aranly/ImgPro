#include<string.h>
#include "Enhance.h"
#include "myImage.h"
#define MY_PROFILE_TIMING

void AdjustImageBrightness(float brightness, int width, int height, int offsetX, int offsetY, int xStride, unsigned char* pucDst, unsigned char* pucSrc) {
	int i, j;
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			pucDst[((i + offsetY)*xStride) + j + offsetX] = (unsigned char)clamp255(brightness * pucSrc[((i + offsetY)*xStride) + j + offsetX]);
		}
	}
}

void AdjustImageContrast(float Contrast, int width, int height, int offsetX, int offsetY, int xStride, unsigned char* pucDst, unsigned char* pucSrc) {
	unsigned char avg = 0;
	avg = CalculateAverageWithStride(width, height, offsetX, offsetY, xStride, pucSrc);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (pucSrc[((i + offsetY)*xStride) + j + offsetX] > avg) {
				int k = pucSrc[((i + offsetY)*xStride) + j + offsetX];
				pucDst[((i + offsetY)*xStride) + j + offsetX] = (unsigned char)clamp255(((pucSrc[((i + offsetY)*xStride) + j + offsetX] - avg)*Contrast + 0.5) + avg);
			}
			else {
				pucDst[((i + offsetY)*xStride) + j + offsetX] = (unsigned char)clamp255(avg - (avg - (pucSrc[((i + offsetY)*xStride) + j + offsetX])*Contrast + 0.5));
			}
		}
	}
}
void SharpenImage(int width, int height, int offsetX, int offsetY, int xStride, unsigned char* pucDst, unsigned char* pucSrc) {
	int* pFilter;
	int filter[] = { 0,-1,0,
					-1,5,-1,
					0,-1,0 };
	pFilter = &filter[0];
	AddPadding(width, height, offsetX, offsetY, xStride, 1, pucSrc);
	FilterImage(pFilter, 3, 1, width, height, xStride, offsetX, offsetY,
		pucDst + offsetY * xStride + offsetX, pucSrc + offsetY * xStride + offsetX);
}

void BlurImage(int width, int height, int offsetX, int offsetY, int xStride, unsigned char* pucDst, unsigned char* pucSrc) {
	int* pFilter;
	int filter[] = { 1,2,1,
					2,4,2,
					1,2,1 };
	pFilter = &filter[0];
	AddPadding(width, height, offsetX, offsetY, xStride, 1, pucSrc);
	FilterImage(pFilter, 3, 1, width, height, xStride, offsetX, offsetY,
		pucDst + offsetY * xStride + offsetX, pucSrc + offsetY * xStride + offsetX);

}

void KernelImage(int width, int height, int offsetX, int offsetY, int xStride, unsigned char* pucDst, unsigned char* pucSrc) {
	int* pFilter;
	int filter[] = { 1,2,1,
					2,4,2,
					1,2,1 };
	pFilter = &filter[0];
	AddPadding(width, height, offsetX, offsetY, xStride, 1, pucSrc);
	FilterImage(pFilter, 3, 16, width, height, xStride, offsetX, offsetY,
		pucDst + offsetY * xStride + offsetX, pucSrc + offsetY * xStride + offsetX);
}

void SobelImage(int width, int height, int offsetX, int offsetY, int xStride, int* piGradientMagnitude, float* pfGradientAngle, unsigned char* pucSrc) {
	int iGradientX, iGradientY;


	AddPadding(width, height, offsetX, offsetY, xStride, 1, pucSrc);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			iGradientX = Pixel(j + 1, i, xStride, offsetX, offsetY, pucSrc) * 2
				+ Pixel(j + 1, i - 1, xStride, offsetX, offsetY, pucSrc)
				+ Pixel(j + 1, i + 1, xStride, offsetX, offsetY, pucSrc)
				- Pixel(j - 1, i, xStride, offsetX, offsetY, pucSrc) * 2
				- Pixel(j - 1, i - 1, xStride, offsetX, offsetY, pucSrc)
				- Pixel(j - 1, i + 1, xStride, offsetX, offsetY, pucSrc);
			iGradientY = Pixel(j, i + 1, xStride, offsetX, offsetY, pucSrc) * 2
				+ Pixel(j - 1, i + 1, xStride, offsetX, offsetY, pucSrc)
				+ Pixel(j + 1, i + 1, xStride, offsetX, offsetY, pucSrc)
				- Pixel(j, i - 1, xStride, offsetX, offsetY, pucSrc) * 2
				- Pixel(j - 1, i - 1, xStride, offsetX, offsetY, pucSrc)
				- Pixel(j + 1, i - 1, xStride, offsetX, offsetY, pucSrc);
			//piGradientMagnitude[i * width + j] = sqrt(iGradientX * iGradientX + iGradientY * iGradientY);
			//pfGradientAngle[i * width + j] = atan2(iGradientY, iGradientX);
		}
	}
}
#define TANPIDIV8	0.4142135623730950488016887242097
#define CTANPIDIV8	2.4142135623730950488016887242097

extern short gpsSqrtFor10x10[1024][1024];
extern short gpsSqrtFor8x8[256][256];

extern int *gpiSqrtFast[1024];
extern int gpiSqrtFastNumBits[1024];

#define SQRTNRSHIFT		10

void CannyImage(int width, int height, int offsetX, int offsetY, int xStride, int iThreshold1, int iThreshold2, unsigned char* pucDst, unsigned char * pucSrc)
{
#ifdef MY_PROFILE_TIMING
	LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
	LARGE_INTEGER Frequency;
#endif
	int i, j;
	int iGradientX, iGradientY;
	int* piGradientMagnitude = (int*)calloc(xStride * (height + OFFSET * 2), sizeof(int));
	unsigned char* pucDirection = (unsigned char*)malloc(width * height);
	int iValue;
	unsigned char *pucPos, *pucCurPos;
	int* piPos, *piCurPos;
#ifdef MY_PROFILE_TIMING
	QueryPerformanceCounter(&StartingTime);
#endif
	// sobel
	AddPadding(width, height, offsetX, offsetY, xStride, 1, pucSrc);
	for (int i = 0; i < height; i++)
	{
		pucPos = ImageStart(pucSrc, offsetX, i + offsetY, xStride);
		for (int j = 0; j < width; j++)
		{
#if 1
			unsigned char ucP00, ucP01, ucP02, ucP10, ucP12, ucP20, ucP21, ucP22;
			pucCurPos = pucPos - xStride - 1;
			ucP00 = *(pucCurPos);
			ucP01 = *(pucCurPos + 1);
			ucP02 = *(pucCurPos + 2);
			pucCurPos += xStride;
			ucP10 = *(pucCurPos);
			ucP12 = *(pucCurPos + 2);
			pucCurPos += xStride;
			ucP20 = *(pucCurPos);
			ucP21 = *(pucCurPos + 1);
			ucP22 = *(pucCurPos + 2);
			iGradientX = ucP12 * 2 + ucP02 + ucP22 - ucP10 * 2 - ucP00 - ucP20;
			iGradientY = ucP21 * 2 + ucP20 + ucP22 - ucP01 * 2 - ucP00 - ucP02;
			pucPos++;
#else
			iGradientX = Pixel(j + 1, i, xStride, offsetX, offsetY, pucSrc) * 2
				+ Pixel(j + 1, i - 1, xStride, offsetX, offsetY, pucSrc)
				+ Pixel(j + 1, i + 1, xStride, offsetX, offsetY, pucSrc)
				- Pixel(j - 1, i, xStride, offsetX, offsetY, pucSrc) * 2
				- Pixel(j - 1, i - 1, xStride, offsetX, offsetY, pucSrc)
				- Pixel(j - 1, i + 1, xStride, offsetX, offsetY, pucSrc);
			iGradientY = Pixel(j, i + 1, xStride, offsetX, offsetY, pucSrc) * 2
				+ Pixel(j - 1, i + 1, xStride, offsetX, offsetY, pucSrc)
				+ Pixel(j + 1, i + 1, xStride, offsetX, offsetY, pucSrc)
				- Pixel(j, i - 1, xStride, offsetX, offsetY, pucSrc) * 2
				- Pixel(j - 1, i - 1, xStride, offsetX, offsetY, pucSrc)
				- Pixel(j + 1, i - 1, xStride, offsetX, offsetY, pucSrc);
#endif
			//Pixel(j, i, xStride, offsetX, offsetY, piGradientMagnitude) = sqrt(iGradientX * iGradientX + iGradientY * iGradientY);
			Pixel(j, i, xStride, offsetX, offsetY, piGradientMagnitude) = gpsSqrtFor10x10[abs(iGradientX)][abs(iGradientY)];
#if 1
			if (iGradientX == 0)
				pucDirection[i * width + j] = 2;
			else
			{
				float tan = abs((float)iGradientY / iGradientX);
				if (tan <= TANPIDIV8)
					pucDirection[i * width + j] = 0;
				else if (tan >= CTANPIDIV8)
					pucDirection[i * width + j] = 2;
				else if ((iGradientY ^ iGradientX) > 0)
					pucDirection[i * width + j] = 1;
				else
					pucDirection[i * width + j] = 3;
			}
#endif
		}
	}
	//SobelImage(width, height, offsetX, offsetY, xStride, piGradientMagnitude, pfGradientAngle, pucSrc);
#if 0
#ifdef MY_PROFILE_TIMING
	QueryPerformanceCounter(&EndingTime);
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
#endif
#endif

#if 1	
#ifdef MY_PROFILE_TIMING
	QueryPerformanceCounter(&StartingTime);
#endif
#endif
	AddPaddingInt(width, height, offsetX, offsetY, xStride, 1, piGradientMagnitude);
	unsigned char* pucEdge = (unsigned char *)malloc(xStride * (height + OFFSET * 2));
	memset(pucEdge, 0, xStride * (height + OFFSET * 2));
	for (i = 0; i < height; i++)
	{
		piPos = ImageStart(piGradientMagnitude, offsetX, i + offsetY, xStride);
		for (j = 0; j < width; j++)
		{
			//iValue = *(piPos);

			switch (pucDirection[i * width + j])
			{
			case 0:
				if(*(piPos) > *(piPos - 1) && *(piPos) > *(piPos + 1))
				{ 
					//iValue = *(piPos);
				/*if ((Pixel(j, i, xStride, offsetX, offsetY, piGradientMagnitude) >
						Pixel(j - 1, i, xStride, offsetX, offsetY, piGradientMagnitude)) &&
					(Pixel(j, i, xStride, offsetX, offsetY, piGradientMagnitude) >
						Pixel(j + 1, i, xStride, offsetX, offsetY, piGradientMagnitude)))
				{
					iValue = Pixel(j, i, xStride, offsetX, offsetY, piGradientMagnitude);*/
					if (*(piPos) > iThreshold1)
						pucEdge[((i + offsetY)*xStride) + j + offsetX] = 255;
					else if (*(piPos) >= iThreshold2)
						pucEdge[((i + offsetY)*xStride) + j + offsetX] = 128;
				}
				break;
			case 1:
				if (*(piPos) > *(piPos - xStride - 1) && *(piPos) > *(piPos + xStride + 1))
				{
					//iValue = *(piPos);
					/*
				if ((Pixel(j, i, xStride, offsetX, offsetY, piGradientMagnitude) >
						Pixel(j - 1, i - 1, xStride, offsetX, offsetY, piGradientMagnitude)) &&
					(Pixel(j, i, xStride, offsetX, offsetY, piGradientMagnitude) >
						Pixel(j + 1, i + 1, xStride, offsetX, offsetY, piGradientMagnitude))) 
				{
					iValue = Pixel(j, i, xStride, offsetX, offsetY, piGradientMagnitude);
					*/
					if (*(piPos) > iThreshold1)
						pucEdge[((i + offsetY)*xStride) + j + offsetX] = 255;
					else if (*(piPos) >= iThreshold2)
						pucEdge[((i + offsetY)*xStride) + j + offsetX] = 128;
				}
				break;
			case 2:
				if (*(piPos) > *(piPos - xStride) && *(piPos) > *(piPos + xStride))
				{
					//iValue = *(piPos);
					/*
				if ((Pixel(j, i, xStride, offsetX, offsetY, piGradientMagnitude) >
					Pixel(j, i - 1, xStride, offsetX, offsetY, piGradientMagnitude)) &&
					(Pixel(j, i, xStride, offsetX, offsetY, piGradientMagnitude) >
						Pixel(j, i + 1, xStride, offsetX, offsetY, piGradientMagnitude)))
				{
					iValue = Pixel(j, i, xStride, offsetX, offsetY, piGradientMagnitude);
					*/
					if (*(piPos) > iThreshold1)
						pucEdge[((i + offsetY)*xStride) + j + offsetX] = 255;
					else if (*(piPos) >= iThreshold2)
						pucEdge[((i + offsetY)*xStride) + j + offsetX] = 128;
				}
				break;
			default:	// 3
				if (*(piPos) > *(piPos - xStride + 1) && *(piPos) > *(piPos + xStride - 1))
				{
					//iValue = *(piPos);
					/*
				if ((Pixel(j, i, xStride, offsetX, offsetY, piGradientMagnitude) >
					Pixel(j + 1, i - 1, xStride, offsetX, offsetY, piGradientMagnitude)) &&
					(Pixel(j, i, xStride, offsetX, offsetY, piGradientMagnitude) >
						Pixel(j - 1, i + 1, xStride, offsetX, offsetY, piGradientMagnitude)))
				{
					iValue = Pixel(j, i, xStride, offsetX, offsetY, piGradientMagnitude);
					*/
					if (*(piPos) > iThreshold1)
						pucEdge[((i + offsetY)*xStride) + j + offsetX] = 255;
					else if (*(piPos) >= iThreshold2)
						pucEdge[((i + offsetY)*xStride) + j + offsetX] = 128;
				}
				break;
			}
			piPos++;
		}
	}

	memset(pucDst, 0, xStride * (height + OFFSET * 2));

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{	
			if (Pixel(j, i, xStride, offsetX, offsetY, pucEdge) == 255) 
			{
				pucDst[(i + offsetY) * xStride + j + offsetX] = 255;
			}
			else if (Pixel(j, i, xStride, offsetX, offsetY, pucEdge) == 128)
			{
				for (int k = -1; k <= 1; k++) 
				{
					for (int l = -1; l <= 1; l++)
					{
						if (Pixel(j + k , i + l, xStride, offsetX, offsetY, pucEdge) == 255) {
							pucDst[(i + offsetY) * xStride + j + offsetX] = 255;
						}
					}
				}
			}
		}
	}
#ifdef MY_PROFILE_TIMING
	QueryPerformanceCounter(&EndingTime);
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
#endif
	return;
}



void Greyscale4Y(int width, int height, int xStride, int offsetX, int offsetY, unsigned char*pucDst, unsigned char* pucSrc) {
	memcpy(pucDst, pucSrc, (height + offsetY * 2) * xStride);
}

void Greyscale4UV(int width, int height, int xStride, int offsetX, int offsetY, unsigned char*pcData) {
	memset(pcData, 128, (height + offsetY * 2) * xStride);
}

void BinarizeImage(int width, int height, int xStride, int offsetX, int offsetY, unsigned char*pcData) {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (pcData[((i + offsetY)*xStride) + j + offsetX] > 128) {
				pcData[((i + offsetY)*xStride) + j + offsetX] = 255;
			}
			else {
				pcData[((i + offsetY)*xStride) + j + offsetX] = 0;
			}
		}
	}
}
void CustomBinarizeImage(int width, int height, int xStride, int offsetX, int offsetY, int threshold, unsigned char*pcData) {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (pcData[((i + offsetY)*xStride) + j + offsetX] > threshold) {
				pcData[((i + offsetY)*xStride) + j + offsetX] = 255;
			}
			else {
				pcData[((i + offsetY)*xStride) + j + offsetX] = 0;
			}
		}
	}
}

void OtsuBinarizeImage(int width, int height, int xStride, int offsetX, int offsetY, int*histogram, unsigned char*pcData) {
	float sum = 0;
	for (int i = 0; i < 256; i++) {
		sum += i * histogram[i];
	}
	float sumB = 0;
	int weightBackground = 0;
	int weightForeground = 0;
	float varMax = 0;
	float threshold = 0;
	for (int t = 0; t < 256; t++) {
		weightBackground += histogram[t];
		if (weightBackground == 0) {
			continue;
		}
		weightForeground = width * height - weightBackground;
		if (weightForeground == 0) {
			continue;
		}
		sumB += (float)(t * histogram[t]);
		float mB = sumB / weightBackground;
		float mF = (sum - sumB) / weightForeground;
		float varBetween = (float)weightBackground * (float)weightForeground * (mB - mF) * (mB - mF);
		if (varBetween > varMax) {
			varMax = varBetween;
			threshold = t;
		}
	}

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (pcData[((i + offsetY)*xStride) + j + offsetX] > threshold) {
				pcData[((i + offsetY)*xStride) + j + offsetX] = 255;
			}
			else {
				pcData[((i + offsetY)*xStride) + j + offsetX] = 0;
			}
		}
	}
}

void SauvolaBinarizeImage(int width, int height, int xStride, int offsetX, int offsetY, int windowSize, unsigned char*pcSource, unsigned char*pcDest) {
	AddPadding(width, height, offsetX, offsetY, xStride, 15, pcSource);
	unsigned char* pcImageStart = pcSource + offsetY * xStride + offsetX;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int Sauvolathreshold;
			//threshold = mean * ( 1 + k * ( standard_deviation / r - 1 ) 
			Sauvolathreshold = LocalMean(windowSize, j, i, xStride, offsetX, offsetY, pcImageStart)*(1 +
				0.5*(LocalStandardDeviation(windowSize, j, i, xStride, offsetX, offsetY, pcImageStart) / 128 - 1));
			if (pcSource[((i + offsetY)*xStride) + j + offsetX] > Sauvolathreshold) {
				pcDest[((i + offsetY)*xStride) + j + offsetX] = 255;
			}
			else {
				pcDest[((i + offsetY)*xStride) + j + offsetX] = 0;
			}
		}
	}
}
void NiblackBinarizeImage(int width, int height, int xStride, int offsetX, int offsetY, int windowSize, unsigned char*pcSource, unsigned char* pcDest) {
	AddPadding(width, height, offsetX, offsetY, xStride, 15, pcSource);
	unsigned char* pcImageStart = pcSource + offsetY * xStride + offsetX;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int Sauvolathreshold;
			//threshold = mean - k * standard_deviation 
			Sauvolathreshold = LocalMean(windowSize, j, i, xStride, offsetX, offsetY, pcImageStart) - 0.2*
				LocalStandardDeviation(windowSize, j, i, xStride, offsetX, offsetY, pcImageStart);
			if (pcSource[((i + offsetY)*xStride) + j + offsetX] > Sauvolathreshold) {
				pcDest[((i + offsetY)*xStride) + j + offsetX] = 255;
			}
			else {
				pcSource[((i + offsetY)*xStride) + j + offsetX] = 0;
			}
		}
	}
}
//implemented from Zxing 
void ZxingBinarizeImage(int xStride, int width, int height, unsigned char *pcSource, unsigned char *pcDest)
{
	int BLOCK_SIZE_POWER = 3;
	int BLOCK_SIZE = 1 << BLOCK_SIZE_POWER; // ...0100...00
	int BLOCK_SIZE_MASK = BLOCK_SIZE - 1;   // ...0011...11
	int MINIMUM_DIMENSION = BLOCK_SIZE * 5;
	int MIN_DYNAMIC_RANGE = 24;

	int subWidth = width >> BLOCK_SIZE_POWER;
	if ((width & BLOCK_SIZE_MASK) != 0) {
		subWidth++;
	}
	int subHeight = height >> BLOCK_SIZE_POWER;
	if ((height & BLOCK_SIZE_MASK) != 0) {
		subHeight++;
	}
	unsigned char* pcBlackPoints = (unsigned char *)malloc(width * height);
	CalculateBlackPoints(subWidth, subHeight, xStride, width, height, pcSource, pcBlackPoints);

	calculateThresholdForBlock(subWidth, subHeight, xStride, width, height, pcBlackPoints, pcSource, pcDest);


}

// for every block (BLOCK_SIZE * BLOCK_SIZE) of pixels, calculate min/max/average.
// if the block is too flat (max - min) < certain threshold (MIN_DYNAMIC_RANGE), 
//	then the above average may not be useful. So it chooses between min/2 and
//	the average of already calculated neighbors (if the average of neighbors is
//	smaller than min, this block is likely to be white. So we lower the threshold (blackpoint)
//	to min/2 to prefer white).
void CalculateBlackPoints(int subWidth, int subHeight, int xStride, int width, int height, unsigned char*pcData, unsigned char* blackpoints) {
	int BLOCK_SIZE_POWER = 3;
	int BLOCK_SIZE = 1 << BLOCK_SIZE_POWER; // ...0100...00
	int BLOCK_SIZE_MASK = BLOCK_SIZE - 1;   // ...0011...11
	int MINIMUM_DIMENSION = BLOCK_SIZE * 5;
	int MIN_DYNAMIC_RANGE = 24;

	int maxYOffset = height - BLOCK_SIZE;
	int maxXOffset = width - BLOCK_SIZE;
	for (int y = 0; y < subHeight; y++) {
		int yoffset = y << BLOCK_SIZE_POWER;
		if (yoffset > maxYOffset) {
			yoffset = maxYOffset;
		}
		for (int x = 0; x < subWidth; x++) {
			int xoffset = x << BLOCK_SIZE_POWER;
			if (xoffset > maxXOffset) {
				xoffset = maxXOffset;
			}
			int sum = 0;
			int min = 0xFF;
			int max = 0;
			for (int yy = 0, offset = yoffset * xStride + xoffset; yy < BLOCK_SIZE; yy++, offset += xStride) {
				for (int xx = 0; xx < BLOCK_SIZE; xx++) {
					unsigned char pixel = pcData[offset + xx];
					sum += pixel;
					if (pixel < min) {
						min = pixel;
					}
					if (pixel > max) {
						max = pixel;
					}
				}
				if (max - min > MIN_DYNAMIC_RANGE) {
					// finish the rest of the rows quickly
					for (yy++, offset += xStride; yy < BLOCK_SIZE; yy++, offset += xStride) {
						for (int xx = 0; xx < BLOCK_SIZE; xx++) {
							sum += pcData[offset + xx];
						}
					}
				}
			}
			int average = sum >> (BLOCK_SIZE_POWER * 2);
			if (max - min <= MIN_DYNAMIC_RANGE) {
				average = min / 2;

				if (y > 0 && x > 0) {
					int averageNeighborBlackPoint =
						(blackpoints[(y - 1)*subWidth + x] + (2 * blackpoints[y*subWidth + x - 1]) + blackpoints[(y - 1)*subWidth + (x - 1)]) / 4;
					if (min < averageNeighborBlackPoint) {
						average = averageNeighborBlackPoint;
					}
				}
			}
			blackpoints[y*subWidth + x] = average;
		}
	}
}

// use the average of blackpoint of 5x5 neighbors as threshold for binarization
void calculateThresholdForBlock(int subWidth, int subHeight, int xStride, int width, int height, unsigned char* blackPoints, unsigned char* pcSource, unsigned char* pcDest) {
	int BLOCK_SIZE_POWER = 3;
	int BLOCK_SIZE = 1 << BLOCK_SIZE_POWER; // ...0100...00
	int BLOCK_SIZE_MASK = BLOCK_SIZE - 1;   // ...0011...11
	int MINIMUM_DIMENSION = BLOCK_SIZE * 5;
	int MIN_DYNAMIC_RANGE = 24;

	unsigned char* pcSmoothedBlackPoints = (unsigned char *)malloc(width * height);

	for (int i = 2; i < subHeight - 2; i++)
	{
		for (int j = 2; j < subWidth - 2; j++)
		{
			int iSum = 0;
			for (int k = -2; k <= 2; k++)
			{
				for (int l = -2; l <= 2; l++)
					iSum += blackPoints[(i + k) * subWidth + j + l];
			}
			pcSmoothedBlackPoints[i * subWidth + j] = iSum / 25;
		}
	}
	for (int i = 0; i < subHeight; i++)
	{
		for (int j = 0; j < subWidth; j++)
		{
			for (int k = 0; k < BLOCK_SIZE; k++)
			{
				for (int l = 0; l < BLOCK_SIZE; l++)
				{
					if (pcSource[(i*BLOCK_SIZE + k)*xStride + j * BLOCK_SIZE + l] > pcSmoothedBlackPoints[i*subWidth + j]) {
						pcDest[(i*BLOCK_SIZE + k)*xStride + j * BLOCK_SIZE + l] = 255;
					}
					else {
						pcDest[(i*BLOCK_SIZE + k)*xStride + j * BLOCK_SIZE + l] = 0;
					}
				}
			}
		}
	}
}


void GlobalBinarizeImage(int width, int height, int xStride, int offsetX, int offsetY, int* histogram, unsigned char*pcData) {
#define BINARIZATION_VALLEY_START_THRESHOLD	0.05	// 0.15 of max value
#define BINARIZATION_VALLEY_PERCENTAGE	0.15	// 0.15 of total bins
	int ValleyThreshold;
	int ValleyBins;
	int piValleyStart[64];
	int piValleyEnd[64];
	int iNumValley = 0;
	int iValleyStart = -1;
	int MaxOfBins = 0;
	int buckets = 256;
	for (int i = 0; i < buckets; i++) {
		if (histogram[i] > MaxOfBins) {
			MaxOfBins = histogram[i];
		}
	}
	ValleyThreshold = BINARIZATION_VALLEY_START_THRESHOLD * MaxOfBins;
	while (true) {
		ValleyBins = 0;
		for (int i = 0; i < buckets; i++)
		{
			if (histogram[i] < ValleyThreshold) {
				ValleyBins++;
			}
		}
		if ((float)ValleyBins / buckets < BINARIZATION_VALLEY_PERCENTAGE)
		{
			ValleyThreshold++;
		}
		else
			break;
	}

	for (int i = 0; i < buckets; i++)
	{
		if (histogram[i] < ValleyThreshold) {
			if (iValleyStart == -1) {
				iValleyStart = i;
			}
		}
		else
		{
			if (iValleyStart != -1)
			{
				piValleyStart[iNumValley] = iValleyStart;
				piValleyEnd[iNumValley] = i - 1;
				iNumValley++;
				iValleyStart = -1;
			}
		}
	}
	int Threshold = (piValleyStart[1] + piValleyEnd[1]) / 2;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (pcData[((i + offsetY)*xStride) + j + offsetX] >= Threshold) {
				pcData[((i + offsetY)*xStride) + j + offsetX] = 255;
			}
			else {
				pcData[((i + offsetY)*xStride) + j + offsetX] = 0;
			}
		}
	}
}
void GreyscaleRGB(int width, int height, unsigned char*pcData) {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int temp = ((66 * pcData[(i*width + j) * 3] + 129 * pcData[(i*width + j) * 3 + 1] + 25 * pcData[(i*width + j) * 3 + 2] + 128) >> 8) + 16;
			pcData[(i*width + j) * 3] = (unsigned char)clamp255(temp);
			pcData[(i*width + j) * 3 + 1] = (unsigned char)clamp255(temp);
			pcData[(i*width + j) * 3 + 2] = (unsigned char)clamp255(temp);
		}
	}
}
/*
void ErodeImage(int width, int height, int xStride, int offsetX, int offsetY, unsigned char* pcImageSource, unsigned char* pcImageDest) {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (pcImageSource[((i + offsetY - 1)*xStride) + j + offsetX - 1] == 255 && pcImageSource[((i + offsetY + 1)*xStride) + j + offsetX] == 255 && pcImageSource[((i + offsetY + 1)*xStride) + j + offsetX + 1] == 255 &&
				pcImageSource[((i + offsetY)*xStride) + j + offsetX - 1] == 255 && pcImageSource[((i + offsetY)*xStride) + j + offsetX] == 255 && pcImageSource[((i + offsetY)*xStride) + j + offsetX + 1] == 255 &&
				pcImageSource[((i + offsetY - 1)*xStride) + j + offsetX - 1] == 255 && pcImageSource[((i + offsetY - 1)*xStride) + j + offsetX] == 255 && pcImageSource[((i + offsetY - 1)*xStride) + j + offsetX + 1] == 255) {
				pcImageDest[((i + offsetY)*xStride) + j + offsetX] = 255;
			}
			else {
				pcImageDest[((i + offsetY)*xStride) + j + offsetX] = 0;
			}
		}
	}
}
*/
void ErodeImage(int dimensions, int width, int height, int offsetX, int offsetY, int xStride, unsigned char* pcImageSource, unsigned char* pcImageDest) {
	int *element;
	element = (int*)malloc(dimensions * dimensions * sizeof(int));
	for (int i = 0; i < dimensions * dimensions; i++) {
		element[i] = 1;
	}
	AddPadding255(width, height, offsetX, offsetY, xStride, dimensions/2, pcImageSource);
	ErodeImageWithElement(element, dimensions, 1, width, height, xStride, offsetX, offsetY, pcImageSource, pcImageDest);
	free(element);
}

void DilateImage(int dimensions, int width, int height, int offsetX, int offsetY, int xStride, unsigned char* pcImageSource, unsigned char* pcImageDest) {
	int *element;
	element = (int*)malloc(dimensions * dimensions * sizeof(int));
	for (int i = 0; i < dimensions * dimensions; i++) {
		element[i] = 1;
	}
	AddPadding0(width, height, offsetX, offsetY, xStride, dimensions/2, pcImageSource);
	DilateImageWithElement(element, 5, 1, width, height, xStride, offsetX, offsetY, pcImageSource, pcImageDest);
	free(element);
}


