#include <Windows.h>
#include "MyImage.h"
#include "Transform.h"
#include "ColourspaceConversion.h"
#include "Enhance.h"
#include "Utility.h"
#include "QRCode.h"

void MyImage::Init()
{
	format = UNDEFINED_FORMAT;
	subformat = 0;
	width = 0;
	height = 0;
	xStride = 0;
	offsetX = 0;
	offsetY = 0;
	for (int i = 0; i < 3; i++) {
		pucData[i] = NULL;
	}
}

void MyImage::Init(ImageConfig &config)
{
	format = config.format;
	subformat = config.subformat;
	width = config.width;
	height = config.height;
	xStride = config.xStride;
	offsetX = config.offsetX;
	offsetY = config.offsetY;
	switch (format) {
	case RGB:
		if (subformat == SUBFORMAT_PLANAR) {
			for (int i = 0; i < 3; i++) {
				pucData[i] = (unsigned char*)(malloc)(xStride * (height + 2 * offsetY));
				if (config.ifClear == 1)
					memset(pucData[i], 0, xStride * (height + 2 * offsetY));
				else if (config.ifClear == 2)
					memset(pucData[i], 255, xStride * (height + 2 * offsetY));
			}
		}
		else if (subformat == SUBFORMAT_NONPLANAR) {
			xStride = (width + 2 * offsetX) * 3;
			pucData[0] = (unsigned char *)malloc(xStride * (height + 2 * offsetY));
			if (config.ifClear == 1)
				memset(pucData[0], 0, xStride * (height + 2 * offsetY));
			else if (config.ifClear == 2)
				memset(pucData[0], 255, xStride * (height + 2 * offsetY));
		}
		else if (subformat == SUBFORMAT_RGBA) {
			xStride = (width + 2 * offsetX) * 4;
			pucData[0] = (unsigned char *)malloc(xStride * (height + 2 * offsetY));
			if (config.ifClear == 1)
				memset(pucData[0], 0, xStride * (height + 2 * offsetY));
			else if (config.ifClear == 2)
			{
				memset(pucData[0], 255, xStride * (height + 2 * offsetY));
			}
		}
		break;
	case YUV:
		for (int i = 0; i < 3; i++) {
			pucData[i] = (unsigned char*)(malloc)(xStride * (height + 2 * offsetY));
			if (config.ifClear == 1)
			{
				if (i == 0)
					memset(pucData[i], 0, xStride * (height + 2 * offsetY));
				else
					memset(pucData[i], 128, xStride * (height + 2 * offsetY));
			}
			else if (config.ifClear == 2)
			{
				if (i == 0)
					memset(pucData[i], 255, xStride * (height + 2 * offsetY));
				else
					memset(pucData[i], 128, xStride * (height + 2 * offsetY));
			}
		}
		break;
	}
}

bool MyImage::isInited() {
	if (format != UNDEFINED_FORMAT) {
		return true;
	}
	return false;
}

bool MyImage::isFormatEqual(MyImage& image)
{
	if (image.format == format &&
		image.subformat == subformat &&
		image.width == width &&
		image.height == height &&
		image.xStride == xStride &&
		image.offsetX == offsetX &&
		image.offsetY == offsetY)
		return true;
	else
		return false;
}

void MyImage::Deinitialize() {
	for (int i = 0; i < 3; i++) {
		if (pucData[i]) {
			free(pucData[i]);
			pucData[i] = NULL;
		}
	}
	Init();
}

void MyImage::imageCopyConfig(ImageConfig &config) {
	config.format = format;
	config.subformat = subformat;
	config.width = width;
	config.height = height;
	config.xStride = xStride;
	config.offsetX = offsetX;
	config.offsetY = offsetY;
	config.ifClear = 0;
}

void MyImage::Copy(MyImage & srcImage)
{
	int i;

 	CloneFormat(srcImage);
	switch (srcImage.format)
	{
	case RGB:
		if (srcImage.subformat == SUBFORMAT_NONPLANAR)
		{
			memcpy(pucData[0], srcImage.pucData[0], height * xStride);
		}
		break;
	case YUV:
		for (i = 0; i < 3; i++)
			memcpy(pucData[i], srcImage.pucData[i], height * xStride);
		break;
	}
}

