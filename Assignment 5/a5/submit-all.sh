#!/bin/bash

WIDTH=1000
HEIGHT=1000
ROUNDS=1000

for n in {1..64}; do
sbatch --exclusive work.sh $n $WIDTH $HEIGHT $ROUNDS
done
