#include <string.h>
#include <malloc.h>
#include <math.h>
#include "MyImage.h"
#include "Utility.h"

#define QRCODE_MAX_SEGMENTS	1024
#define QRCODE_MAX_ANCHORS	64
#define QRCODE_MAX_ANCHOR_WIDTH_DIVIATION	0.3

#define QRCODE_NORMALIZED_WIDTH		512
#define QRCODE_NORMALIZED_TOPLEFT	55

class QRCode
{
	int piSeg[IMAGE_MAX_HEIGHT][QRCODE_MAX_SEGMENTS];
	int piNumSegs[IMAGE_MAX_HEIGHT];
	int piAnchorCandidate[IMAGE_MAX_HEIGHT][QRCODE_MAX_ANCHORS];
	int piNumAnchors[IMAGE_MAX_HEIGHT];

public:
	Line pResultLine[4];
	Line pInitiaLine[4];
	Point pPoints[4];
	Transform transform;

public:
	void Init();
	void Process(MyImage& image);
};


