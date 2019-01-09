#include "utility.h"
#include <queue>

#define MY_PROFILE_TIMING

short SqrtFast(short sValue)
{

	return(0);
}

unsigned char CalculateAverage(int width, int height, unsigned char *pcData){
	long long avg = 0;
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			avg += pcData[i*width + j];
		}
	}
	avg = avg / (height*width);
	return avg;
}
unsigned char CalculateAverageWithStride(int width, int height, int offsetX, int offsetY, int xStride, unsigned char *pcData){
	long long avg = 0;
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			avg += pcData[((i + offsetY)*xStride) + j + offsetX];
		}
	}
	avg = avg / (height*width);
	return avg;
}
void AddPadding(int width, int height, int offsetX, int offsetY, int xStride, int padWidth, unsigned char* pcData){
	unsigned char* pcImage = pcData + xStride * offsetY + offsetX;
	//pad left
	for (int i = 0; i < height; i++){
		for (int j = 1; j <= padWidth; j++) {
			pcImage[-j + i * xStride] = pcImage[i * xStride];
		}
	}
	//pad right
	for (int i = 0; i < height; i++) {
		for (int j = 1; j <= padWidth; j++) {
			pcImage[j + i * xStride + width - 1] = pcImage[i * xStride + width - 1];
		}
	}
	//pad up
	for (int i = 1; i <= padWidth; i++) {
		memcpy( pcImage-padWidth - i * xStride,pcImage - padWidth, width + padWidth*2);
	}
	//pad down
	for (int i = 1; i <= padWidth; i++) {
		memcpy(pcImage + (height - 1 + i)*xStride- padWidth, pcImage + (height-1) * xStride- padWidth, width + padWidth * 2);
	}
}

void AddPaddingInt(int width, int height, int offsetX, int offsetY, int xStride, int padWidth, int* piData) {
	int* piImage = piData + xStride * offsetY + offsetX;
	//pad left
	for (int i = 0; i < height; i++) {
		for (int j = 1; j <= padWidth; j++) {
			piImage[-j + i * xStride] = piImage[i * xStride];
		}
	}
	//pad right
	for (int i = 0; i < height; i++) {
		for (int j = 1; j <= padWidth; j++) {
			piImage[j + i * xStride + width - 1] = piImage[i * xStride + width - 1];
		}
	}
	//pad up
	for (int i = 1; i <= padWidth; i++) {
		memcpy(piImage - padWidth - i * xStride, piImage - padWidth, (width + padWidth * 2 ) * sizeof(int));
	}
	//pad down
	for (int i = 1; i <= padWidth; i++) {
		memcpy(piImage + (height - 1 + i)*xStride - padWidth, piImage + (height - 1) * xStride - padWidth, (width + padWidth * 2) * sizeof(int));
	}
}

void AddPadding255(int width, int height, int offsetX, int offsetY, int xStride, int padWidth, unsigned char* pcData) {
	unsigned char* pcImage = pcData + xStride * offsetY + offsetX;
	//pad left
	for (int i = 0; i < height; i++)
		memset(pcImage + i * xStride - padWidth, 255, padWidth);

	//pad right
	for (int i = 0; i < height; i++)
		memset(pcImage + i * xStride + width, 255, padWidth);

	//pad up
	for (int i = 1; i <= padWidth; i++)
		memset(pcImage - padWidth - i * xStride, 255, width + padWidth * 2);

	//pad down
	for (int i = 1; i <= padWidth; i++)
		memset(pcImage + (height - 1 + i)*xStride - padWidth, 255, width + padWidth * 2);
}

void AddPadding0(int width, int height, int offsetX, int offsetY, int xStride, int padWidth, unsigned char* pcData) {
	unsigned char* pcImage = pcData + xStride * offsetY + offsetX;
	//pad left
	for (int i = 0; i < height; i++)
		memset(pcImage + i * xStride - padWidth, 0, padWidth);

	//pad right
	for (int i = 0; i < height; i++)
		memset(pcImage + i * xStride + width, 0, padWidth);

	//pad up
	for (int i = 1; i <= padWidth; i++)
		memset(pcImage - padWidth - i * xStride, 0, width + padWidth * 2);

	//pad down
	for (int i = 1; i <= padWidth; i++)
		memset(pcImage + (height - 1 + i)*xStride - padWidth, 0, width + padWidth * 2);
}

