/* Ciobanu Andrei-Bogdan 312CB */

#ifndef UTILS_H
#define UTILS_H

#include "bmp_header.h"
#include "cJSON.h"

#include <stdio.h>

typedef struct {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
} Color;

typedef struct {
    bmp_fileheader *fileheader;
    bmp_infoheader *infoheader;
    Color *pixel_array;
    unsigned short padding;
} bmp;

cJSON *get_json_from_file(FILE *file);

bmp *get_bmp_from_json(cJSON *json);

void free_bmp(bmp *bitmap);

int write_to_bitmap_file(bmp *bitmap, char *input_path, unsigned int task);

int write_to_json_file(char *input_path, int state);

#endif /* UTILS_H */
