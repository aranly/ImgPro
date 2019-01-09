#include "Histogram.h"

extern void HistogramInit(int buckets, Histogram *pHistogram)
{
	pHistogram->iNumBins = buckets;
	for (int i = 0; i < HISTOGRAM_MAX_CATEGORIES; i++)
	{
		pHistogram->piMinOfBins[i] = 0x7fffffff;
		pHistogram->piMaxOfBins[i] = 0;
		pHistogram->piMinValueOfSample[i] = 0x7fffffff;
		pHistogram->piMaxValueOfSample[i] = 0;
		pHistogram->piBins[i] = (int *)calloc(buckets, sizeof(int));
		memset(pHistogram->piBins[i], 0, buckets * sizeof(int));
	}
}