void FilterImage(int* filter, int dimensions, int divisor, int width, int height, int xStride, int offsetX, int offsetY, unsigned char* pcImageSource, unsigned char* pcImageDest){
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int sum = 0;
			for (int k = 0; k < dimensions; k++) {
				for (int m = 0; m < dimensions; m++) {
					sum += pcImageSource[(i- (dimensions / 2))*xStride + j - (dimensions/2) + k*xStride + m] * filter[k*dimensions + m];
				}
			}
			pcImageDest[i*xStride+j] = clamp255(sum / divisor);
		}
	}
}
int LocalMean(int dimensions, int widthposition, int heightposition, int xStride, int offsetX, int offsetY, unsigned char* pcData) {
	int mean=0;
	int sum=0;
	for (int k = 0; k < dimensions; k++) {
		for (int m = 0; m < dimensions; m++) {
			sum += pcData[(heightposition - (dimensions / 2))*xStride + widthposition - (dimensions / 2) + k * xStride + m];
		}
	}
	mean = sum / (dimensions*dimensions);
	return mean;
}
float LocalStandardDeviation(int dimensions, int widthposition, int heightposition, int xStride, int offsetX, int offsetY, unsigned char* pcData) {
	int mean=0;
	int sum=0;
	float standardDeviation=0;
	for (int k = 0; k < dimensions; k++) {
		for (int m = 0; m < dimensions; m++) {
			sum += pcData[(heightposition - (dimensions / 2))*xStride + widthposition - (dimensions / 2) + k * xStride + m];
		}
	}
	mean = sum / (dimensions*dimensions);
	for (int k = 0; k < dimensions; k++) {
		for (int m = 0; m < dimensions; m++) {
			standardDeviation += pow(pcData[(heightposition - (dimensions / 2))*xStride + widthposition - (dimensions / 2) + k * xStride + m] - mean, 2);
		}
	}
	return sqrt(standardDeviation /(dimensions*dimensions));
}
void FilterImage(int* filter, int dimensions, int divisor, int width, int height, int xStride, int offsetX, int offsetY, unsigned char* pcImageSource, int* pcImageDest) {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int sum = 0;
			for (int k = 0; k < dimensions; k++) {
				for (int m = 0; m < dimensions; m++) {
					sum += pcImageSource[(i - (dimensions / 2))*xStride + j - (dimensions / 2) + k * xStride + m] * filter[k*dimensions + m];
				}
			}
			pcImageDest[i*xStride + j] = sum / divisor;
		}
	}
}
void ErodeImageWithElement(int* element, int dimensions, int divisor, int width, int height, int xStride, int offsetX, int offsetY, unsigned char* pcImageSource, unsigned char* pcImageDest) {
	memset(pcImageDest, 255, xStride*(height + offsetY*2));
	unsigned char *dst;

	for (int i = 0; i < height; i++) {
		dst = pcImageDest + (i + offsetY)*xStride + offsetX;
		for (int j = 0; j < width; j++) {
			bool hit = true;
			for (int k = 0; k < dimensions; k++) {
				for (int m = 0; m < dimensions; m++) {
					int xPos = j - dimensions / 2 + m;
					int yPos = i - dimensions / 2 + k;
					if (element[k*dimensions + m] == 1) {
						if (pcImageSource[(yPos + offsetY)*xStride + xPos + offsetX] != 255) {
							*dst = 0;
							hit = false;
							break;
						}
					}
				}
				if (hit == false) {
					break;
				}
			}
			dst++;
		}
	}
}
void ErodeImageWithElementWithoutPadding(int* element, int dimensions, int divisor, int width, int height, unsigned char* pcImageSource, unsigned char* pcImageDest) {
	memset(pcImageDest, 0, width * height);
	unsigned char *dst;

	for (int i = 0; i < height; i++) {
		dst = pcImageDest + i * width;
		for (int j = 0; j < width; j++) {
			bool hit = true;
			for (int k = 0; k < dimensions; k++) {
				for (int m = 0; m < dimensions; m++) {
					int xPos = j - dimensions / 2 + m;
					int yPos = i - dimensions / 2 + k;
					if (element[k*dimensions + m] == 1) {
						if (xPos >= 0 && xPos < width && yPos >= 0 && yPos < height && pcImageSource[(yPos)*width + xPos] != 255) {
							*dst = 0;
							hit = false;
							break;
						}
					}
				}
				if (hit == false) {
					break;
				}
			}
			dst++;
		}
	}
}
void DilateImageWithElement(int* element, int dimensions, int divisor, int width, int height, int xStride, int offsetX, int offsetY, unsigned char* pcImageSource, unsigned char* pcImageDest) {
	memset(pcImageDest, 0, xStride*(height + offsetY * 2));
	unsigned char *dst;
	for (int i = 0; i < height; i++) {
		dst = pcImageDest + (i + offsetY)*xStride + offsetX;
		for (int j = 0; j < width; j++) {
			bool hit = true;
			for (int k = 0; k < dimensions; k++) {
				for (int m = 0; m < dimensions; m++) {
					int xPos = j - dimensions / 2 + m;
					int yPos = i - dimensions / 2 + k;
					if (element[k*dimensions + m] == 1) {
						if (pcImageSource[(yPos + offsetY)*xStride + xPos + offsetX] == 255) {
							*dst = 255;
							hit = false;
							break;
						}
					}
				}
				if (hit == false) {
					break;
				}
			}
			dst++;
		}
	}
}

void DilateImageWithElementWithoutPadding(int* element, int dimensions, int divisor, int width, int height, unsigned char* pcImageSource, unsigned char* pcImageDest) {
	memset(pcImageDest, 0, width * height);
	unsigned char *dst;
	for (int i = 0; i < height; i++) {
		dst = pcImageDest + i*width;
		for (int j = 0; j < width; j++) {
			bool hit = true;
			for (int k = 0; k < dimensions; k++) {
				for (int m = 0; m < dimensions; m++) {
					int xPos = j - dimensions / 2 + m;
					int yPos = i - dimensions / 2 + k;
					if (element[k*dimensions + m] == 1) {
						if (xPos >= 0 && xPos < width && yPos >= 0 && yPos < height && pcImageSource[(yPos)*width + xPos] == 255) {
							*dst = 255;
							hit = false;
							break;
						}
					}
				}
				if (hit == false) {
					break;
				}
			}
			dst++;
		}
	}
}
int* CreateHistogram(int buckets, int width, int height, int xStride, int offsetX, int offsetY, unsigned char*pcData) {
	int* pHistogram = (int*)malloc(buckets*4);
	memset(pHistogram, 0, buckets*4);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			 int pixeldata = pcData[((i + offsetY)*xStride) + j + offsetX]/(256/buckets);
			 pHistogram[pixeldata]++;
		}
	}
	return pHistogram;
}

void NormalizeMinMax(int width, int height, int offsetX, int offsetY, int xStride, int* GradientData, 	unsigned char* pcNew)
{
	float min = 0.;
	float max = 0.;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (GradientData[((i + offsetY)*xStride) + j + offsetX] > max)
			{
				max = GradientData[((i + offsetY)*xStride) + j + offsetX];
			}
			if (GradientData[((i + offsetY)*xStride) + j + offsetX] < min)
			{
				min = GradientData[((i + offsetY)*xStride) + j + offsetX];
			}
			
		}
	}
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			pcNew[((i + offsetY)*xStride) + j + offsetX] = (GradientData[((i + offsetY)*xStride) + j + offsetX] - min) * 255 / (max - min);
		}
	}
}
unsigned char *ImageGetRegionsWithHighBlockContrast4QRCode(int iBlockSize, int iRegionWidth, int iRegionHeight, int iRegionThreshold, int iContrastThreshold, 
	int width, int height, int offsetX, int offsetY, int xStride, int padWidth, unsigned char* pcData)
{
	int i, j;
	int widthInBlocks, heightInBlocks;
	unsigned char *pucBlockContrast;
	unsigned char *pucTemp;
	int piElement[49] = { 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1 ,1 ,1
	};

	widthInBlocks = width / iBlockSize;
	heightInBlocks = height / iBlockSize;
	pucBlockContrast = (unsigned char *)malloc(widthInBlocks * heightInBlocks);
	ImageCalBlockContrast(pucBlockContrast, iBlockSize, iContrastThreshold, width, height, offsetX, offsetY, xStride, padWidth, pcData);
	for (i = 200; i < heightInBlocks; i++)
	{
		for (j = 0; j < widthInBlocks; j++)
		{
			pucBlockContrast[i * widthInBlocks + j] = 0;
		}
	}
#if 0
	return(pucBlockContrast);
#endif
	pucTemp = (unsigned char *)malloc(widthInBlocks * heightInBlocks);
	memset(pucTemp, 0, widthInBlocks * heightInBlocks);
	DilateImageWithElementWithoutPadding(piElement, 7, 1, widthInBlocks, heightInBlocks, pucBlockContrast, pucTemp);
	return(pucTemp);
}

