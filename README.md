# stegobmp

Small program to perform steganography on BMP files.

## Dependencies

To compile stegobmp, you will need to have the development libraries of OpenSSL, or an API
compatible library installed, and `cmake`.

On Ubuntu, or other Debian based distributions, execute:

    $ sudo apt install libssl-dev cmake
    
## Compilation

    $ mkdir build/
    $ cd build/
    $ cmake ..
    $ make
    
## Running

     $ ./stegobmp                                                                                                                                                                                                               [23:20:49]
    Requeridos:
            -embed                   Indica que se va a ocultar información.
            -extract                 Indica que se va a extraer información.
            -in <file>               Archivo que se va a ocultar.
            -p <bitmapfile>          Archivo bmp que será el portador.
            -out bitmapfile          Archivo bmp de salida, es decir, el archivo bitmapfile con la información de file incrustada.
            -steg <LSB1|LSB4|LSBE>   algoritmo de esteganografiado: LSB de 1bit, LSB de 4 bits, LSB Enhanced
    
    Opcionales:
            -a <aes128|aes192|aes256|des>
            -m <ecb|cfb|ofb|cbc>
            -pass <password>

## Samples

    $ ./stegobmp -extract -p "sample/ladoLSB4.bmp" -out "sample/mensajeLSB4" -steg LSB4
    
    $ ./stegobmp -extract -p "sample/budapest.bmp" -out "sample/budapest.out" \
        -steg LSB4 -a aes128 -m ecb -pass "sorpresa"
    
    $ ./stegobmp -embed -p "sample/lado.bmp" -out "sample/mensajeLSB4aes256ofb.bmp" \
        -in "sample/mensaje1.txt" -steg LSB4 -a aes256 -m cbc -pass "secreto"