void MyImage::CalHistogram(Histogram * pHistogram)
{
	int i, j;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			pHistogram->piBins[0][pucData[0][((i + offsetY) * xStride) + j + offsetX] / (256 / pHistogram->iNumBins)]++;
			pHistogram->piBins[1][pucData[1][((i + offsetY) * xStride) + j + offsetX] / (256 / pHistogram->iNumBins)]++;
			pHistogram->piBins[2][pucData[2][((i + offsetY) * xStride) + j + offsetX] / (256 / pHistogram->iNumBins)]++;

		}
	}
	for (int i = 0; i < pHistogram->iNumBins; i++)
	{
		for (int j = 0; j < HISTOGRAM_MAX_CATEGORIES; j++)
		{
			int iBinValue = pHistogram->piBins[j][i];
			if (iBinValue < pHistogram->piMinOfBins[j])
				pHistogram->piMinOfBins[j] = iBinValue;

			if (iBinValue > pHistogram->piMaxOfBins[j])
				pHistogram->piMaxOfBins[j] = iBinValue;

			if (iBinValue > 0 && i > pHistogram->piMaxValueOfSample[j])
				pHistogram->piMaxValueOfSample[j] = i;

			if (iBinValue > 0 && i < pHistogram->piMinValueOfSample[j])
				pHistogram->piMinValueOfSample[j] = i;
		}
	}
}

MyImage::MyImage()
{
	Init();
}

MyImage::MyImage(MyImage& image)
{
	Copy(image);
}


MyImage::~MyImage()
{
	Deinitialize();
}

void MyImage::CloneFormat(MyImage& image)
{
	if (isInited())
	{
		if (isFormatEqual(image))
			return;
		else
			Deinitialize();
	}
	ImageConfig config;
	image.imageCopyConfig(config);
	Init(config);
}

void MyImage::LoadImageFromFile(char* filename)
{
	FILE *ifp;
	BITMAPFILEHEADER fileheader;
	BITMAPINFOHEADER infoheader;
	ImageConfig config;
	unsigned char *pucLine;
	int i;

	if (isInited())
	{
		Deinitialize();
	}

	ifp = fopen(filename, "rb");
	fread(&fileheader, sizeof(BITMAPFILEHEADER), 1, ifp);
	fread(&infoheader, sizeof(BITMAPINFOHEADER), 1, ifp);

	config.format = RGB;
	config.subformat = SUBFORMAT_NONPLANAR;
	config.width = infoheader.biWidth;
	config.height = infoheader.biHeight;
	config.offsetX = 0;
	config.offsetY = 0;
	config.xStride = 4 * ((width * 3 + 3) / 4);
	config.ifClear = 0;
	Init(config);

	fseek(ifp, fileheader.bfOffBits, SEEK_SET);
	fread(pucData[0], 1, infoheader.biSizeImage, ifp);
	// flip vertically
	pucLine = (unsigned char *)malloc(xStride);
	for (i = 0; i < height / 2; i++)
	{
		memcpy(pucLine, pucData[0] + i * xStride, xStride);
		memcpy(pucData[0] + i * xStride, pucData[0] + (height - i - 1) * xStride, xStride);
		memcpy(pucData[0] + (height - i - 1) * xStride, pucLine, xStride);
	}
	free(pucLine);
	fclose(ifp);
}

//todo
void MyImage::StoreImageToFile(char* filename){
	FILE *ofp;
	unsigned char *pucLine;
	int i;

	if (format == RGB){
		BITMAPFILEHEADER fileheader;
		BITMAPINFOHEADER infoheader;
		fileheader.bfType = 'BM';
		fileheader.bfSize = 54 + xStride * height;
		fileheader.bfOffBits = 54;
		infoheader.biSize = 40;
		infoheader.biWidth = width;
		infoheader.biHeight = height;
		infoheader.biSizeImage = xStride * height;
		infoheader.biPlanes = 1;
		infoheader.biBitCount = 24;
		infoheader.biXPelsPerMeter = width;
		infoheader.biYPelsPerMeter = height;
		ofp = fopen(filename, "wb");
		fwrite(&fileheader, sizeof(BITMAPFILEHEADER), 1, ofp);
		fwrite(&infoheader, sizeof(BITMAPINFOHEADER), 1, ofp);

		if (subformat == SUBFORMAT_NONPLANAR){
			// flip vertically
			for (i = 0; i < height; i++)
				fwrite(pucData[0] + (height - i - 1) * xStride, xStride, 1, ofp);
		}
		else{

		}
		fclose(ofp);
	}
	if (format == YUV){
		ofp = fopen(filename, "wb");
		fwrite(pucData[0], width*height, 1, ofp);
		fwrite(pucData[1], width*height, 1, ofp);
		fwrite(pucData[2], width*height, 1, ofp);
		fclose(ofp);
	}
}

void MyImage::FlipX(){
	switch (format)
	{
		case RGB:
			{
				unsigned char* pcDestImageData = (unsigned char *)malloc(width * height * 3);
				FlipImageXNonPlanar(width, height, pcDestImageData, pucData[0]);
				free(pucData[0]);
				pucData[0] = pcDestImageData;
			}
			break;
		case YUV:
			for (int i = 0; i < 3; i++)
			{
				unsigned char* pcDestImageData = (unsigned char *)malloc(xStride * (height + 2 * offsetY));
				FlipImageXPlanar(width, height, xStride, pcDestImageData + offsetY * xStride + offsetX,
					pucData[i] + offsetY * xStride + offsetX);
				free(pucData[i]);
				pucData[i] = pcDestImageData;
			}
			break;
	}
}

