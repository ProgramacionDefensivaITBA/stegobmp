#include <stdio.h>

#include "bmp.h"

int main() {
    printf("Loading sample BMP!\n");

    bmp_image_t *image = bmp_from_path("/home/sebikul/Proyectos/stegobmp/sample/barbara.bmp");

    bmp_save(image, "/home/sebikul/Proyectos/stegobmp/sample/barbara.save.bmp");

    return 0;
}