void ImageCalBlockContrast(unsigned char *pucContrast, int iBlockSize, int iThreshold, 
	int width, int height, int offsetX, int offsetY, int xStride, int padWidth, unsigned char* pcData)
{
	// block contrast can be evaluated at different block sizes
	// for every block, its basic contrast data includes two most dominant luma values,
	//	and the standard deviation from them to their average
	// the final contrast value is based on this deviation, and its difference from all its neighbors
	int i, j, k, l;
	int iDataWidth, iDataHeight;
	unsigned char *pucBlockStart;
	unsigned char *pucUpper, *pucLower;
	int *piNumLower, *piNumUpper;
	int *piDeviation;
	int iContrastNeighborhood;
	int iSum, iSumLower, iSumUpper;
	int iNumLower, iNumUpper;
	int iMaxDeviation;
	int iLowerCenterX, iLowerCenterY;
	int iUpperCenterX, iUpperCenterY;
	int iLowerDistance, iUpperDistance;
	iDataWidth = width / iBlockSize;
	iDataHeight = height / iBlockSize;
	pucUpper = (unsigned char *)malloc(iDataWidth * iDataHeight);
	pucLower = (unsigned char *)malloc(iDataWidth * iDataHeight);
	piNumLower = (int *)calloc(iDataWidth * iDataHeight, sizeof(int));
	piNumUpper = (int *)calloc(iDataWidth * iDataHeight, sizeof(int));
	piDeviation = (int *)calloc(iDataWidth * iDataHeight, sizeof(int));
	memset(pucContrast, 0, iDataWidth * iDataHeight);
	memset(pucUpper, 0, iDataWidth * iDataHeight);
	memset(pucLower, 0, iDataWidth * iDataHeight);

	memset(piNumLower, 0, iDataWidth * iDataHeight * sizeof(int));
	memset(piNumUpper, 0, iDataWidth * iDataHeight * sizeof(int));
	memset(piDeviation, 0, iDataWidth * iDataHeight * sizeof(int));

	for (i = 0; i < iDataHeight; i++)
	{
		for (j = 0; j < iDataWidth; j++)
		{
			pucBlockStart = pcData + (offsetY + i * iBlockSize) * xStride + j * iBlockSize + offsetX;
			iSum = 0;
			for (k = 0; k < iBlockSize; k++)
			{
				for (l = 0; l < iBlockSize; l++)
				{
					iSum += pucBlockStart[k * xStride + l];
				}
			}
			iSum = iSum / (iBlockSize * iBlockSize);	// iSum = average value of the block
			iSumLower = iSumUpper = 0;
			iNumLower = iNumUpper = 0;

			// divide the block into two groups (low & high)
			iLowerCenterX = iLowerCenterY = 0;
			iUpperCenterX = iUpperCenterY = 0;
			for (k = 0; k < iBlockSize; k++)
			{
				for (l = 0; l < iBlockSize; l++)
				{
					if (pucBlockStart[k * xStride + l] < iSum)
					{
						iSumLower += pucBlockStart[k * xStride + l];
						iLowerCenterX += l;
						iLowerCenterY += k;
						iNumLower++;
					}
					else
					{
						iSumUpper += pucBlockStart[k * xStride + l];
						iUpperCenterX += l;
						iUpperCenterY += k;
						iNumUpper++;
					}
				}
			}
			if (iNumLower > 0)
			{
				pucLower[i * iDataWidth + j] = iSumLower / iNumLower;
				iLowerCenterX /= iNumLower;
				iLowerCenterY /= iNumLower;
			}
			else
			{
				iLowerCenterX = (iBlockSize + 1) / 2;
				iLowerCenterY = (iBlockSize + 1) / 2;
			}
			if (iNumUpper > 0)
			{
				pucUpper[i * iDataWidth + j] = iSumUpper / iNumUpper;
				iUpperCenterX /= iNumUpper;
				iUpperCenterY /= iNumUpper;
			}
			else
			{
				iUpperCenterX = (iBlockSize + 1) / 2;
				iUpperCenterY = (iBlockSize + 1) / 2;
			}

			// check if each group is scattered or not
			iLowerDistance = 1;
			iUpperDistance = 1;
			for (k = 0; k < iBlockSize; k++)
			{
				for (l = 0; l < iBlockSize; l++)
				{
					if (pucBlockStart[k * xStride + l] < iSum)
					{
						iLowerDistance += (l - iLowerCenterX) * (l - iLowerCenterX) + (k - iLowerCenterY) * (k - iLowerCenterY);
					}
					else
					{
						iUpperDistance += (l - iUpperCenterX) * (l - iUpperCenterX) + (k - iUpperCenterY) * (k - iUpperCenterY);
					}
				}
			}

			if (iNumLower) iLowerDistance /= iNumLower;
			if (iNumUpper) iUpperDistance /= iNumUpper;
			piNumLower[i * iDataWidth + j] = iNumLower;
			piNumUpper[i * iDataWidth + j] = iNumUpper;
			piDeviation[i * iDataWidth + j]
				= (pucLower[i * iDataWidth + j] - iSum) * (pucLower[i * iDataWidth + j] - iSum) * iNumLower
				+ (pucUpper[i * iDataWidth + j] - iSum) * (pucUpper[i * iDataWidth + j] - iSum) * iNumUpper;
			piDeviation[i * iDataWidth + j] = piDeviation[i * iDataWidth + j] * 256 / (iLowerDistance * iUpperDistance);
		}
	}

	// smooth out with neighbors
	for (i = 1; i < iDataHeight - 1; i++)
	{
		for (j = 1; j < iDataWidth - 1; j++)
		{
			iNumUpper = piNumUpper[i * iDataWidth + j]
				+ piNumUpper[i * iDataWidth + j - 1]
				+ piNumUpper[i * iDataWidth + j + 1]
				+ piNumUpper[(i - 1) * iDataWidth + j]
				+ piNumUpper[(i + 1) * iDataWidth + j];
			iSumUpper = piNumUpper[i * iDataWidth + j] * pucUpper[i * iDataWidth + j]
				+ piNumUpper[i * iDataWidth + j - 1] * pucUpper[i * iDataWidth + j - 1]
				+ piNumUpper[i * iDataWidth + j + 1] * pucUpper[i * iDataWidth + j + 1]
				+ piNumUpper[(i - 1) * iDataWidth + j] * pucUpper[(i - 1) * iDataWidth + j]
				+ piNumUpper[(i + 1) * iDataWidth + j] * pucUpper[(i + 1) * iDataWidth + j];

			iNumLower = piNumLower[i * iDataWidth + j]
				+ piNumLower[i * iDataWidth + j - 1]
				+ piNumLower[i * iDataWidth + j + 1]
				+ piNumLower[(i - 1) * iDataWidth + j]
				+ piNumLower[(i + 1) * iDataWidth + j];
			iSumLower = piNumLower[i * iDataWidth + j] * pucLower[i * iDataWidth + j]
				+ piNumLower[i * iDataWidth + j - 1] * pucLower[i * iDataWidth + j - 1]
				+ piNumLower[i * iDataWidth + j + 1] * pucLower[i * iDataWidth + j + 1]
				+ piNumLower[(i - 1) * iDataWidth + j] * pucLower[(i - 1) * iDataWidth + j]
				+ piNumLower[(i + 1) * iDataWidth + j] * pucLower[(i + 1) * iDataWidth + j];
			iSum = (iSumLower + iSumUpper) / (iNumLower + iNumUpper);
			if (iNumLower) iSumLower /= iNumLower;
			if (iSumUpper) iSumUpper /= iNumUpper;
			iContrastNeighborhood = (iSumLower - iSum) * (iSumLower - iSum) * iNumLower
				+ (iSumUpper - iSum) * (iSumUpper - iSum) * iNumUpper;

			piDeviation[i * iDataWidth + j] += iContrastNeighborhood;
		}
	}

	// normalize
	iMaxDeviation = 0;
	for (i = 0; i < iDataHeight; i++)
	{
		for (j = 0; j < iDataWidth; j++)
		{
			if (piDeviation[i * iDataWidth + j] > iMaxDeviation)
				iMaxDeviation = piDeviation[i * iDataWidth + j];
		}
	}

	for (i = 0; i < iDataHeight; i++)
	{
		if (i == 30) { 
			int tester = 3;
		}
		for (j = 0; j < iDataWidth; j++)
		{
			pucContrast[i * iDataWidth + j] = 255 * piDeviation[i * iDataWidth + j] / iMaxDeviation;
			if (pucContrast[i * iDataWidth + j] < iThreshold)
				pucContrast[i * iDataWidth + j] = 0;
			else
				pucContrast[i * iDataWidth + j] = 255;

		}
	}

	free(pucUpper);
	free(pucLower);
	free(piNumLower);
	free(piNumUpper);
	free(piDeviation);
}