void MyImage::FlipX(MyImage &srcImage) {
	CloneFormat(srcImage);
	switch (format)
	{
	case RGB:
	{
		FlipImageXNonPlanar(width, height, pucData[0], srcImage.pucData[0]);
	}
	break;
	case YUV:
		for (int i = 0; i < 3; i++)
		{
			FlipImageXPlanar(width, height, xStride, pucData[i] + offsetY * xStride + offsetX,
				srcImage.pucData[i] + offsetY * xStride + offsetX);
		}
		break;
	}
}

void MyImage::FlipY() {
	switch (format)
	{
		case RGB:
		{
			unsigned char* pcDestImageData = (unsigned char *)malloc(width * height * 3);
			FlipImageYNonPlanar(width, height, pcDestImageData, pucData[0]);
			free(pucData[0]);
			pucData[0] = pcDestImageData;
		}
		break;
		case YUV:
			for (int i = 0; i < 3; i++)
			{
				unsigned char* pcDestImageData = (unsigned char *)malloc(xStride * (height + 2 * offsetY));
				FlipImageYPlanar(width, height, xStride, pcDestImageData + offsetY * xStride + offsetX,
					pucData[i] + offsetY * xStride + offsetX);
				free(pucData[i]);
				pucData[i] = pcDestImageData;
			}
			break;
	}
}

void MyImage::FlipY(MyImage &srcImage) {
	CloneFormat(srcImage);
	switch (format)
	{
	case RGB:
	{
		FlipImageYNonPlanar(width, height, pucData[0], srcImage.pucData[0]);
	}
	break;
	case YUV:
		for (int i = 0; i < 3; i++)
		{
			FlipImageYPlanar(width, height, xStride, pucData[i] + offsetY * xStride + offsetX,
				srcImage.pucData[i] + offsetY * xStride + offsetX);
		}
		break;
	}
}

void MyImage::Scale(int scale) {
	int oldwidth = width;
	int oldheight = height;
	int oldxStride = xStride;
	unsigned char* pcNewImageData[3];

	width = width * (float)scale / IMAGE_SCALE_FACTOR;
	height = height * (float)scale / IMAGE_SCALE_FACTOR;

	switch (format) {
	case RGB:
		if (subformat == SUBFORMAT_NONPLANAR) {
			xStride = (width + offsetX * 2) * 3;
			pcNewImageData[0] = (unsigned char *)malloc(xStride* (height + offsetY * 2));
			ScaleImage(scale, width, height, offsetX, offsetY, xStride, offsetX, offsetY, oldwidth, oldheight, oldxStride, pcNewImageData[0], pucData[0]);
		}
		break;
	case YUV:
		xStride = (width + offsetX * 2) * 3;
		pcNewImageData[0] = (unsigned char *)malloc(xStride* (height + offsetY * 2));
		ScaleImage(scale, width, height, offsetX, offsetY, xStride, offsetX, offsetY, oldwidth, oldheight, oldxStride, pcNewImageData[0], pucData[0]);
		break;
	}
	free(pucData[0]);
	pucData[0] = pcNewImageData[0];
}

void MyImage::Scale(int scale, MyImage &srcImage) {
	ImageConfig config;
	srcImage.imageCopyConfig(config);
	config.width = srcImage.width * (float)scale / IMAGE_SCALE_FACTOR;
	config.height = srcImage.height * (float)scale / IMAGE_SCALE_FACTOR;
	config.xStride = (srcImage.width + srcImage.offsetX * 2) * 3;
	Init(config);

	switch (format)
	{
	case RGB:
		if (subformat == SUBFORMAT_NONPLANAR) {
			ScaleImage(scale, width, height, offsetX, offsetY, xStride, offsetX, offsetY, srcImage.width, srcImage.height, srcImage.xStride,
				pucData[0], srcImage.pucData[0]);
		}
		break;
	case YUV:
		ScaleImage(scale, width, height, offsetX, offsetY, xStride, offsetX, offsetY, srcImage.width, srcImage.height, srcImage.xStride,
			pucData[0], srcImage.pucData[0]);
		break;
	}
}

