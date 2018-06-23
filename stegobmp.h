//
// Created by sebikul on 14/06/18.
//

#ifndef STEGOBMP_STREGOBMP_H
#define STEGOBMP_STREGOBMP_H


#include "crypt.h"

STEG_METHOD stegobmp_steg_method(const char *steg_method);

ENC_METHOD stegobmp_enc_method(const char *enc_method);

ENC_MODE stegobmp_enc_mode(const char *enc_mode);

int stegobmp_embed(bmp_image_t *image, const char *input_path, STEG_METHOD steg_method, ENC_METHOD enc_method,
                   ENC_MODE enc_mode, const char *password);

int stegobmp_extract(bmp_image_t *image, const char *output_path, STEG_METHOD steg_method, ENC_METHOD enc_method,
                     ENC_MODE enc_mode, const char *password);

#endif //STEGOBMP_STREGOBMP_H
