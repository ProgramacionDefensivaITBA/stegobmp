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

static void lsb1_hide(uint8_t *dst, const uint8_t *src, long nbytes) {

    static uint8_t mask = (uint8_t) (~0x1);

    for (int i = 0, d = 0; i < nbytes; ++i) {

        for (int j = 7; j >= 0; --j) {
            uint8_t new_bit = src[i] & (0x1 << j);

            new_bit = new_bit >> j;


            dst[d] = (dst[d] & mask) | new_bit;
            d++;
        }
    }
}

static int lsb1_extract(uint8_t *dst, const uint8_t *src, long nbytes, int null_cutoff) {

    int curr_src = 0;
    int curr_dst = 0;

    while (curr_dst < nbytes) {

        for (int j = 0; j < 8; ++j) {
            uint8_t new_bit = src[curr_src + j] & 0x1;

            new_bit = new_bit << (7 - j);

            dst[curr_dst] = dst[curr_dst] | new_bit;
        }

        if (null_cutoff && dst[curr_dst] == 0) {
            return curr_dst;
        }

        curr_src += 8;
        curr_dst++;
    }


    return curr_dst;
}

static void lsb4_hide(uint8_t *dst, const uint8_t *src, long nbytes) {

    static uint8_t mask = (uint8_t) (~0x1);

    for (int i = 0, d = 0; i < nbytes; ++i) {

        for (int j = 1; j >= 0; --j) {
            uint8_t new_bit = src[i] & (0xF << (j * 4));

            new_bit = new_bit >> (j * 4);


            dst[d] = (dst[d] & mask) | new_bit;
            d++;
        }
    }
}

static int lsb4_extract(uint8_t *dst, const uint8_t *src, long nbytes, int null_cutoff) {

    int curr_src = 0;
    int curr_dst = 0;

    while (curr_dst < nbytes) {

        for (int j = 0; j < 2; ++j) {
            uint8_t new_bit = src[curr_src + j] & 0xF;

            new_bit = new_bit << (4 - j * 4);

            dst[curr_dst] = dst[curr_dst] | new_bit;
        }

        if (null_cutoff && dst[curr_dst] == 0) {
            return curr_dst;
        }

        curr_src += 2;
        curr_dst++;
    }


    return curr_dst;
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

        // TODO

    } else {

        size_of_data = sizeof(uint32_t) + input_file_size + strlen(ext) + 1;

        data_to_save = malloc(size_of_data);
//        *(uint32_t *) data_to_save = (uint32_t) input_file_size;

        uint32_t swapped_size = __bswap_32(input_file_size);

        memcpy(data_to_save, &swapped_size, sizeof(uint32_t));
        memcpy(data_to_save + sizeof(uint32_t), file_buffer, input_file_size);

        memcpy(data_to_save + sizeof(uint32_t) + input_file_size, ext, strlen(ext) + 1);

//        *(data_to_save + sizeof(uint32_t) + input_file_size) = '\0';

//        data_to_save = file_buffer;
//        size_of_data = input_file_size;
    }

    switch (steg_method) {
        case LSB1:
            lsb1_hide(image_buffer, data_to_save, size_of_data);
            break;
        case LSB4:
            break;
        case LSBE:
            break;
    }

    return 0;
}

int stegobmp_extract(bmp_image_t *image, const char *output_path, STEG_METHOD steg_method, ENC_METHOD enc_method,
                     ENC_MODE enc_mode, const char *password) {

    uint8_t *image_buffer = bmp_get_data_buffer(image);
    uint32_t hidden_data_size = 0;
    char *extension;


    switch (steg_method) {
        case LSB1:
            lsb1_extract(&hidden_data_size, image_buffer, sizeof(hidden_data_size), 0);
            break;
        case LSB4:
            lsb4_extract(&hidden_data_size, image_buffer, sizeof(hidden_data_size), 0);
            break;
        case LSBE:
            break;
    }
    hidden_data_size = __bswap_32(hidden_data_size);

    if (hidden_data_size < bmp_get_image_size(image) / 8) {
        printf("Embeded file possibly found.\n");
    } else {
        return 1;
    }

    void *raw_data = calloc(hidden_data_size, 1);

    switch (steg_method) {
        case LSB1:
            lsb1_extract(raw_data, image_buffer + sizeof(hidden_data_size) * 8, hidden_data_size, 0);
            break;
        case LSB4:
            lsb4_extract(raw_data, image_buffer + sizeof(hidden_data_size) * 2, hidden_data_size, 0);
            break;
        case LSBE:
            break;
    }


    if (enc_method != ENC_METHOD_NONE) {

        // TODO

    } else {
        extension = calloc(30, 1);
        uint8_t *offset;
        int ext_size;

        switch (steg_method) {
            case LSB1:
                offset = image_buffer + sizeof(hidden_data_size) * 8 + hidden_data_size * 8;
                ext_size = lsb1_extract(extension, offset, hidden_data_size, 1);
                break;
            case LSB4:
                offset = image_buffer + sizeof(hidden_data_size) * 2 + hidden_data_size * 2;
                ext_size = lsb4_extract(extension, offset, hidden_data_size, 1);
                break;
            case LSBE:
                break;
        }


        realloc(extension, ext_size + 1);

        extension[ext_size] = 0;
    }

    char *filename = malloc(strlen(output_path) + strlen(extension) + 1);
    strcpy(filename, output_path);
    strcat(filename, extension);

    printf("Saving to: %s", filename);
    FILE *fp = fopen(filename, "wb+");

    if (fp == NULL) {
        printf("Could not open file %s: ", output_path);
        printf("%s\n", strerror(errno));
        return 0;
    }

    fwrite(raw_data, hidden_data_size, 1, fp);

    fclose(fp);

}


//
//void DumpHex(const void *data, size_t size) {
//    char ascii[17];
//    size_t i, j;
//    ascii[16] = '\0';
//    for (i = 0; i < size; ++i) {
//        printf("%02X ", ((unsigned char *) data)[i]);
//        if (((unsigned char *) data)[i] >= ' ' && ((unsigned char *) data)[i] <= '~') {
//            ascii[i % 16] = ((unsigned char *) data)[i];
//        } else {
//            ascii[i % 16] = '.';
//        }
//        if ((i + 1) % 8 == 0 || i + 1 == size) {
//            printf(" ");
//            if ((i + 1) % 16 == 0) {
//                printf("|  %s \n", ascii);
//            } else if (i + 1 == size) {
//                ascii[(i + 1) % 16] = '\0';
//                if ((i + 1) % 16 <= 8) {
//                    printf(" ");
//                }
//                for (j = (i + 1) % 16; j < 16; ++j) {
//                    printf("   ");
//                }
//                printf("|  %s \n", ascii);
//            }
//        }
//    }
//}
//
//void main() {
//    char *src = "holaholahola";//"\x00\x00\x00\x00\x00\x00\x00\x00";//;
//    uint8_t dst[10000];
//    uint8_t dst2[10000] = {0};
//
//
//    memset(dst, 0xFF, 10000);
//
//
//    lsb1_hide(dst, src, strlen(src));
//
//    lsb1_extract(dst2, dst, strlen(src),0);
//
////    DumpHex(dst2, 6);
//
//    printf("brak");
//}