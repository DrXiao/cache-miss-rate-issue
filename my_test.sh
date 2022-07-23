#!/bin/bash

gcc -g -o main *.c
echo "cache size, block size, set degree, miss rate"> $2
for cache_size in 128 256 512 1024 2048; do
    for block_size in 2 4 8 16 32; do
        for set_degree in 1 2 4 8 16 32; do
            ./main $1 $cache_size $block_size $set_degree >> $2
        done
        echo >> $2
    done
    echo >> $2
done

rm main