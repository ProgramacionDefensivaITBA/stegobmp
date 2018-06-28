#!/bin/bash

declare -a images=("avatar" "quito" "buenosaires" "budapest")
declare -a algorithm=("LSB1" "LSB4" "LSBE")
declare -a encrypt=("aes128" "aes192" "aes256" "des")
declare -a mode=("ecb" "cfb" "ofb" "cbc")

password=sorpresa

for img in "${images[@]}"
do
	image="finalfiles/$img.bmp"
	for i in "${algorithm[@]}"
	do

		for j in "${encrypt[@]}"
		do
			for k in "${mode[@]}"
			do
				modetu=$k
				encrypttu=$j
				algoritmtu=$i
				
				out="finalfiles/$img-descubierto-$algoritmtu-$modetu-$encrypttu"
			
				./build/stegobmp -extract -p "$image" -out "$out" -steg $algoritmtu -a $encrypttu -m $modetu -pass $password
				sleep 1
				echo ""
			done
		done
	done
done

