#include "MyImage.h"
#include "Transform.h"

void FlipImageXNonPlanar(int width, int height, unsigned char* pucDst, unsigned char* pucSrc){
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			pucDst[(i*width + j) * 3 ] = pucSrc[3 * (i*width + width - j - 1)];
			pucDst[(i*width + j) * 3 + 1] = pucSrc[3 * (i*width + width - j - 1) + 1];
			pucDst[(i*width + j) * 3 + 2] = pucSrc[3 * (i*width + width - j - 1) + 2];
		}
	}
}

void FlipImageXPlanar(int width, int height, int xStride, unsigned char* pucDst, unsigned char* pucSrc) {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			pucDst[i * xStride + j] = pucSrc[i * xStride + width - j - 1];
		}
	}
}

void FlipImageYNonPlanar(int width, int height, unsigned char* pucDst, unsigned char* pucSrc){
	for (int i = 0; i < width; i++){
		for (int j = 0; j < height; j++){
			pucDst[3 * (i + j * width)] = pucSrc[3 * (i + width*(height - j - 1))];
			pucDst[3 * (i + j * width) + 1] = pucSrc[3 * (i + width*(height - j - 1)) + 1];
			pucDst[3 * (i + j * width) + 2] = pucSrc[3 * (i + width*(height - j - 1)) + 2];
		}
	}

}
void FlipImageYPlanar(int width, int height, int xStride, unsigned char* pucDst, unsigned char* pucSrc) {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			pucDst[i + j* xStride] = pucSrc[i + xStride * (height - j - 1)];
		}
	}
}
void ScaleImage(int scale, int dstWidth, int dstHeight, int dstOffsetX, int dstOffsetY,
	int dstXStride, int srcOffsetX, int srcOffsetY, int srcWidth, int srcHeight, int srcXStride, unsigned char* pucDst, unsigned char* pucSrc){
	int sourcex, sourcey;
	for (int i = 0; i < dstHeight; i++) {
		for (int j = 0; j < dstWidth; j++) {
			sourcex = j * IMAGE_SCALE_FACTOR / scale;
			sourcey = i * IMAGE_SCALE_FACTOR / scale;
			pucDst[((i + dstOffsetY) * dstXStride) + (j + dstOffsetX) * 3]
				= pucSrc[(sourcey + srcOffsetY) * srcXStride + (sourcex + srcOffsetX) * 3];
			pucDst[((i + dstOffsetY) * dstXStride) + (j + dstOffsetX) * 3 + 1]
				= pucSrc[(sourcey + srcOffsetY) * srcXStride + (sourcex + srcOffsetX) * 3 + 1];
			pucDst[((i + dstOffsetY) * dstXStride) + (j + dstOffsetX) * 3 + 2]
				= pucSrc[(sourcey + srcOffsetY) * srcXStride + (sourcex + srcOffsetX) * 3 + 2];
		}
	}
}

void RotateImage(int degree, int dstWidth, int dstHeight, int dstOffsetX, int dstOffsetY,
	int dstXStride, int srcOffsetX, int srcOffsetY, int srcWidth, int srcHeight, int srcXStride, unsigned char* pucDst, unsigned char* pucSrc) {
	float degreeRadians = degree * PI / 180;
	int originX = dstWidth / 2;
	int originY = dstHeight / 2;
	int sourceOriginX = srcWidth / 2;
	int sourceOriginY = srcHeight / 2;
	float sineTheta = sin(degreeRadians);
	float cosineTheta = cos(degreeRadians);

	for (int i = 0; i < dstHeight; i++) {
		for (int j = 0; j < dstWidth; j++) {
			int sourceX = (j - originX)*cosineTheta - (i - originY)*sineTheta + sourceOriginX;
			int sourceY = (j - originX)*sineTheta + (i - originY)*cosineTheta + sourceOriginY;
			if (sourceX >= 0 && sourceX < srcWidth && sourceY >= 0 && sourceY < srcHeight) {
				pucDst[((i + dstOffsetY) * dstXStride) + (j + dstOffsetX) * 3]
					= pucSrc[(sourceY + srcOffsetY) * srcXStride + (sourceX + srcOffsetX) * 3];
				pucDst[((i + dstOffsetY) * dstXStride) + (j + dstOffsetX) * 3 + 1]
					= pucSrc[(sourceY + srcOffsetY) * srcXStride + (sourceX + srcOffsetX) * 3 + 1];
				pucDst[((i + dstOffsetY) * dstXStride) + (j + dstOffsetX) * 3 + 2]
					= pucSrc[(sourceY + srcOffsetY) * srcXStride + (sourceX + srcOffsetX) * 3 + 2];
			}
		}
	}
}