void MyImage::Rotate(int degree) {
	int oldwidth = width;
	int oldheight = height;
	int oldxStride = xStride;
	height = sqrt(width * width + height * height);
	width = height;
	unsigned char* pcNewImageData[3];
	switch (format) {
	case RGB:
		if (subformat == SUBFORMAT_NONPLANAR) {
			xStride = (width + offsetX * 2) * 3;
			pcNewImageData[0] = (unsigned char *)malloc(xStride* (height + OFFSET * 2));
			RotateImage(degree, width, height, offsetX, offsetY, xStride, offsetX, offsetY, oldwidth, oldheight, oldxStride, pcNewImageData[0], pucData[0]);
		}
	}
	free(pucData[0]);
	pucData[0] = pcNewImageData[0];
}

void MyImage::Rotate(int degree, MyImage &srcImage) {
	ImageConfig config;
	srcImage.imageCopyConfig(config);
	config.width = config.height = sqrt(srcImage.width * srcImage.width + srcImage.height * srcImage.height);
	config.xStride = (srcImage.width + srcImage.offsetX * 2) * 3;
	Init(config);

	switch (format)
	{
	case RGB:
		if (subformat == SUBFORMAT_NONPLANAR) {
			RotateImage(degree, width, height, offsetX, offsetY, xStride, offsetX, offsetY, srcImage.width, srcImage.height, srcImage.xStride,
				pucData[0], srcImage.pucData[0]);
		}
		break;
	}
}

void MyImage::RGBtoYUV(){
	unsigned char* pucDataNew[3];
	int newOffsetX = OFFSET;
	int newOffsetY = OFFSET;
	int iNewStride = width + 2 * newOffsetX;
	pucDataNew[0] = (unsigned char *)malloc(iNewStride * (height+ newOffsetY * 2));
	pucDataNew[1] = (unsigned char *)malloc(iNewStride * (height+ newOffsetY * 2));
	pucDataNew[2] = (unsigned char *)malloc(iNewStride * (height+ newOffsetY * 2));
	if (subformat == SUBFORMAT_NONPLANAR){
		for (int i = 0; i < height; i++){
			for (int j = 0; j < width; j++){
				RGBvaluetoYUV(pucData[0][(i + offsetY) * xStride + (j + offsetX) * 3 + 2],
					pucData[0][(i + offsetY) * xStride + (j + offsetX) * 3 + 1],
					pucData[0][(i + offsetY) * xStride + (j + offsetX) * 3],
					pucDataNew[0] + ((i + newOffsetY)*iNewStride) + j + newOffsetX,
					pucDataNew[1] + ((i + newOffsetY)*iNewStride) + j + newOffsetX,
					pucDataNew[2] + ((i + newOffsetY)*iNewStride) + j + newOffsetX);
			}
		}
		free(pucData[0]);
	}
	pucData[0] = pucDataNew[0];
	pucData[1] = pucDataNew[1];
	pucData[2] = pucDataNew[2];
	xStride = iNewStride;
	offsetX = newOffsetX;
	offsetY = newOffsetY;
	format = YUV;
	subformat = SUBFORMAT_PLANAR;
}

void MyImage::RGBtoYUV(MyImage &srcImage) {
	if (subformat == SUBFORMAT_NONPLANAR) {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				RGBvaluetoYUV(srcImage.pucData[0][(i + offsetY)*xStride + (j + offsetX) * 3 + 2],
					srcImage.pucData[0][(i + offsetY)*xStride + (j + offsetX) * 3 + 1],
					srcImage.pucData[0][(i + offsetY)*xStride + (j + offsetX) * 3],
					pucData[0] + ((i + offsetY)*xStride) + j + offsetX,
					pucData[1] + ((i + offsetY)*xStride) + j + offsetX,
					pucData[2] + ((i + offsetY)*xStride) + j + offsetX);

			}
		}
	}
}

