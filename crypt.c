//
// Created by sebikul on 23/06/18.
//

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>
#include <stdint.h>
#include "crypt.h"

static void handle_errors(void) {
    ERR_print_errors_fp(stderr);
    abort();
}

void crypt_init(void) {
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
//    OPENSSL_config(NULL);
}

void crypt_free(void) {
    EVP_cleanup();
    ERR_free_strings();
}


static void crypt_derive_key(const EVP_CIPHER *cipher, const char *password, unsigned char *key, unsigned char *iv) {
    if (EVP_BytesToKey(cipher, EVP_sha256(), NULL, (unsigned char *) password, (int) strlen(password), 1, key, iv) ==
        0) {
        handle_errors();
    }
}

static const EVP_CIPHER *crypt_get_evp_cypher(ENC_METHOD enc_method, ENC_MODE enc_mode) {

    switch (enc_method) {

        case AES128:
            switch (enc_mode) {
                case ECB:
                    return EVP_aes_128_ecb();
                case CFB:
                    return EVP_aes_128_cfb8();
                case OFB:
                    return EVP_aes_128_ofb();
                case CBC:
                    return EVP_aes_128_cbc();
                default:
                    return NULL;
            }

        case AES192:
            switch (enc_mode) {
                case ECB:
                    return EVP_aes_192_ecb();
                case CFB:
                    return EVP_aes_192_cfb8();
                case OFB:
                    return EVP_aes_192_ofb();
                case CBC:
                    return EVP_aes_192_cbc();
                default:
                    return NULL;
            }
        case AES256:
            switch (enc_mode) {
                case ECB:
                    return EVP_aes_256_ecb();
                case CFB:
                    return EVP_aes_256_cfb8();
                case OFB:
                    return EVP_aes_256_ofb();
                case CBC:
                    return EVP_aes_256_cbc();
                default:
                    return NULL;
            }
        case DES:
            switch (enc_mode) {
                case ECB:
                    return EVP_des_ecb();
                case CFB:
                    return EVP_des_cfb8();
                case OFB:
                    return EVP_des_ofb();
                case CBC:
                    return EVP_des_cbc();
                default:
                    return NULL;
            }
        default:
            return NULL;
    }
}


unsigned char *
crypt_enc(ENC_METHOD enc_method, ENC_MODE enc_mode, uint8_t *plaintext, long plaintext_len, const char *password,
          int *ciphertext_length) {

    unsigned char *key;
    unsigned char iv[16];

    const EVP_CIPHER *evp_cipher = crypt_get_evp_cypher(enc_method, enc_mode);

    if (evp_cipher == NULL) {
        return NULL;
    }

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        handle_errors();
    }
    EVP_CIPHER_CTX_set_padding(ctx, 1);

    key = malloc((size_t) EVP_CIPHER_key_length(evp_cipher));

    crypt_derive_key(evp_cipher, password, key, iv);

    if (1 != EVP_EncryptInit_ex(ctx, evp_cipher, NULL, key, iv)) {
        free(key);
        handle_errors();
    }
    free(key);

    size_t ciph_size = (size_t) (plaintext_len + EVP_CIPHER_CTX_block_size(ctx) - 1);
    printf("Allocating %d for ciphertext.\n", (int) ciph_size);
    unsigned char *ciphertext = malloc(ciph_size);

    if (1 != EVP_EncryptUpdate(ctx, ciphertext, ciphertext_length, plaintext, (int) plaintext_len)) {
        handle_errors();
    }
    int temp_len;

    if (1 != EVP_EncryptFinal(ctx, ciphertext + *ciphertext_length, &temp_len)) {
        handle_errors();
    }
    *ciphertext_length += temp_len;

    EVP_CIPHER_CTX_free(ctx);

    ciphertext = realloc(ciphertext, (size_t) *ciphertext_length);

    return ciphertext;
}


unsigned char *
crypt_dec(ENC_METHOD enc_method, ENC_MODE enc_mode, uint8_t *ciphertext, long ciphertext_len, const char *password,
          int *plaintext_length) {

    unsigned char *key;
    unsigned char iv[16];

    const EVP_CIPHER *evp_cipher = crypt_get_evp_cypher(enc_method, enc_mode);

    if (evp_cipher == NULL) {
        return NULL;
    }

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        handle_errors();
    }

    key = malloc((size_t) EVP_CIPHER_key_length(evp_cipher));

    crypt_derive_key(evp_cipher, password, key, iv);

    if (1 != EVP_DecryptInit_ex(ctx, evp_cipher, NULL, key, iv)) {
        free(key);
        handle_errors();
    }
    free(key);


    unsigned char *plaintext = malloc((size_t) (ciphertext_len + EVP_CIPHER_block_size(evp_cipher)));
    int plain_len;

    if (1 != EVP_DecryptUpdate(ctx, plaintext, &plain_len, ciphertext, (int) ciphertext_len)) {
        handle_errors();
    }
    int temp_len;

    if (1 != EVP_DecryptFinal_ex(ctx, plaintext + plain_len, &temp_len)) {
        handle_errors();
    }
    *plaintext_length = plain_len + temp_len;

    EVP_CIPHER_CTX_free(ctx);

    plaintext = realloc(plaintext, (size_t) *plaintext_length);

    return plaintext;
}