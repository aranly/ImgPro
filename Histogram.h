#pragma once
#include <stdio.h>      
#include <stdlib.h>
#include <string.h>

#define HISTOGRAM_MAX_CATEGORIES	3

typedef struct Histogram
{
	int iNumBins;
	int *piBins[HISTOGRAM_MAX_CATEGORIES];		// piBins[i] stores the number of pixels in bin i
	int iNumTotalSamples;						//total number of categories(R,G,B, or Y,U,V)
	int piMaxOfBins[HISTOGRAM_MAX_CATEGORIES];	//number of pixels in the bin with the most pixels in it
	int piMinOfBins[HISTOGRAM_MAX_CATEGORIES];	//number of pixels in the bin with the least pixels in it
	int piMaxValueOfSample[HISTOGRAM_MAX_CATEGORIES];//index of the bin with the most pixels in it
	int piMinValueOfSample[HISTOGRAM_MAX_CATEGORIES];//index of the bin with the least pixels in it
} Histogram;


extern void HistogramInit(int buckets, Histogram *pHistogram);