void MyImage::RGBtoYUV(bool AddSpace) {
	if (AddSpace) {
		unsigned char* pucDataNew[3];
		int iNewStride = (width / 4) * 4 + 2 * OFFSET;
		pucDataNew[0] = (unsigned char *)malloc(iNewStride * (height + OFFSET * 2));
		pucDataNew[1] = (unsigned char *)malloc(iNewStride * (height + OFFSET * 2));
		pucDataNew[2] = (unsigned char *)malloc(iNewStride * (height + OFFSET * 2));
		int newOffsetX = OFFSET;
		int newOffsetY = OFFSET;
		if (subformat == SUBFORMAT_NONPLANAR) {
			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++) {
					RGBvaluetoYUV(pucData[0][(i + offsetY)*xStride + (j + offsetX) * 3 + 2],
						pucData[0][(i + offsetY)*xStride + (j + offsetX) * 3 + 1],
						pucData[0][(i + offsetY)*xStride + (j + offsetX) * 3],
						pucDataNew[0] + ((i + newOffsetY)*iNewStride) + j + newOffsetX,
						pucDataNew[1] + ((i + newOffsetY)*iNewStride) + j + newOffsetX,
						pucDataNew[2] + ((i + newOffsetY)*iNewStride) + j + newOffsetX);

				}
			}
			free(pucData[0]);
		}
		pucData[0] = pucDataNew[0];
		pucData[1] = pucDataNew[1];
		pucData[2] = pucDataNew[2];
		xStride = iNewStride;
		offsetX = newOffsetX;
		offsetY = newOffsetY;
		format = YUV;
		subformat = SUBFORMAT_PLANAR;
	}
	else {
		unsigned char* pucDataNew[3];
		int iNewStride = (width / 4) * 4;
		pucDataNew[0] = (unsigned char *)malloc(iNewStride * (height));
		pucDataNew[1] = (unsigned char *)malloc(iNewStride * (height));
		pucDataNew[2] = (unsigned char *)malloc(iNewStride * (height));
		int newOffsetX = offsetX;
		int newOffsetY = offsetY;
		if (subformat == SUBFORMAT_NONPLANAR) {
			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++) {
					RGBvaluetoYUV(pucData[0][(i + offsetY)*xStride + (j + offsetX) * 3 + 2],
						pucData[0][(i + offsetY)*xStride + (j + offsetX) * 3 + 1],
						pucData[0][(i + offsetY)*xStride + (j + offsetX) * 3],
						pucDataNew[0] + ((i + newOffsetY)*iNewStride) + j + newOffsetX,
						pucDataNew[1] + ((i + newOffsetY)*iNewStride) + j + newOffsetX,
						pucDataNew[2] + ((i + newOffsetY)*iNewStride) + j + newOffsetX);

				}
			}
			free(pucData[0]);
		}
		pucData[0] = pucDataNew[0];
		pucData[1] = pucDataNew[1];
		pucData[2] = pucDataNew[2];
		xStride = iNewStride;
		offsetX = newOffsetX;
		offsetY = newOffsetY;
		format = YUV;
		subformat = SUBFORMAT_PLANAR;
	}
}

void MyImage::RGBtoYUV(bool AddSpace, MyImage &srcImage) {
	ImageConfig config;
	srcImage.imageCopyConfig(config);
	if (AddSpace)
	{
		config.offsetX = OFFSET;
		config.offsetY = OFFSET;
		config.xStride = (srcImage.width / 4) * 4 + 2 * OFFSET;
	}
	else
		config.xStride = (srcImage.width / 4) * 4;
	Init(config);

	if (subformat == SUBFORMAT_NONPLANAR) {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				RGBvaluetoYUV(srcImage.pucData[0][(i + srcImage.offsetY) * srcImage.xStride + (j + srcImage.offsetX) * 3 + 2],
					srcImage.pucData[0][(i + srcImage.offsetY)*srcImage.xStride + (j + srcImage.offsetX) * 3 + 1],
					srcImage.pucData[0][(i + srcImage.offsetY)*srcImage.xStride + (j + srcImage.offsetX) * 3],
					pucData[0] + ((i + offsetY) * xStride) + j + offsetX,
					pucData[1] + ((i + offsetY) * xStride) + j + offsetX,
					pucData[2] + ((i + offsetY) * xStride) + j + offsetX);

			}
		}
	}
}

void MyImage::ConvertToRGBA(MyImage &srcImage)
{	
	int i, j;
	ImageConfig config;
	srcImage.imageCopyConfig(config);
	config.format = RGB;
	config.subformat = SUBFORMAT_RGBA;
	config.xStride = srcImage.width * 4;
	Init(config);
	switch (srcImage.format)
	{
	case RGB:
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				pucData[0][(i * xStride) + j * 4] = srcImage.pucData[0][i * srcImage.xStride + j * 3];
				pucData[0][(i * xStride) + j * 4 + 1] = srcImage.pucData[0][i * srcImage.xStride + j * 3 + 1];
				pucData[0][(i * xStride) + j * 4 + 2] = srcImage.pucData[0][i * srcImage.xStride + j * 3 + 2];
				pucData[0][(i * xStride) + j * 4 + 3] = 0;
			}
		}
		break;
	case YUV:
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				YUVvaluetoRGB(pucData[0] + (4 * (i * width + j) + 2),
					pucData[0] + (4 * (i * width + j) + 1),
					pucData[0] + (4 * (i * width + j)),
					srcImage.pucData[0] [((i + srcImage.offsetY) * srcImage.xStride) + j + srcImage.offsetX],
					srcImage.pucData[1] [((i + srcImage.offsetY) * srcImage.xStride) + j + srcImage.offsetX],
					srcImage.pucData[2] [((i + srcImage.offsetY) * srcImage.xStride) + j + srcImage.offsetX]);
			}
		}
		break;
	}
}

