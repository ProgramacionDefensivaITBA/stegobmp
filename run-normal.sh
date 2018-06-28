#!/bin/bash

declare -a images=("avatar" "quito" "buenosaires" "budapest")
declare -a algorithm=("LSB1" "LSB4" "LSBE")

for img in "${images[@]}"
do
	image="finalfiles/$img.bmp"
	for i in "${algorithm[@]}"
	do
		algoritmtu=$i
		out="finalfiles/$img-descubierto-$algoritmtu"
		./build/stegobmp -extract -p "$image" -out "$out" -steg $i

		sleep 1
		echo ""

	done
done

