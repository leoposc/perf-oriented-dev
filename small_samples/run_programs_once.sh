#!/bin/bash

set -e  # Exit immediately if a command exits with a non-zero status
trap "echo 'Script interrupted! Exiting...'; exit 1" SIGINT  # Handle Ctrl+C

mkdir -p benchmarks

# Repeat all tests 10 times
for i in {1..10}; do
    echo "Running iteration $i..."

    # Delannoy
    { time ./small_samples/build/delannoy 13; } 2> benchmarks/delannoy_13_${i}.txt

    # File generator
    { time ./small_samples/build/filegen 3 5 10240 102400; } 2> benchmarks/file_generator_3_5_10240_102400_output_${i}.txt

    # File search
    { time ./small_samples/build/filesearch; } 2> benchmarks/filesearch_output_${i}.txt

    # Matrix multiplication
    { time ./small_samples/build/mmul 10000; } 2> benchmarks/matrix_n10000_${i}.txt

    # N-body problem
    { time ./small_samples/build/nbody; } 2> benchmarks/nbody_output_${i}.txt

    # QAP
    { time ./small_samples/build/qap ./small_samples/qap/problems/chr22b.dat; } 2> benchmarks/qap_output_${i}.txt

done

echo "Benchmarking complete!"