void MyImage::GenBlockImage(unsigned char * pucBlockData, int iDataWidth, int iDataHeight, int iBlockWidth, int iBlockHeight)
{
	ImageConfig config;
	int i, j, k, l;
	unsigned char *pucBlockStart;
	if (isInited())
		Deinitialize();

	config.format = YUV;
	config.subformat = 1;
	config.offsetX = config.offsetY = 0;
	config.width = iDataWidth * iBlockWidth;
	config.height = iDataHeight * iBlockHeight;
	config.xStride = config.width + offsetX * 2;
	config.ifClear = 1;
	Init(config);

	for (i = 0; i < iDataHeight; i++)
	{
		for (j = 0; j < iDataWidth; j++)
		{
			pucBlockStart = pucData[0] + (offsetY + i * iBlockHeight) * xStride + j * iBlockWidth + offsetX;
			for (k = 0; k < iBlockHeight; k++)
			{
				for (l = 0; l < iBlockWidth; l++)
				{
					pucBlockStart[k * xStride + l] = pucBlockData[i * iDataWidth + j];
				}
			}
		}
	}
}

void MyImage::AdjustBrightness(float brightness){
	switch (format)
	{
	case RGB:
		AdjustImageBrightness(brightness, width*3, height, offsetX*3, offsetY, xStride, pucData[0], pucData[0]);
		break;
	case YUV:
		AdjustImageBrightness(brightness, width, height, offsetX, offsetY, xStride, pucData[0], pucData[0]);
		break;
	}
}

void MyImage::AdjustBrightness(float brightness, MyImage &srcImage) {
	CloneFormat(srcImage);
	switch (format)
	{
	case RGB:
		AdjustImageBrightness(brightness, width * 3, height, offsetX * 3, offsetY, xStride, pucData[0], srcImage.pucData[0]);
		break;
	case YUV:
		AdjustImageBrightness(brightness, width, height, offsetX, offsetY, xStride, pucData[0], srcImage.pucData[0]);
		break;
	}
}

void MyImage::AdjustContrast(float contrast){
	switch (format)
	{
	case RGB:
		RGBtoYUV();
		AdjustImageContrast(contrast, width, height, offsetX, offsetY, xStride/3, pucData[0], pucData[0]);
		break;
	case YUV:
		AdjustImageContrast(contrast, width, height, offsetX, offsetY, xStride, pucData[0], pucData[0]);
		break;
	}
}

void MyImage::AdjustContrast(float contrast, MyImage &srcImage) {
	CloneFormat(srcImage);
	switch (format)
	{
	case RGB:
		AdjustImageContrast(contrast, width, height, offsetX, offsetY, xStride / 3, pucData[0], srcImage.pucData[0]);
		break;
	case YUV:
		AdjustImageContrast(contrast, width, height, offsetX, offsetY, xStride, pucData[0], srcImage.pucData[0]);
		break;
	}
}

void MyImage::MakeGreyscale(){
	switch (format)
	{
	case RGB:
		GreyscaleRGB(width, height, pucData[0]);
		break;
	case YUV:
		Greyscale4UV(width, height, xStride, offsetX, offsetY, pucData[1]);
		Greyscale4UV(width, height, xStride, offsetX, offsetY, pucData[2]);
	break;
	}
}

void MyImage::MakeGreyscale(MyImage &srcImage) {
	CloneFormat(srcImage);
	switch (format)
	{
	case RGB:
		GreyscaleRGB(width, height, pucData[0]);
		break;
	case YUV:
		Greyscale4Y(width, height, xStride, offsetX, offsetY, pucData[0], srcImage.pucData[0]);
		Greyscale4UV(width, height, xStride, offsetX, offsetY, pucData[1]);
		Greyscale4UV(width, height, xStride, offsetX, offsetY, pucData[2]);
		break;
	}
}

void MyImage::Sharpen() {
	unsigned char* pcNew = (unsigned char *)malloc(xStride * (height+ OFFSET*2));
	switch (format)
	{
	case RGB:
		RGBtoYUV();
		SharpenImage(width, height, offsetX, offsetY, xStride, pucData[0], pcNew);
		free(pucData[0]);
		pucData[0] = pcNew;
		break;
	case YUV:
		SharpenImage(width, height, offsetX, offsetY, xStride, pucData[0], pcNew);
		free(pucData[0]);
		pucData[0] = pcNew;
		break;
	}
}

void MyImage::Sharpen(MyImage &srcImage) {
	CloneFormat(srcImage);
	switch (format)
	{
	case RGB:
		SharpenImage(width, height, offsetX, offsetY, xStride, pucData[0], srcImage.pucData[0]);
		break;
	case YUV:
		SharpenImage(width, height, offsetX, offsetY, xStride, pucData[0], srcImage.pucData[0]);
		break;
	}
}

