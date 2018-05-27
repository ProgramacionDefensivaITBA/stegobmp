//
// Created by sebikul on 27/05/18.
//

#ifndef STEGOBMP_BMP_H
#define STEGOBMP_BMP_H


typedef struct bmp_image bmp_image_t;

bmp_image_t *bmp_from_path(const char *path);

int bmp_save(const bmp_image_t *image, const char *path);


#endif //STEGOBMP_BMP_H
