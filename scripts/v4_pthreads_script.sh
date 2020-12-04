#!/bin/bash
#SBATCH --job-name=pds_project
#SBATCH --partition=batch
#SBATCH --ntasks=20
#SBATCH --time=00:10:00

cd ../src/v4

module load gcc

gcc -O3 -o v4_pthreads v4_pthreads.cpp -lpthread -lstdc++

for i in 1 2 8 16 20
do
    for j in 1 2 3
    do 
        echo WORKERS: $i / SESSION: $j
        ./v4_pthreads belgium_osm $i
        ./v4_pthreads com-Youtube $i
        ./v4_pthreads dblp-2010 $i
        ./v4_pthreads mycielskian13 $i
        ./v4_pthreads NACA0015 $i
        echo ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    done
done
