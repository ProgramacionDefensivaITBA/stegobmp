//
// Created by sebikul on 27/05/18.
//

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "bmp.h"

struct __attribute__ ((packed)) bmp_header {
    char fileIdentifier[2];
    uint32_t fileSize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t imageDataOffset;
};

struct __attribute__ ((packed)) bmp_info {
    uint32_t headerSize;
    int32_t imageWidth;
    int32_t imageHeight;
    uint16_t colorPlanes;
    uint16_t bitsPerPixel;
    uint32_t compressionMethod;
    uint32_t imageSize; /*Size of raw bitmap data*/
    int32_t horizontalRes;
    int32_t verticalRes;
    uint32_t colorsInPalette;
    uint32_t numImportantColors;
};

struct bmp_image {
    struct bmp_header header;
    struct bmp_info info;
    uint8_t *extra;
    uint8_t *data;
};

bmp_image_t *bmp_from_path(const char *path) {
    FILE *fp = fopen(path, "rb");

    if (fp == NULL) {
        printf("Could not open file %s", path);
        printf("Error: ! %s\n", strerror(errno));
        return NULL;
    }

    bmp_image_t *image = malloc(sizeof(bmp_image_t));

    fread(&image->header, sizeof(struct bmp_header), 1, fp);
    fread(&image->info, sizeof(struct bmp_info), 1, fp);

    image->data = malloc(image->info.imageSize);

    const size_t extra_size = image->header.imageDataOffset - sizeof(struct bmp_header) - sizeof(struct bmp_info);

    if (extra_size != 54) {
        // Si la imagen contiene datos extra, vamos a asegurarnos de que queden en la imagen destino, pero
        // no garantizamos que el programa funcione correctamente.
        printf("Image contains extra data between header and bitmap!");
    }

    image->extra = malloc(extra_size);

//    fseek(fp, image->header.imageDataOffset, SEEK_SET);

    fread(image->extra, extra_size, 1, fp);
    fread(image->data, image->info.imageSize, 1, fp);

    return image;
}


int bmp_save(const bmp_image_t *image, const char *path) {
    FILE *fp = fopen(path, "wb+");

    if (fp == NULL) {
        printf("Could not open file %s", path);
        printf("Error: ! %s\n", strerror(errno));
        return NULL;
    }

    fwrite(&image->header, sizeof(struct bmp_header), 1, fp);
    fwrite(&image->info, sizeof(struct bmp_info), 1, fp);

    const size_t extra_size = image->header.imageDataOffset - sizeof(struct bmp_header) - sizeof(struct bmp_info);

    fwrite(image->extra, extra_size, 1, fp);

    fwrite(image->data, image->info.imageSize, 1, fp);

    return 1;
}
