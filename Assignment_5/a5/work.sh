#!/bin/bash
export OMP_NUM_THREADS=${1}
echo "Running (${2}x${3}:${4}) with ${OMP_NUM_THREADS} threads"
time ./heat-equation ${2} ${3} ${4}