//void ImageFindContainingQuadrilateral(Point * pQuadrilateral, unsigned char * pucMask, int iMaskBlockSize, MyImage *pImage, int width, int height, int offsetX, int offsetY, int xStride, unsigned char * pcData)
void ImageFindContainingQuadrilateral(Point *pQuadrilateral, unsigned char *pucMask, int iMaskBlockSize, MyImage &image)
{
	// the idea here is to find the black pixels of the target, then find the enclosing quadrilateral.
	//	for license plate the bottom is often connected to black boundary. so the white pixels are checked and
	//	a line formed by the white pixels is drawed as the boundary to filter out non-target black pixels first.
	ImageConfig imageConfig;
	MyImage newImage;
	int i, j, k, l;
	int iScore, iBestStart, iBestEnd, iBest0, iBest1;
	int iLeft, iRight, iTop, iBottom;
	//Point topLeftBlock, topRightBlock, bottomLeftBlock, bottomRightBlock;
	int widthInBlocks, heightInBlocks;
	unsigned char *pucStarted;
	unsigned char ucPixelValue;
	Point topLeft, topRight, bottomLeft, bottomRight;
	Point start, end;

	widthInBlocks = image.width / iMaskBlockSize;
	heightInBlocks = image.height / iMaskBlockSize;

	image.imageCopyConfig(imageConfig);
	imageConfig.ifClear = 2;
	newImage.Init(imageConfig);

	pucStarted = (unsigned char *)malloc(image.height);
	memset(pucStarted, 0, image.height);

	// from left to right, start with the first black pixel follows white
	for (i = 0; i < heightInBlocks; i++)
	{
		for (j = 0; j < widthInBlocks; j++)
		{
			if (pucMask[i * widthInBlocks + j])
			{
				// from left to right, start with the first black pixel follows white
				for (k = 0; k < iMaskBlockSize; k++)
				{
					for (l = 0; l < iMaskBlockSize; l++)
					{
						ucPixelValue = image.pucData[0][(i * iMaskBlockSize + k + image.offsetY) * image.xStride + j * iMaskBlockSize + l + image.offsetX];
						if (pucStarted[i * iMaskBlockSize + k] == 0 && ucPixelValue == 255)
							pucStarted[i * iMaskBlockSize + k] = 1;
						if (pucStarted[i * iMaskBlockSize + k] == 1 && ucPixelValue == 0)
							pucStarted[i * iMaskBlockSize + k] = 2;
						if (pucStarted[i * iMaskBlockSize + k] == 2 && ucPixelValue == 0)
						{
							newImage.pucData[0][(i * iMaskBlockSize + k + image.offsetY) * image.xStride + j * iMaskBlockSize + l + image.offsetX]
								= 0;
						}
					}
				}
			}
		}
	}
#if 0
	memcpy(image.pucData[0], newImage.pucData[0], image.xStride * (image.height + image.offsetY));
	return;
#endif
	// from right to left, start with the first black pixel follows white
	memset(pucStarted, 0, image.height);
	for (i = 0; i < heightInBlocks; i++)
	{
		for (j = widthInBlocks - 1; j >= 0; j--)
		{
			if (pucMask[i * widthInBlocks + j])
			{
				for (k = 0; k < iMaskBlockSize; k++)
				{
					for (l = iMaskBlockSize - 1; l >= 0; l--)
					{
						ucPixelValue = image.pucData[0][(i * iMaskBlockSize + k + image.offsetY) * image.xStride + j * iMaskBlockSize + l + image.offsetX];
						if (pucStarted[i * iMaskBlockSize + k] == 0 && ucPixelValue == 255)
							pucStarted[i * iMaskBlockSize + k] = 1;
						if (pucStarted[i * iMaskBlockSize + k] == 0)
						{
							newImage.pucData[0][(i * iMaskBlockSize + k + image.offsetY) * image.xStride + j * iMaskBlockSize + l + image.offsetX]
								= 255;
						}
					}
				}
			}
		}
	}
#if 0
	memcpy(image.pucData[0], newImage.pucData[0], image.xStride * (image.height + image.offsetY));
	return;
#endif
	// from top to bottom, start with the first black pixel follows white
	memset(pucStarted, 0, image.width);
	for (j = 0; j < widthInBlocks; j++)
	{
		for (i = 0; i < heightInBlocks; i++)
		{
			if (pucMask[i * widthInBlocks + j])
			{
				// from top to bottom, start with the first black pixel follows white
				for (l = 0; l < iMaskBlockSize; l++)
				{
					for (k = 0; k < iMaskBlockSize; k++)
					{
						ucPixelValue = image.pucData[0][(i * iMaskBlockSize + k + image.offsetY) * image.xStride + j * iMaskBlockSize + l + image.offsetX];
						if (pucStarted[j * iMaskBlockSize + l] == 0 && ucPixelValue == 255)
							pucStarted[j * iMaskBlockSize + l] = 1;
						if (pucStarted[j * iMaskBlockSize + l] == 0)
						{
							newImage.pucData[0][(i * iMaskBlockSize + k + image.offsetY) * image.xStride + j * iMaskBlockSize + l + image.offsetX]
								= 255;
						}
					}
				}
			}
		}
	}
	
	// from bottom to top, start with the first black pixel follows white
	memset(pucStarted, 0, image.width);
	for (j = 0; j < widthInBlocks; j++)
	{
		for (i = heightInBlocks; i >= 0; i--)
		{
			if (pucMask[i * widthInBlocks + j])
			{
				// from bottom to top, start with the first black pixel follows white
				for (l = 0; l < iMaskBlockSize; l++)
				{
					for (k = iMaskBlockSize - 1; k >= 0; k--)
					{
						ucPixelValue = image.pucData[0][(i * iMaskBlockSize + k + image.offsetY) * image.xStride + j * iMaskBlockSize + l + image.offsetX];
						if (pucStarted[j * iMaskBlockSize + l] == 0 && ucPixelValue == 255)
							pucStarted[j * iMaskBlockSize + l] = 1;
						if (pucStarted[j * iMaskBlockSize + l] == 0)
						{
							newImage.pucData[0][(i * iMaskBlockSize + k + image.offsetY) * image.xStride + j * iMaskBlockSize + l + image.offsetX]
								= 255;
						}
					}
				}
			}
		}
	}
	newImage.Dilate(3);
	newImage.Erode(3);
#if 0
	memcpy(image.pucData[0], newImage.pucData[0], image.xStride * (image.height + image.offsetY));
	return;
#endif
#if 1
	iLeft = widthInBlocks * iMaskBlockSize - 1;
	iRight = 0;
	iTop = heightInBlocks * iMaskBlockSize - 1;
	iBottom = 0;
	for (i = 0; i < heightInBlocks * iMaskBlockSize; i++)
	{
		for (j = 0; j < widthInBlocks * iMaskBlockSize; j++)
		{
			ucPixelValue = newImage.pucData[0][(i + image.offsetY) * image.xStride + j + image.offsetX];
			if (ucPixelValue == 0 && j < iLeft)
				iLeft = j;
			if (ucPixelValue == 0 && j > iRight)
				iRight = j;
			if (ucPixelValue == 0 && i < iTop)
				iTop = i;
			if (ucPixelValue == 0 && i > iBottom)
				iBottom = i;
		}
	}
	iTop -= 8;
	iBottom += 8;

	// search for best topleft
#if 1
	iScore = 0x7fffffff;
	for (i = iTop / iMaskBlockSize; i < heightInBlocks; i++)
	{
		for (j = iLeft / iMaskBlockSize; j < widthInBlocks; j++)
		{
			if (pucMask[i * widthInBlocks + j])
			{
				int iCurScore = abs(i - iTop / iMaskBlockSize)/* *(i - iTop / iMaskBlockSize) */
					+ (j - (iLeft + iMaskBlockSize / 2) / iMaskBlockSize) * (j - (iLeft + iMaskBlockSize / 2) / iMaskBlockSize);
				if (iCurScore < iScore)
				{
					iScore = iCurScore;
					iBest0 = j;
					iBest1 = i;
				}
			}
		}
	}
	topLeft.iX = iBest0 * iMaskBlockSize;
	topLeft.iY = iBest1 * iMaskBlockSize;
#else
	topLeft.iX = iLeft;
	topLeft.iY = iTop;
	for (i = 0; i < heightInBlocks; i++)
	{
		if (pucMask[i * widthInBlocks + iLeft / iMaskBlockSize])
		{
			topLeft.iY = i * iMaskBlockSize;
			break;
		}
	}
#endif

	topRight.iX = iRight;
	topRight.iY = iTop;
#if 1
	iScore = 0x7fffffff;
	for (i = iTop / iMaskBlockSize; i < heightInBlocks; i++)
	{
		for (j = iLeft / iMaskBlockSize; j < widthInBlocks; j++)
		{
			if (pucMask[i * widthInBlocks + j])
			{
				int iCurScore = abs(i - iTop / iMaskBlockSize)/* *(i - iTop / iMaskBlockSize) */
					+ (j - (iRight + iMaskBlockSize / 2) / iMaskBlockSize) * (j - (iRight + iMaskBlockSize / 2) / iMaskBlockSize);
				if (iCurScore < iScore)
				{
					iScore = iCurScore;
					iBest0 = j;
					iBest1 = i;
				}
			}
		}
	}
	topRight.iX = iBest0 * iMaskBlockSize;
	topRight.iY = iBest1 * iMaskBlockSize;
#else
	for (i = 0; i < heightInBlocks; i++)
	{
		if (pucMask[i * widthInBlocks + iRight / iMaskBlockSize])
		{
			topRight.iY = i * iMaskBlockSize;
			break;
		}
	}
#endif
	bottomLeft.iX = iLeft;
	bottomLeft.iY = iBottom;
#if 1
	iScore = 0x7fffffff;
	for (i = iTop / iMaskBlockSize; i < heightInBlocks; i++)
	{
		for (j = iLeft / iMaskBlockSize; j < widthInBlocks; j++)
		{
			if (pucMask[i * widthInBlocks + j])
			{
				int iCurScore = abs(i - iBottom / iMaskBlockSize)/* *(i - iBottom / iMaskBlockSize) */
					+ (j - (iLeft + iMaskBlockSize / 2) / iMaskBlockSize) * (j - (iLeft + iMaskBlockSize / 2) / iMaskBlockSize);
				if (iCurScore < iScore)
				{
					iScore = iCurScore;
					iBest0 = j;
					iBest1 = i;
				}
			}
		}
	}
	bottomLeft.iX = iBest0 * iMaskBlockSize;
	bottomLeft.iY = iBest1 * iMaskBlockSize;
#else
	for (i = heightInBlocks - 1; i >= 0; i--)
	{
		if (pucMask[i * widthInBlocks + iLeft / iMaskBlockSize])
		{
			bottomLeft.iY = (i + 1) * iMaskBlockSize - 1;
			break;
		}
	}
#endif

	bottomRight.iX = iRight;
	bottomRight.iY = iBottom;
#if 1
	iScore = 0x7fffffff;
	for (i = iTop / iMaskBlockSize; i < heightInBlocks; i++)
	{
		for (j = iLeft / iMaskBlockSize; j < widthInBlocks; j++)
		{
			if (pucMask[i * widthInBlocks + j])
			{
#if 1
				// for qrcode
				int iCurScore = (i - (iBottom + iMaskBlockSize / 2) / iMaskBlockSize) * (i - (iBottom + iMaskBlockSize / 2) / iMaskBlockSize)
					+ abs(j - (iRight + iMaskBlockSize / 2) / iMaskBlockSize)/* * (j - (iRight + iMaskBlockSize / 2) / iMaskBlockSize)*/;
#else
				// for lp
				int iCurScore = abs(i - iBottom / iMaskBlockSize)/* *(i - iBottom / iMaskBlockSize) */
					+ (j - (iRight + iMaskBlockSize / 2) / iMaskBlockSize) * (j - (iRight + iMaskBlockSize / 2) / iMaskBlockSize);
#endif
				if (iCurScore < iScore)
				{
					iScore = iCurScore;
					iBest0 = j;
					iBest1 = i;
				}
			}
		}
	}
	bottomRight.iX = iBest0 * iMaskBlockSize;
	bottomRight.iY = iBest1 * iMaskBlockSize;
#else
	for (i = heightInBlocks - 1; i >= 0; i--)
	{
		if (pucMask[i * widthInBlocks + iRight / iMaskBlockSize])
		{
			bottomRight.iY = (i + 1) * iMaskBlockSize - 1;
			break;
		}
	}
#endif
	pQuadrilateral[0] = topLeft;
	pQuadrilateral[1] = topRight;
	pQuadrilateral[2] = bottomRight;
	pQuadrilateral[3] = bottomLeft;
	//return;

	iScore = -1;
	iBestStart = -1;
	iBestEnd = -1;
	for (i = -64; i < 64; i++)
	{
		for (j = -64; j < 64; j++)
		{
			start.iX = topLeft.iX;
			start.iY = topLeft.iY + i;
			end.iX = topRight.iX;
			end.iY = topRight.iY + j;
			if (start.iY == 330 && end.iY == 561)
			{
				int kkk = 0;
				kkk++;
			}
			if (IfWhiteLine(start, end, widthInBlocks * iMaskBlockSize, heightInBlocks * iMaskBlockSize, newImage.xStride, newImage.offsetX, newImage.offsetY, newImage.pucData[0]))
			{
				if (start.iY + end.iY > iScore)
				{
					iScore = start.iY + end.iY;
					iBestStart = start.iY;
					iBestEnd = end.iY;
					pQuadrilateral[0] = start;
					pQuadrilateral[1] = end;
				}
			}
		}
	}

	iScore = bottomLeft.iY + 32 + bottomRight.iY + 32;
	iBestStart = -1;
	iBestEnd = -1;
#if 0
	// for LP
	for (i = -64; i < 64; i++)
#else
	// for qrcode
	for (i = -100; i < 64; i++)
#endif
	{
		for (j = -68; j < 68; j++)
		{
			start.iX = bottomLeft.iX;
			start.iY = bottomLeft.iY + i;
			end.iX = bottomRight.iX;
			end.iY = bottomRight.iY + j;
			if (start.iY == 1316 && end.iY == 1176)
			{
				int kkk = 0;
				kkk++;
			}
			if (IfWhiteLine(start, end, widthInBlocks * iMaskBlockSize, heightInBlocks * iMaskBlockSize, newImage.xStride, newImage.offsetX, newImage.offsetY, newImage.pucData[0]))
			{
				if (start.iY + end.iY < iScore)
				{
					iScore = start.iY + end.iY;
					iBestStart = start.iY;
					iBestEnd = end.iY;
					pQuadrilateral[3] = start;
					pQuadrilateral[2] = end;
				}
			}
		}
	}

	// clean pixels outside of the boundary
	for (i = 0; i < heightInBlocks * iMaskBlockSize; i++)
	{
		for (j = 0; j < widthInBlocks * iMaskBlockSize; j++)
		{
			if (i < pQuadrilateral[0].iY + (j - pQuadrilateral[0].iX) * (pQuadrilateral[1].iY - pQuadrilateral[0].iY) / (pQuadrilateral[1].iX - pQuadrilateral[0].iX))
				newImage.pucData[0][(i + image.offsetY) * image.xStride + j + image.offsetX] = 255;
			if (i > pQuadrilateral[3].iY + (j - pQuadrilateral[3].iX) * (pQuadrilateral[2].iY - pQuadrilateral[3].iY) / (pQuadrilateral[2].iX - pQuadrilateral[3].iX))
				newImage.pucData[0][(i + image.offsetY) * image.xStride + j + image.offsetX] = 255;
		}
	}

	topLeft = pQuadrilateral[0];
	topRight = pQuadrilateral[1];
	bottomRight = pQuadrilateral[2];
	bottomLeft = pQuadrilateral[3];
	// move left boundary
	iScore = topLeft.iX + bottomLeft.iX;
	for (i = -16; i < 16; i++)
	{
		for (j = -16; j < 16; j++)
		{
			start.iX = topLeft.iX + i;
			start.iY = topLeft.iY + i * (topRight.iY - topLeft.iY) / (topRight.iX - topLeft.iX);
			end.iX = bottomLeft.iX + j;
			end.iY = bottomLeft.iY + j * (bottomRight.iY - bottomLeft.iY) / (bottomRight.iX - bottomLeft.iX);
			if (IfWhiteLine(start, end, widthInBlocks * iMaskBlockSize, heightInBlocks * iMaskBlockSize, newImage.xStride, newImage.offsetX, newImage.offsetY, newImage.pucData[0]))
			{
				if (start.iX + end.iX > iScore)
				{
					iScore = start.iX + end.iX;
					iBestStart = start.iX;
					iBestEnd = end.iX;
					pQuadrilateral[0] = start;
					pQuadrilateral[3] = end;
				}
			}
		}
	}

	// move right boundary
	iScore = topRight.iX + bottomRight.iX;
	for (i = -16; i < 16; i++)
	{
		for (j = -16; j < 16; j++)
		{
			start.iX = topRight.iX + i;
			start.iY = topRight.iY + i * (topRight.iY - topLeft.iY) / (topRight.iX - topLeft.iX);
			end.iX = bottomRight.iX + j;
			end.iY = bottomRight.iY + j * (bottomRight.iY - bottomLeft.iY) / (bottomRight.iX - bottomLeft.iX);
			if (IfWhiteLine(start, end, widthInBlocks * iMaskBlockSize, heightInBlocks * iMaskBlockSize, newImage.xStride, newImage.offsetX, newImage.offsetY, newImage.pucData[0]))
			{
				if (start.iX + end.iX < iScore)
				{
					iScore = start.iX + end.iX;
					iBestStart = start.iX;
					iBestEnd = end.iX;
					pQuadrilateral[1] = start;
					pQuadrilateral[2] = end;
				}
			}
		}
	}
#endif
	memcpy(image.pucData[0], newImage.pucData[0], image.xStride * (image.height + image.offsetY));
}

