//
// Created by sebikul on 14/06/18.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <ctype.h>
#include "bmp.h"
#include "stegobmp.h"


STEG_METHOD stegobmp_steg_method(const char *steg_method) {
    if (strcmp(steg_method, "LSB1") == 0) {
        return LSB1;
    } else if (strcmp(steg_method, "LSB4") == 0) {
        return LSB4;
    } else if (strcmp(steg_method, "LSBE") == 0) {
        return LSBE;
    }

    return STEG_ERROR;
}

ENC_METHOD stegobmp_enc_method(const char *enc_method) {
    if (strcmp(enc_method, "aes128") == 0) {
        return AES128;
    } else if (strcmp(enc_method, "aes192") == 0) {
        return AES192;
    } else if (strcmp(enc_method, "aes256") == 0) {
        return AES256;
    } else if (strcmp(enc_method, "des") == 0) {
        return DES;
    }

    return ENC_METHOD_ERROR;
}

ENC_MODE stegobmp_enc_mode(const char *enc_mode) {
    if (strcmp(enc_mode, "ecb") == 0) {
        return ECB;
    } else if (strcmp(enc_mode, "cfb") == 0) {
        return CFB;
    } else if (strcmp(enc_mode, "ofb") == 0) {
        return OFB;
    } else if (strcmp(enc_mode, "cbc") == 0) {
        return CBC;
    }

    return ENC_MODE_ERROR;
}

static void hide(uint8_t *dst, const uint8_t *src, long nbytes) {

    static uint8_t mask = (uint8_t) (-1 << 1);

    for (int i = 0, d = 0; i < nbytes; ++i) {

        for (int j = 0; j < 8; ++j) {
            uint8_t new_byte = src[i] & (1 << j);

            new_byte = new_byte >> j;


            dst[d] = dst[d] & (mask | new_byte);
            d++;
        }
    }
}

//https://www.rosettacode.org/wiki/Extract_file_extension#C
static char *file_ext(const char *string) {
    char *ext = strrchr(string, '.');

    if (ext == NULL)
        return (char *) string + strlen(string);

    for (char *iter = ext + 1; *iter != '\0'; iter++) {
        if (!isalnum((unsigned char) *iter))
            return (char *) string + strlen(string);
    }

    return ext;
}


int stegobmp_embed(bmp_image_t *image, const char *input_path, STEG_METHOD steg_method, ENC_METHOD enc_method,
                   ENC_MODE enc_mode, const char *password) {

    uint8_t *image_buffer = bmp_get_data_buffer(image);
    uint8_t *file_buffer;

    FILE *fp = fopen(input_path, "rb");
    if (fp == NULL) {
        printf("Could not open file %s", input_path);
        printf("Error: ! %s\n", strerror(errno));
        return 1;
    }


    fseek(fp, 0, SEEK_END);
    long input_file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if (!bmp_check_size(image, input_file_size)) {
        printf("Carrier of insufficient size!");
        return 2;
    }

    file_buffer = malloc((size_t) input_file_size);
    fread(file_buffer, (size_t) input_file_size, 1, fp);
    fclose(fp);

    uint8_t *data_to_save;
    long size_of_data;

    char *ext = file_ext(input_path);

    if (enc_method != ENC_METHOD_NONE) {

    } else {

        size_of_data = sizeof(uint32_t) + input_file_size + strlen(ext) + 1;

        data_to_save = malloc(size_of_data);
        *(uint32_t *) data_to_save = (uint32_t) input_file_size;

        memcpy(data_to_save + sizeof(uint32_t), file_buffer, input_file_size);

        strcpy((char *) (data_to_save + sizeof(uint32_t) + input_file_size), ext);
//
//        data_to_save = file_buffer;
//        size_of_data = input_file_size;
    }

    hide(image_buffer, data_to_save, size_of_data);

    return 0;
}

