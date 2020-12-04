#!/bin/bash
#SBATCH --job-name=pds_project
#SBATCH --partition=batch
#SBATCH --ntasks=20
#SBATCH --time=00:10:00

cd ../src/v4

module load gcc

for i in 1 2 8 16 20
do
    export OMP_NUM_THREADS=$i
    gcc -O3 -o v4_omp v4_omp.cpp -fopenmp -lstdc++
    for j in 1 2 3
    do 
        echo WORKERS: $i / SESSION: $j
        ./v4_omp belgium_osm
        ./v4_omp com-Youtube
        ./v4_omp dblp-2010
        ./v4_omp mycielskian13
        ./v4_omp NACA0015
        echo ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    done
done