int IfWhiteLine(Point start, Point end, int iWidth, int iHeight, int iStride, int iStartX, int iStartY, unsigned char *pucImage)
{
	int i, j;
	int iNumErrors = 0;
	if (abs(end.iX - start.iX) > abs(end.iY - start.iY))
	{
		for (i = start.iX; i < end.iX; i++)
		{
			j = start.iY + (end.iY - start.iY) * (i - start.iX) / (end.iX - start.iX);
			if (pucImage[(j + iStartY) * iStride + i + iStartX] == 0)
			{
				iNumErrors++;
				if (iNumErrors > 3)
					return(0);
			}
		}
	}
	else
	{
		for (i = start.iY; i < end.iY; i++)
		{
			j = start.iX + (end.iX - start.iX) * (i - start.iY) / (end.iY - start.iY);
if (pucImage[(i + iStartY) * iStride + j + iStartX] == 0)
{
	iNumErrors++;
	if (iNumErrors > 3)
		return(0);
}
		}
	}
}
void ImageCalPerspectiveTransformSquareToQuadrilateral(Point *pPoints, Transform *pTransform)
{
	float dx3 = pPoints[0].iX - pPoints[1].iX + pPoints[2].iX - pPoints[3].iX;// x0 - x1 + x2 - x3;
	float dy3 = pPoints[0].iY - pPoints[1].iY + pPoints[2].iY - pPoints[3].iY;// y0 - y1 + y2 - y3;
	float dx1 = pPoints[1].iX - pPoints[2].iX;//  x1 - x2;
	float dx2 = pPoints[3].iX - pPoints[2].iX;//  x3 - x2;
	float dy1 = pPoints[1].iY - pPoints[2].iY;//  y1 - y2;
	float dy2 = pPoints[3].iY - pPoints[2].iY;//  y3 - y2;
	float denominator = dx1 * dy2 - dx2 * dy1;
	float a13 = (dx3 * dy2 - dx2 * dy3) / denominator;
	float a23 = (dx1 * dy3 - dx3 * dy1) / denominator;
	float pfCoefs[3][3];

	// (x1 - x0 + a13 * x1, x3 - x0 + a23 * x3, x0, y1 - y0 + a13 * y1, y3 - y0 + a23 * y3, y0, a13, a23, 1.0f));
	pTransform->pfCoefs[0][0] = pPoints[1].iX - pPoints[0].iX + a13 * pPoints[1].iX;	// x1 - x0 + a13 * x1;
	pTransform->pfCoefs[0][1] = pPoints[3].iX - pPoints[0].iX + a23 * pPoints[3].iX;	// x3 - x0 + a23 * x3;
	pTransform->pfCoefs[0][2] = pPoints[0].iX;											// x0;
	pTransform->pfCoefs[1][0] = pPoints[1].iY - pPoints[0].iY + a13 * pPoints[1].iY;	// y1 - y0 + a13 * y1;
	pTransform->pfCoefs[1][1] = pPoints[3].iY - pPoints[0].iY + a23 * pPoints[3].iY;	// y3 - y0 + a23 * y3;
	pTransform->pfCoefs[1][2] = pPoints[0].iY;											// y0;
	pTransform->pfCoefs[2][0] = a13;													// a13;
	pTransform->pfCoefs[2][1] = a23;													// a23;
	pTransform->pfCoefs[2][2] = 1.0;													// 1.0;

}
void ImageTransform(Transform *pTransform, MyImage &dstImage, MyImage &srcImage)
{
	int i, j;
	ImageConfig config;
	float fDstX, fDstY;
	int iOrigX, iOrigY;

	switch (pTransform->iMode)
	{
	case IMAGE_TRANSFORM_PERSPECTIVE:
	{
		float a11, a12, a13, a21, a22, a23, a31, a32, a33;
		float fDenominator;
		a11 = pTransform->pfCoefs[0][0];
		a12 = pTransform->pfCoefs[1][0];
		a13 = pTransform->pfCoefs[2][0];
		a21 = pTransform->pfCoefs[0][1];
		a22 = pTransform->pfCoefs[1][1];
		a23 = pTransform->pfCoefs[2][1];
		a31 = pTransform->pfCoefs[0][2];
		a32 = pTransform->pfCoefs[1][2];
		a33 = pTransform->pfCoefs[2][2];

		switch (srcImage.format)
		{
		case YUV:
			for (i = 0; i < dstImage.height; i++)
			{
				for (j = 0; j < dstImage.width; j++)
				{
					fDstX = (float)j / dstImage.width;
					fDstY = (float)i / dstImage.height;
					fDenominator = a13 * fDstX + a23 * fDstY + a33;
					iOrigX = (a11 * fDstX + a21 * fDstY + a31) / fDenominator;
					iOrigY = (a12 * fDstX + a22 * fDstY + a32) / fDenominator;
					if (iOrigX >= 0 && iOrigX < srcImage.width && iOrigY >= 0 && iOrigY < srcImage.height)
					{
						dstImage.pucData[0][((i + dstImage.offsetY) * dstImage.xStride) + j + dstImage.offsetX] =
							srcImage.pucData[0][((iOrigY + srcImage.offsetY)*srcImage.xStride) + iOrigX + srcImage.offsetX];
						dstImage.pucData[1][((i + dstImage.offsetY) * dstImage.xStride) + j + dstImage.offsetX] =
							srcImage.pucData[1][((iOrigY + srcImage.offsetY)*srcImage.xStride) + iOrigX + srcImage.offsetX];
						dstImage.pucData[2][((i + dstImage.offsetY) * dstImage.xStride) + j + dstImage.offsetX] =
							srcImage.pucData[2][((iOrigY + srcImage.offsetY)*srcImage.xStride) + iOrigX + srcImage.offsetX];
					}
				}
			}
			break;
		}
	}
	break;
	}

}

