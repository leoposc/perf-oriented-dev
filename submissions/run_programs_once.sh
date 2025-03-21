#!/bin/bash

set -e  # Exit immediately if a command exits with a non-zero status
trap "echo 'Script interrupted! Exiting...'; exit 1" SIGINT  # Handle Ctrl+C

mkdir -p benchmarks

# Delannoy
# Function to generate a unique filename
generate_unique_filename() {
    local base_name=$1
    local ext=$2
    local i=1
    local new_name="${base_name}${ext}"
    while [[ -e $new_name ]]; do
        new_name="${base_name}_${i}${ext}"
        ((i++))
    done
    echo $new_name
}

for i in {1..3}; do
    # Delannoy
    delannoy_file=$(generate_unique_filename "benchmarks/delannoy_13" ".txt")
    { time ./small_samples/build/delannoy 13; } 2> "$delannoy_file"

    # File generator
    filegen_file=$(generate_unique_filename "benchmarks/file_generator_10_50_20240_502400_output" ".txt")
    { time ./small_samples/build/filegen 10 50 20240 502400; } 2> "$filegen_file"

    # File search
    filesearch_file=$(generate_unique_filename "benchmarks/filesearch_output" ".txt")
    { time ./small_samples/build/filesearch; } 2> "$filesearch_file"

    # Matrix multiplication
    mmul_file=$(generate_unique_filename "benchmarks/matrix_n20000" ".txt")
    { time ./small_samples/build/mmul; } 2> "$mmul_file"

    # N-body problem
    nbody_file=$(generate_unique_filename "benchmarks/nbody_output" ".txt")
    { time ./small_samples/build/nbody; } 2> "$nbody_file"

    # QAP
    { time ./small_samples/build/qap ./small_samples/qap/problems/chr15b.dat; } 2> benchmarks/qap_output.txt
done

echo "Benchmarking complete!"
