#include "ColourspaceConversion.h"

void RGBvaluetoYUV(unsigned char R, unsigned char G, unsigned char B, unsigned char* pucY, unsigned char* pucU, unsigned char*pucV){
	int temp;
	temp = ((77 * R + 150 * G + 29 * B + 128) >> 8) ;
	*pucY = (unsigned char)clamp255(temp);

	temp = ((-43 * R - 84 * G + 127 * B + 128) >> 8) + 128;
	*pucU = (unsigned char)clamp255(temp);

	temp = ((127 * R - 106 * G - 21 * B + 128) >> 8) + 128;
	*pucV = (unsigned char)clamp255(temp);
}

void YUVvaluetoRGB(unsigned char *R, unsigned char *G, unsigned char *B, unsigned char pucY, unsigned char pucU, unsigned char pucV){
	int temp;
	temp = ((298 * (pucY - 16) + 409 * (pucV - 128) + 128) )>> 8;
	*R = (unsigned char)clamp255(temp);
	
	temp = ((298 * (pucY - 16) - 100 * (pucU - 128) - 208 * (pucV - 128) + 128) )>> 8;
	*G = (unsigned char)clamp255(temp);
	
	temp = ((298 * (pucY - 16) + 516 * (pucU - 128) + 128) )>> 8;
	*B = (unsigned char)clamp255(temp);
}