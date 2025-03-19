#!/bin/bash


mkdir -p benchmarks

# Repeat all tests 10 times
for i in {1..10}; do

    # Delannoy
    /usr/bin/time ./delannoy 16 > /dev/null 2> benchmarks/delannoy_16_${i}.txt


    # File generator
    /usr/bin/time ./file_generator 3 5 10240 102400 > /dev/null 2> benchmarks/file_generator_3_5_10240_102400_output_${i}.txt


    # file seach
    /usr/bin/time ./filesearch > /dev/null 2> benchmarks/filesearch_output{$i}.txt


    # Matrix multiplication
    /usr/bin/time ./matrix 10000 > /dev/null 2> benchmarks/matrix_n10000_${i}.txt


    # n-body problem
    /usr/bin/time ./nbody > /dev/null 2> benchmarks/nbody_output_${i}.txt

    # qap
    /usr/bin/time ./qap ../qap/problems/chr22b.dat > /dev/null 2> benchmarks/qap_output_${i}.txt

done
