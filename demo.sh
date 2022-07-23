#!/bin/bash

#!/bin/bash
gcc -g -o main *.c
echo "cache size, block size, set degree, miss rate"> $2
for set_degree in 1 2 4 8; do
    ./main $1 64 4 $set_degree >> $2
done
echo >> $2

rm main