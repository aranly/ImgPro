#pragma once
#include "MyImage.h"
#include "Histogram.h"

typedef struct ImageContext {
	MyImage originalImage;
	MyImage image;
	MyImage display;

	Histogram histogram;

} ImageContext;