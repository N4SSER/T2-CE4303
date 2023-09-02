#include "filter.h"

void equalizeHistogram(RGBPixel *image, int size)
{
    if (image == NULL || size <= 0) 
    {
        fprintf(stderr, "Invalid input for histogram equalization.\n");
        return;
    }
    
    int histogramR[256] = {0};
    int histogramG[256] = {0};
    int histogramB[256] = {0};
    int cumulativeHistogramR[256] = {0};
    int cumulativeHistogramG[256] = {0};
    int cumulativeHistogramB[256] = {0};

    for (int i = 0; i < size; i++) 
    {
        histogramR[image[i].r]++;
        histogramG[image[i].g]++;
        histogramB[image[i].b]++;
    }

    cumulativeHistogramR[0] = histogramR[0];
    cumulativeHistogramG[0] = histogramG[0];
    cumulativeHistogramB[0] = histogramB[0];

    for (int i = 1; i < 256; i++) 
    {
        cumulativeHistogramR[i] = cumulativeHistogramR[i - 1] + histogramR[i];
        cumulativeHistogramG[i] = cumulativeHistogramG[i - 1] + histogramG[i];
        cumulativeHistogramB[i] = cumulativeHistogramB[i - 1] + histogramB[i];
    }

    for (int i = 0; i < size; i++) 
    {
        image[i].r = (unsigned char)((cumulativeHistogramR[image[i].r] - 1) * 255.0 / (size - 1));
        image[i].g = (unsigned char)((cumulativeHistogramG[image[i].g] - 1) * 255.0 / (size - 1));
        image[i].b = (unsigned char)((cumulativeHistogramB[image[i].b] - 1) * 255.0 / (size - 1));
    }
}