#include "filter.h"

void computeHistogram(const RGBPixel *image, int size, int *histogram, int channel) {
    for (int i = 0; i < size; i++) {
        if (channel == 0) histogram[image[i].r]++;
        if (channel == 1) histogram[image[i].g]++;
        if (channel == 2) histogram[image[i].b]++;
    }
}

void computeCumulativeHistogram(const int *histogram, int *cumulativeHistogram) {
    cumulativeHistogram[0] = histogram[0];
    for (int i = 1; i < 256; i++) {
        cumulativeHistogram[i] = cumulativeHistogram[i - 1] + histogram[i];
    }
}

void equalizeChannel(RGBPixel *image, int size, const int *cumulativeHistogram, int channel) {
    for (int i = 0; i < size; i++) {
        if (channel == 0) image[i].r = (unsigned char)((cumulativeHistogram[image[i].r] - 1) * 255.0 / (size - 1));
        if (channel == 1) image[i].g = (unsigned char)((cumulativeHistogram[image[i].g] - 1) * 255.0 / (size - 1));
        if (channel == 2) image[i].b = (unsigned char)((cumulativeHistogram[image[i].b] - 1) * 255.0 / (size - 1));
    }
}

void equalizeHistogram(RGBPixel *image, int size) {
    if (image == NULL || size <= 0) {
        fprintf(stderr, "Invalid input for histogram equalization.\n");
        return;
    }

    int histogramR[256] = {0};
    int histogramG[256] = {0};
    int histogramB[256] = {0};
    int cumulativeHistogramR[256] = {0};
    int cumulativeHistogramG[256] = {0};
    int cumulativeHistogramB[256] = {0};

    computeHistogram(image, size, histogramR, 0);
    computeHistogram(image, size, histogramG, 1);
    computeHistogram(image, size, histogramB, 2);

    computeCumulativeHistogram(histogramR, cumulativeHistogramR);
    computeCumulativeHistogram(histogramG, cumulativeHistogramG);
    computeCumulativeHistogram(histogramB, cumulativeHistogramB);

    equalizeChannel(image, size, cumulativeHistogramR, 0);
    equalizeChannel(image, size, cumulativeHistogramG, 1);
    equalizeChannel(image, size, cumulativeHistogramB, 2);
}
