//
// Created by sebikul on 14/06/18.
//

#ifndef STEGOBMP_STREGOBMP_H
#define STEGOBMP_STREGOBMP_H

typedef enum {
    STEG_ERROR, LSB1, LSB4, LSBE, STEG_NONE
} STEG_METHOD;

typedef enum {
    ENC_METHOD_ERROR, AES128, AES192, AES256, DES, ENC_METHOD_NONE
} ENC_METHOD;

typedef enum {
    ENC_MODE_ERROR, ECB, CFB, OFB, CBC, ENC_MODE_NONE
} ENC_MODE;

STEG_METHOD stegobmp_steg_method(const char *steg_method);

ENC_METHOD stegobmp_enc_method(const char *enc_method);

ENC_MODE stegobmp_enc_mode(const char *enc_mode);

int stegobmp_embed(bmp_image_t *image, const char *input_path, STEG_METHOD steg_method, ENC_METHOD enc_method,
                   ENC_MODE enc_mode, const char *password);

#endif //STEGOBMP_STREGOBMP_H
