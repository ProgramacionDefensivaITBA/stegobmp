//
// Created by sebikul on 23/06/18.
//

#ifndef STEGOBMP_CRYPT_H
#define STEGOBMP_CRYPT_H

#include <stdint.h>
#include "stegobmp.h"

void crypt_init(void);

void crypt_free(void);

unsigned char *
crypt_enc(ENC_METHOD enc_method, ENC_MODE enc_mode, uint8_t *plaintext, long plaintext_len, const char *password,
          int *ciphertext_length);


#endif //STEGOBMP_CRYPT_H