void ImageRegionGrowing(Point seed, int width, int height, int xStride, int offsetX, int offsetY, unsigned char* pucSrc, unsigned char* pucDst)
{
	int iCurPos;
	int i, j, iPos;
	int iLeft = seed.iX; // inclusive
	int iRight = seed.iX;


	Point *pboundaries;
	int *piStart, *piEnd;
	int iNumBoundaries = 0;
	int iBoundaryStart = -1;
	int iBoundaryEnd = -1;
	int iBoundaryArraySize = 64 * width * height;
	pboundaries = (Point *)calloc(iBoundaryArraySize, sizeof(Point));
	piStart = (int *)calloc(width, sizeof(int));
	piEnd = (int *)calloc(width, sizeof(int));
	int iCurrentX, iCurrentY;
	for (i = 0; i < iBoundaryArraySize; i++)
	{
		pboundaries[i].iX = -1;
		pboundaries[i].iY = -1;
	}

#ifdef MY_PROFILE_TIMING
	LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
	LARGE_INTEGER Frequency;
#endif

	if (Pixel(seed.iX, seed.iY, xStride, offsetX, offsetY, pucSrc) != 0)
		return;

#ifdef MY_PROFILE_TIMING
	QueryPerformanceCounter(&StartingTime);
#endif

	//search left
	do
	{
		Pixel(iLeft, seed.iY, xStride, offsetX, offsetY, pucDst) = 255;
		
		if (Pixel(iLeft - 1, seed.iY, xStride, offsetX, offsetY, pucSrc) == 0)
		{
			iLeft--;
		}
		else {
			break;
		}
	} while (iLeft > 0);

	//search right 
	do
	{
		Pixel(iRight, seed.iY, xStride, offsetX, offsetY, pucDst) = 255;
		
		if (Pixel(iRight + 1, seed.iY, xStride, offsetX, offsetY, pucSrc) == 0)
		{
			iRight++;
		}
		else
			break;
	} while (iRight < width - 1);
	
	//scan from left to right
	for (int i = iLeft ; i <= iRight; i++) {
		//search down
		for (int  j = seed.iY; j < height; j++) {
			if (Pixel(i, j, xStride, offsetX, offsetY, pucSrc) == 0)
			{
				Pixel(i, j, xStride, offsetX, offsetY, pucDst) = 255;
				piEnd[i - iLeft] = j;
			}
			else
				break;
		}
		//search up
		for (int j = seed.iY; j >= 0; j--) {
			if (Pixel(i, j, xStride, offsetX, offsetY, pucSrc) == 0)
			{
				Pixel(i, j, xStride, offsetX, offsetY, pucDst) = 255;
				piStart[i - iLeft] = j;
			}
			else
				break;
		}
	}
	for (int i = iLeft; i <= iRight; i++)
	{
		if (i == iLeft || i == iRight)
		{
			for (j = piStart[i - iLeft]; j <= piEnd[i - iLeft]; j++)
			{
				pboundaries[iNumBoundaries].iX = i;
				pboundaries[iNumBoundaries].iY = j;
				iNumBoundaries++;
			}
		}
		else
		{
			iPos = max(piStart[i - iLeft - 1], piStart[i - iLeft + 1]);
			if (iPos > piStart[i - iLeft])
			{
				for (j = piStart[i - iLeft]; j < iPos; j++)
				{
					pboundaries[iNumBoundaries].iX = i;
					pboundaries[iNumBoundaries].iY = j;
					iNumBoundaries++;
				}
			}
			else
			{
				pboundaries[iNumBoundaries].iX = i;
				pboundaries[iNumBoundaries].iY = piStart[i - iLeft];
				iNumBoundaries++;
			}
			iPos = min(piEnd[i - iLeft - 1], piEnd[i - iLeft + 1]);
			if (iPos < piEnd[i - iLeft])
			{
				for (j = iPos; j <= piEnd[i - iLeft]; j++)
				{
					pboundaries[iNumBoundaries].iX = i;
					pboundaries[iNumBoundaries].iY = j;
					iNumBoundaries++;
				}
			}
			else
			{
				pboundaries[iNumBoundaries].iX = i;
				pboundaries[iNumBoundaries].iY = piEnd[i - iLeft];
				iNumBoundaries++;
			}
		}
	}
	iBoundaryStart = 0;
	iBoundaryEnd = iNumBoundaries;
	while (true)
	{
		i = iBoundaryStart;
		iCurrentX = pboundaries[i].iX - 1;
		iCurrentY = pboundaries[i].iY;
		if (iCurrentX >= 0 && pucDst[iCurrentY * xStride + iCurrentX] == 0 && pucSrc[iCurrentY * xStride + iCurrentX] == 0)
		{
			pucDst[iCurrentY * xStride + iCurrentX] = 255;
			pboundaries[iNumBoundaries].iX = iCurrentX;
			pboundaries[iNumBoundaries].iY = iCurrentY;
			iNumBoundaries++;
			iNumBoundaries %= iBoundaryArraySize;
		}
		iCurrentX = pboundaries[i].iX + 1;
		iCurrentY = pboundaries[i].iY;
		if (iCurrentX < width && pucDst[iCurrentY * xStride + iCurrentX] == 0 && pucSrc[iCurrentY * xStride + iCurrentX] == 0)
		{
			pucDst[iCurrentY * xStride + iCurrentX] = 255;
			pboundaries[iNumBoundaries].iX = iCurrentX;
			pboundaries[iNumBoundaries].iY = iCurrentY;
			iNumBoundaries++;
			iNumBoundaries %= iBoundaryArraySize;
		}
		iCurrentX = pboundaries[i].iX;
		iCurrentY = pboundaries[i].iY - 1;
		if (iCurrentY >= 0 && pucDst[iCurrentY * xStride + iCurrentX] == 0 && pucSrc[iCurrentY * xStride + iCurrentX] == 0)
		{
			pucDst[iCurrentY * xStride + iCurrentX] = 255;
			pboundaries[iNumBoundaries].iX = iCurrentX;
			pboundaries[iNumBoundaries].iY = iCurrentY;
			iNumBoundaries++;
			iNumBoundaries %= iBoundaryArraySize;
		}
		iCurrentX = pboundaries[i].iX;
		iCurrentY = pboundaries[i].iY + 1;
		if (iCurrentY < height && pucDst[iCurrentY * xStride + iCurrentX] == 0 && pucSrc[iCurrentY * xStride + iCurrentX] == 0)
		{
			pucDst[iCurrentY * xStride + iCurrentX] = 255;
			pboundaries[iNumBoundaries].iX = iCurrentX;
			pboundaries[iNumBoundaries].iY = iCurrentY;
			iNumBoundaries++;
			iNumBoundaries %= iBoundaryArraySize;
		}
		iBoundaryStart++;
		iBoundaryStart %= iBoundaryArraySize;
		if (iBoundaryStart == iNumBoundaries)
		{
			break;
		}
	}
#ifdef MY_PROFILE_TIMING
	QueryPerformanceCounter(&EndingTime);
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
	//ElapsedMicroseconds.QuadPart *= 1000000;
	//ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;
#endif
	
}
