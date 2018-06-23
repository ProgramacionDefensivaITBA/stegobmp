//
// Created by sebikul on 23/06/18.
//

#ifndef STEGOBMP_CRYPT_H
#define STEGOBMP_CRYPT_H

#include <stdint.h>

typedef enum {
    STEG_ERROR, LSB1, LSB4, LSBE, STEG_NONE
} STEG_METHOD;

typedef enum {
    ENC_METHOD_ERROR, AES128, AES192, AES256, DES, ENC_METHOD_NONE
} ENC_METHOD;

typedef enum {
    ENC_MODE_ERROR, ECB, CFB, OFB, CBC, ENC_MODE_NONE
} ENC_MODE;


void crypt_init(void);

void crypt_free(void);

unsigned char *
crypt_enc(ENC_METHOD enc_method, ENC_MODE enc_mode, uint8_t *plaintext, long plaintext_len, const char *password,
          int *ciphertext_length);

unsigned char *
crypt_dec(ENC_METHOD enc_method, ENC_MODE enc_mode, uint8_t *ciphertext, long ciphertext_len, const char *password,
          int *plaintext_length);

#endif //STEGOBMP_CRYPT_H
