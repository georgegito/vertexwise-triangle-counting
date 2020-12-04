#!/bin/bash
#SBATCH --job-name=pds_project
#SBATCH --partition=batch
#SBATCH --ntasks=20
#SBATCH --time=00:10:00

cd ../src/v4

module load OpenCilk

for i in 1 2 8 16 20
do
    export CILK_NWORKERS=$i
    clang++ -O3 -o v4_cilk v4_cilk.cpp -fcilkplus
    for j in 1 2 3
    do 
        echo WORKERS: $i / SESSION: $j
        ./v4_cilk belgium_osm
        ./v4_cilk com-Youtube
        ./v4_cilk dblp-2010
        ./v4_cilk mycielskian13
        ./v4_cilk NACA0015
        echo ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    done
done