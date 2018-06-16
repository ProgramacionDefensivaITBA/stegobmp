#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "bmp.h"
#include "stegobmp.h"

struct {
    unsigned int embed;
    unsigned int extract;
    char *in_path;
    char *carrier_path;
    char *out_path;
    char *steg_method;
    char *enc_method;
    char *enc_mode;
    char *password;
} arg_opts = {0};


void usage() {
    printf("Requeridos:\n"
           "\t-embed\t\t\t\t\t Indica que se va a ocultar información.\n"
           "\t-extract\t\t\t\t\t Indica que se va a extraer información.\n"
           "\t-in <file>\t\t\t\t Archivo que se va a ocultar.\n"
           "\t-p <bitmapfile>\t\t\t Archivo bmp que será el portador.\n"
           "\t-out bitmapfile\t\t\t Archivo bmp de salida, es decir, el archivo bitmapfile con la información de file incrustada.\n"
           "\t-steg <LSB1|LSB4|LSBE>\t algoritmo de esteganografiado: LSB de 1bit, LSB de 4 bits, LSB Enhanced\n"
           "\nOpcionales:\n"
           "\t-a <aes128|aes192|aes256|des>\n"
           "\t-m <ecb|cfb|ofb|cbc>\n"
           "\t-pass <password>\n");
}


int main(int argc, char *argv[]) {
    int opt = 0;

    static struct option long_options[] = {
            {"embed",   no_argument,       0, 'e'},
            {"extract", no_argument,       0, 'x'},
            {"in",      required_argument, 0, 'i'},
            {"p",       required_argument, 0, 'p'},
            {"out",     required_argument, 0, 'o'},
            {"steg",    required_argument, 0, 's'},
            {"a",       required_argument, 0, 'a'},
            {"m",       required_argument, 0, 'm'},
            {"pass",    required_argument, 0, 'c'},
            {0, 0,                         0, 0}
    };

    int long_index = 0;
    while ((opt = getopt_long_only(argc, argv, "", long_options, &long_index)) != -1) {
        switch (opt) {
            case 'e' :
                arg_opts.embed = 1;
                break;

            case 'x' :
                arg_opts.extract = 1;
                break;

            case 'i':
                arg_opts.in_path = optarg;
                break;

            case 'p':
                arg_opts.carrier_path = optarg;
                break;

            case 'o':
                arg_opts.out_path = optarg;
                break;

            case 's':
                arg_opts.steg_method = optarg;
                break;

            case 'a':
                arg_opts.enc_method = optarg;
                break;

            case 'm':
                arg_opts.enc_mode = optarg;
                break;

            case 'c':
                arg_opts.password = optarg;
                break;

            default:
                usage();
                exit(EXIT_FAILURE);
        }
    }

    if (!(arg_opts.embed ^ arg_opts.extract)) {
        usage();
        exit(EXIT_FAILURE);
    }

    if (arg_opts.embed) {
        printf("[+] Embeding %s into %s\n", arg_opts.in_path, arg_opts.carrier_path);
    } else {
        printf("[+] Extracting %s from %s\n", arg_opts.out_path, arg_opts.carrier_path);
    }

    STEG_METHOD steg_method = STEG_NONE;
    ENC_METHOD enc_method = ENC_METHOD_NONE;
    ENC_MODE enc_mode = ENC_MODE_NONE;

    if (arg_opts.steg_method) {
        steg_method = stegobmp_steg_method(arg_opts.steg_method);
        if (steg_method == STEG_ERROR) {
            printf("Invalid steg method: %s\n", arg_opts.steg_method);
            return 1;
        }
        printf("\t\tSteg Method: %s\n", arg_opts.steg_method);
    }

    if (arg_opts.enc_method) {
        enc_method = stegobmp_enc_method(arg_opts.enc_method);
        if (enc_method == ENC_METHOD_ERROR) {
            printf("Invalid enc method: %s\n", arg_opts.enc_method);
            return 1;
        }
    }

    if (enc_method == ENC_METHOD_NONE) {
        printf("\t\tEncryption Method: None\n");
    } else {
        printf("\t\tEncryption Method: %s\n", arg_opts.enc_method);
    }

    if (arg_opts.enc_mode) {
        enc_mode = stegobmp_enc_mode(arg_opts.enc_mode);
        if (enc_mode == ENC_MODE_ERROR) {
            printf("Invalid enc mode: %s\n", arg_opts.enc_mode);
            return 1;
        }
    }

    if (enc_mode == ENC_MODE_NONE) {
        printf("\t\tEncryption Mode: None\n");
    } else {
        printf("\t\tEncryption Mode: %s\n", arg_opts.enc_mode);
    }

    bmp_image_t *image = bmp_from_path(arg_opts.carrier_path);

    if (image == NULL) {
        return 2;
    }

    if (arg_opts.embed) {
        stegobmp_embed(image, arg_opts.in_path, steg_method, enc_method, enc_mode, arg_opts.password);
        bmp_save(image, arg_opts.out_path);
    }

    if (arg_opts.extract) {
        stegobmp_extract(image, arg_opts.out_path, steg_method, enc_method, enc_mode, arg_opts.password);
    }

    bmp_free(image);


    return 0;
}