void MyImage::Blur() {
	unsigned char* pcNew = (unsigned char *)malloc(xStride * (height+ OFFSET*2));
	switch (format)
	{
	case RGB:
		
		RGBtoYUV();
		BlurImage(width, height, offsetX, offsetY, xStride, pucData[0], pcNew);
		free(pucData[0]);
		pucData[0] = pcNew;
		break;
	case YUV:
		BlurImage(width, height, offsetX, offsetY, xStride, pucData[0], pcNew);
		free(pucData[0]);
		pucData[0] = pcNew;
		break;
	}
}

void MyImage::Blur(MyImage &srcImage) {
	CloneFormat(srcImage);
	switch (format)
	{
	case RGB:
		BlurImage(width, height, offsetX, offsetY, xStride, pucData[0], srcImage.pucData[0]);
		break;
	case YUV:
		BlurImage(width, height, offsetX, offsetY, xStride, pucData[0], srcImage.pucData[0]);
		break;
	}
}

void MyImage::Kernel() {
	unsigned char* pcNew = (unsigned char *)malloc(xStride * (height+ OFFSET*2));
	switch (format)
	{
	case RGB:
		
		RGBtoYUV();
		KernelImage(width, height, offsetX, offsetY, xStride, pucData[0], pcNew);
		free(pucData[0]);
		pucData[0] = pcNew;
		break;
	case YUV:
		KernelImage(width, height, offsetX, offsetY, xStride, pucData[0], pcNew);
		free(pucData[0]);
		pucData[0] = pcNew;
		break;
	}
}

void MyImage::Kernel(MyImage &srcImage) {
	CloneFormat(srcImage);
	switch (format)
	{
	case RGB:
		KernelImage(width, height, offsetX, offsetY, xStride, pucData[0], srcImage.pucData[0]);
		break;
	case YUV:
		KernelImage(width, height, offsetX, offsetY, xStride, pucData[0], srcImage.pucData[0]);
		break;
	}
}



void MyImage::Canny()
{
	unsigned char* pcNew = (unsigned char *)malloc(xStride * (height + OFFSET * 2));
	int threshold1 = 128;
	int threshold2 = 200;
	//unsigned char *origpos;
	//unsigned char *dstpos = ImageStart(pcNew, offsetX, offsetY, xStride);
	switch (format)
	{
	case RGB:
		RGBtoYUV();
		//origpos = ImageStart(pucData[0], offsetX, offsetY, xStride);
		Greyscale4UV(width, height, xStride, offsetX, offsetY, pucData[1]);
		Greyscale4UV(width, height, xStride, offsetX, offsetY, pucData[2]);
		CannyImage(width, height, offsetX, offsetY, xStride, threshold1, threshold2, pcNew, pucData[0]);
		free(pucData[0]);
		pucData[0] = pcNew;
		break;
	case YUV:
		Greyscale4UV(width, height, xStride, offsetX, offsetY, pucData[1]);
		Greyscale4UV(width, height, xStride, offsetX, offsetY, pucData[2]);
		CannyImage(width, height, offsetX, offsetY, xStride, threshold1, threshold2, pcNew, pucData[0]);
		free(pucData[0]);
		pucData[0] = pcNew;
		break;
	}
}

void MyImage::SobelVisualization() {
	int* piGradientMagnitude = (int *)calloc(width * height, sizeof(int));
	float* pfGradientAngle = (float *)calloc(width * height, sizeof(float));
	int iNewStride = width;
	int iNewOffsetX = 0;
	int iNewOffsetY = 0;
	unsigned char* pcNew = (unsigned char *)malloc(iNewStride * (height + iNewOffsetY * 2));
	switch (format)
	{
	case RGB:
		RGBtoYUV();
		Greyscale4UV(width, height, xStride, offsetX, offsetY, pucData[1]);
		Greyscale4UV(width, height, xStride, offsetX, offsetY, pucData[2]);
		SobelImage(width, height, offsetX, offsetY, xStride, piGradientMagnitude, pfGradientAngle, pucData[0]);
		NormalizeMinMax(width, height, 0, 0, width, piGradientMagnitude,pcNew);
		free(pucData[0]);
		pucData[0] = pcNew;
		break;
	case YUV:
		Greyscale4UV(width, height, xStride, offsetX, offsetY, pucData[1]);
		Greyscale4UV(width, height, xStride, offsetX, offsetY, pucData[2]);
		SobelImage(width, height, offsetX, offsetY, xStride, piGradientMagnitude, pfGradientAngle, pucData[0]);
		NormalizeMinMax(width, height, 0, 0, width, piGradientMagnitude, pcNew);
		free(pucData[0]);
		pucData[0] = pcNew;
		break;
	}
	xStride = iNewStride;
	offsetX = iNewOffsetX;
	offsetY = iNewOffsetY;
}

