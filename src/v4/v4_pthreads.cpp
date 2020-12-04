#include <iostream>  
#include <string>
#include <fstream>
#include <chrono>
#include <pthread.h>

using namespace std;


// global variables 
int iteration; 
pthread_mutex_t mutex; 

struct arguments{

  int* row_ptr;
  int* col_idx;
  int* c3;
  int n;

} ;

void readmtxvalues(string f, int &n, int &nzz){

    // Open the file:
    std::ifstream fin(f);
    // Declare variables:
    int M, N;
    // Ignore headers and comments:
    while (fin.peek() == '%') fin.ignore(2048, '\n');
    // Read defining parameters:
    fin >> M >> N >> nzz;
    if (M == N){
        n = M;
    }else{
        std::cout << "not square" << std::endl;
    }
    fin.close();
    
}


void openmtxfile(string f, int *row, int *col, int &n, int &nzz){

    // Open the file:
    std::ifstream fin(f);
    // Declare variables:
    int M, N;
    // Ignore headers and comments:
    while (fin.peek() == '%') fin.ignore(2048, '\n');
    // Read defining parameters:
    fin >> M >> N >> nzz;
    

    for (int i = 0; i < 2*nzz; i++){
        fin >> row[i] >> col[i];
        i++;
        row[i]=col[i-1];
        col[i]=row[i-1];
    }
    fin.close();

}


void coo2csc(
    int       * const row,       /*!< CSC row start indices */
    int       * const col,       /*!< CSC column indices */
    int const * const row_coo,   /*!< COO row indices */
    int const * const col_coo,   /*!< COO column indices */
    int const         nnz,       /*!< Number of nonzero elements */
    int const         n,         /*!< Number of rows/columns */
    int const         isOneBased /*!< Whether COO is 0- or 1-based */) {

    // ----- cannot assume that input is already 0!
    for (int l = 0; l < n+1; l++) col[l] = 0;
    // ----- find the correct column sizes
    for (int l = 0; l < nnz; l++)
        col[col_coo[l] - isOneBased]++;
    // ----- cumulative sum
    for (int i = 0, cumsum = 0; i < n; i++) {
        int temp = col[i];
        col[i] = cumsum;
        cumsum += temp;
    }
    col[n] = nnz;
    // ----- copy the row indices to the correct place
    for (int l = 0; l < nnz; l++) {
        int col_l;
        col_l = col_coo[l] - isOneBased;
        int dst = col[col_l];
        row[dst] = row_coo[l] - isOneBased;
        col[col_l]++;
    }
    // ----- revert the column pointers
    for (int i = 0, last = 0; i < n; i++) {
        int temp = col[i];
        col[i] = last;
        last = temp;
    }

}


// common_neighbors function
// Compare two rows (row1, row2) and return the number of common neighbors with O(k + l), if row1 has k neighbors and row2 l.
int common_neighbors(int row1, int row2, int row_ptr[], int col_idx[]){                

    int cnt = 0;
    int ptr1 = 0;
    int ptr2 = 0;
    while(row_ptr[row1] + ptr1 < row_ptr[row1 + 1] && row_ptr[row2] + ptr2 < row_ptr[row2 + 1]){
        if(col_idx[row_ptr[row1] + ptr1] < col_idx[row_ptr[row2] + ptr2]){
            ptr1++;
        }
        else if(col_idx[row_ptr[row1] + ptr1] > col_idx[row_ptr[row2] + ptr2]){
            ptr2++;
        }
        else{
            cnt++;
            ptr1++;
            ptr2++;
        }
    }
    return cnt;

}


// find_triangles function
void *find_triangles(void *args){

    int t_it;                                                     //thread iteration
    struct arguments *t_args = (struct arguments *)args;

    while(iteration < t_args->n){
        pthread_mutex_lock(&mutex);
        t_it = iteration;
        iteration++;
        pthread_mutex_unlock(&mutex); 
        t_args->c3[t_it] = 0;
        for(int idx = t_args->row_ptr[t_it]; idx < t_args->row_ptr[t_it + 1]; idx++){                                                              
            t_args->c3[t_it] += common_neighbors(t_it, t_args->col_idx[idx], t_args->row_ptr, t_args->col_idx);
        }
        t_args->c3[t_it] = t_args->c3[t_it]/2;
    }
    return 0;

}



int main(int argc, char** argv){

    int num_threads = atoi(argv[2]);
    pthread_t threads[num_threads];
    int rc;
    long t;

    struct arguments args;

    int nzz;
    string graph = argv[1];
    string file = "../../graphs/" + graph + ".mtx";

    readmtxvalues(file, args.n, nzz);
    
    int *coo_row = new int[2*nzz];
    int *coo_col = new int[2*nzz];

    openmtxfile(file, coo_row, coo_col, args.n, nzz);

    int isOneBased = 1;
    args.col_idx = new int[2*nzz];
    args.row_ptr = new int[args.n+1];

    // Call coo2csc to convert COO matrix to CSC.
    coo2csc(args.col_idx, args.row_ptr, coo_row, coo_col, 2*nzz, args.n, isOneBased);

    args.c3 = new int[args.n];
    int triangles = 0;
    
    chrono::steady_clock sc;   
    auto start = sc.now();  
    // START of proccess - Start timer here.
    
    for(t = 0; t < num_threads; t++){
        rc = pthread_create(&threads[t], NULL, find_triangles, &args);
        if(rc){
            printf("Error: return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
    for(int i = 0; i < num_threads; i++){
    pthread_join(threads[i], NULL);
    }
    
    // END of proccess - Stop timer here.
    auto end = sc.now();       
    auto time_span = static_cast<chrono::duration<double>>(end - start);   

    pthread_mutex_destroy(&mutex);
    
    for(int i = 0; i < args.n; i++){
        triangles += args.c3[i];
    }
    triangles /= 3;

    cout << endl << "Workers: " << num_threads << endl << "Graph: " << graph << "\nParallel operation time (PThreads): " << time_span.count() << " seconds\n";
    printf("Triangles: %d", triangles);
    
    printf("\n\n");
    pthread_exit(NULL);

    return 0; 

}