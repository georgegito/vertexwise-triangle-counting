_**Compile commands examples:**_
| language | command    |
| ---         |      --                                                   |
| C++ (sequential)  | gcc -O3 -o v3_seq v3_seq.cpp -lstdc++                     |
| Cilk (parallel)       | clang++ -O3 -o v3_cilk v3_cilk.cpp -fcilkplus             |
| OpenMP (parallel)      | gcc -O3 -o v3_omp v3_omp.cpp -fopenmp -lstdc++            |
| PThreads (parallel)   | gcc -O3 -o v4_pthreads v4_pthreads.cpp -lpthread -lstdc++ |

_**Run command example:**_ ./v3_seq '*GraphName*' (e.g. ./v3_seq com-Youtube)

_**IMPORTANT NOTE:**_ Please do not change the directory of the graphs. 
