#!/bin/bash
#SBATCH --job-name=pds_project
#SBATCH --partition=batch
#SBATCH --ntasks=20
#SBATCH --time=00:10:00

cd ../src/v3

module load gcc

for i in 1 2 8 16 20
do
    export OMP_NUM_THREADS=$i
    gcc -O3 -o v3_omp v3_omp.cpp -fopenmp -lstdc++
    for j in 1 2 3
    do 
        echo WORKERS: $i / SESSION: $j
        ./v3_omp belgium_osm
        ./v3_omp com-Youtube
        ./v3_omp dblp-2010
        ./v3_omp mycielskian13
        ./v3_omp NACA0015
        echo ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    done
done
