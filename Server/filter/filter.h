#include <stdlib.h>
#include <stdio.h>

#ifndef FILTER_H
#define FILTER_H

typedef struct {
    unsigned char r, g, b;
} RGBPixel;


void equalizeHistogram(RGBPixel *image, int size);

#endif /* FILTER_H */