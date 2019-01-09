
extern void FlipImageXNonPlanar(int width, int height, unsigned char* pucDst, unsigned char* pucSrc);
extern void FlipImageXPlanar(int width, int height, int xStride, unsigned char* pucDst, unsigned char* pucSrc);
extern void FlipImageYNonPlanar(int width, int height, unsigned char* pucDst, unsigned char* pucSrc);
extern void FlipImageYPlanar(int width, int height, int xStride, unsigned char* pucDst, unsigned char* pucSrc);

extern void ScaleImage(int scale, int dstWidth, int dstHeight, int dstOffsetX, int dstOffsetY,
	int dstXStride, int srcOffsetX, int srcOffsetY, int srcWidth, int srcHeight, int srcXStride, unsigned char* pucDst, unsigned char* pucSrc);
extern void RotateImage(int degree, int dstWidth, int dstHeight, int dstOffsetX, int dstOffsetY,
	int dstXStride, int srcOffsetX, int srcOffsetY, int srcWidth, int srcHeight, int srcXStride, unsigned char* pucDst, unsigned char* pucSrc);
