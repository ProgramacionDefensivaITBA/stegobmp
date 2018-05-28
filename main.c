#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "bmp.h"


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

    bmp_image_t *image = bmp_from_path("/home/sebikul/Proyectos/stegobmp/sample/barbara.bmp");

    bmp_save(image, "/home/sebikul/Proyectos/stegobmp/sample/barbara.save.bmp");

    return 0;
}