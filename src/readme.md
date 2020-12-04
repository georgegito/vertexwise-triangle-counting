**Compile commands**:

1) Sequential implementations: gcc -O3 -o v3_seq v3_seq.cpp -lstdc++
2) Cilk implementations: clang++ -O3 -o v3_cilk v3_cilk.cpp -fcilkplus
3) OpenMP implementations: gcc -O3 -o v3_omp v3_omp.cpp -fopenmp -lstdc++
4) PThreads implementations: gcc -O3 -o v4_pthreads v4_pthreads.cpp -lpthread -lstdc++

**Run command**: ./v3_seq '*GraphName*'