void MyImage::Segmentation()
{
	unsigned char* pucRegions = (unsigned char *)malloc(xStride * (height + OFFSET * 2));
	memset(pucRegions, 0, xStride * (height + OFFSET * 2));
	Point seed = { 493, 200 };
	Canny();
	Dilate(5);
	//Erode(3);
	ImageRegionGrowing(seed, width, height, xStride, offsetX, offsetY, pucData[0], pucRegions);
	free(pucData[0]);
	pucData[0] = pucRegions;
}

// Binarization
void MyImage::CustomBinarization(int threshold) {
	//unsigned char* pcNew = (unsigned char *)malloc(xStride * (height + OFFSET * 2));
	switch (format)
	{
	case RGB:
		GreyscaleRGB(width, height, pucData[0]);
		RGBtoYUV();
	case YUV:
		CustomBinarizeImage(width, height, xStride, offsetX, offsetY, threshold, pucData[0]);
		memset(pucData[1], 128, xStride * height);
		memset(pucData[2], 128, xStride * height);
		break;
	}
}
void MyImage::OtsuBinarization(Histogram histogram) {
	switch (format)
	{
	case RGB:
		GreyscaleRGB(width, height, pucData[0]);
		RGBtoYUV();
	case YUV:
		OtsuBinarizeImage(width, height, xStride, offsetX, offsetY, histogram.piBins[0], pucData[0]);
		memset(pucData[1], 128, xStride * height);
		memset(pucData[2], 128, xStride * height);
		break;
	}
}
void MyImage::SauvolaBinarization()
{
	unsigned char* pcNew = (unsigned char *)malloc(xStride * (height + OFFSET * 2));
	switch (format)
	{
	case RGB:
		GreyscaleRGB(width, height, pucData[0]);
		RGBtoYUV();
	case YUV:
		SauvolaBinarizeImage(width, height, xStride, offsetX, offsetY, 30, pucData[0], pcNew);
		free(pucData[0]);
		pucData[0] = pcNew;
		memset(pucData[1], 128, xStride * height);
		memset(pucData[2], 128, xStride * height);
		break;
	}
}
void MyImage::NiblackBinarization()
{
	unsigned char* pcNew = (unsigned char *)malloc(xStride * (height + OFFSET * 2));
	switch (format)
	{
	case RGB:
		GreyscaleRGB(width, height, pucData[0]);
		RGBtoYUV();
	case YUV:
		NiblackBinarizeImage(width, height, xStride, offsetX, offsetY, 30, pucData[0],pcNew);
		free(pucData[0]);
		pucData[0] = pcNew;
		break;
	}
}
void MyImage::ZxingBinarization() {
	unsigned char* pcNew = (unsigned char *)malloc(xStride * (height + OFFSET * 2));
	switch (format)
	{
	case RGB:
		GreyscaleRGB(width, height, pucData[0]);
		RGBtoYUV();
	case YUV:
		ZxingBinarizeImage(xStride, width, height, pucData[0], pcNew);
		free(pucData[0]);
		pucData[0] = pcNew;
		break;
	}
}
void MyImage::GlobalBinarization(Histogram histogram) {
	switch (format)
	{
	case RGB:
		GreyscaleRGB(width, height, pucData[0]);
		RGBtoYUV();
	case YUV:
		GlobalBinarizeImage(width, height, xStride, offsetX, offsetY, histogram.piBins[0], pucData[0]);
		break;
	}
}

void MyImage::Erode(int dimension) {
	unsigned char* pcNew = (unsigned char *)malloc(xStride * (height + OFFSET * 2));
	switch (format)
	{
	case RGB:
		GreyscaleRGB(width, height, pucData[0]);
		RGBtoYUV();
		ErodeImage(dimension, width, height, offsetX, offsetY, xStride, pucData[0],pcNew);
		free(pucData[0]);
		pucData[0] = pcNew;
		break;
	case YUV:
		ErodeImage(dimension, width, height, offsetX, offsetY, xStride, pucData[0],pcNew);
		free(pucData[0]);
		pucData[0] = pcNew;
		break;
	}
}
void MyImage::Dilate(int dimension) {
	unsigned char* pcNew = (unsigned char *)malloc(xStride * (height + OFFSET * 2));
	switch (format)
	{
	case RGB:
		GreyscaleRGB(width, height, pucData[0]);
		RGBtoYUV(false);
		DilateImage(dimension, width, height, offsetX, offsetY, xStride, pucData[0], pcNew);
		free(pucData[0]);
		pucData[0] = pcNew;
		break;
	case YUV:
		DilateImage(dimension, width, height, offsetX, offsetY, xStride, pucData[0], pcNew);
		free(pucData[0]);
		pucData[0] = pcNew;
		break;
	}
}

