_**Compile commands examples:**_
| type | command    |
| ---         |      --                                                   |
| Sequential  | gcc -O3 -o v3_seq v3_seq.cpp -lstdc++                     |
| Cilk        | clang++ -O3 -o v3_cilk v3_cilk.cpp -fcilkplus             |
| OpenMP      | gcc -O3 -o v3_omp v3_omp.cpp -fopenmp -lstdc++            |
| PThreads    | gcc -O3 -o v4_pthreads v4_pthreads.cpp -lpthread -lstdc++ |

_**Run command example:**_ ./v3_seq '*GraphName*'
