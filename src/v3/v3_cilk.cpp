#include <iostream>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <fstream>
#include <chrono>

using namespace std;


void readmtxvalues(string f, uint32_t &n, uint32_t &nzz){

    // Open the file:
    std::ifstream fin(f);
    // Declare variables:
    uint32_t M, N;
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


void openmtxfile(string f, uint32_t *row, uint32_t *col, uint32_t &n, uint32_t &nzz){

    // Open the file:
    std::ifstream fin(f);
    // Declare variables:
    uint32_t M, N;
    // Ignore headers and comments:
    while (fin.peek() == '%') fin.ignore(2048, '\n');
    // Read defining parameters:
    fin >> M >> N >> nzz;

    for (uint32_t i = 0; i < 2*nzz; i++){
        fin >> row[i] >> col[i];
        i++;
        row[i]=col[i-1];
        col[i]=row[i-1];
    }
    fin.close();

}


/*
 *  check_edge function
 *  Checks if there is an edge between nodes a and b with binary search (O(logn)).
 *  Written for CSR. To use for SYMMETRIC CSC just call as: find_triangles(col_ptr[], int row_idx[], int n).
*/
bool check_edge(int a, int b, uint32_t row_ptr[], uint32_t col_idx[]){

    uint32_t row_start = row_ptr[a];
    uint32_t row_end = row_ptr[a + 1]-1;
    uint32_t row_middle = (row_start+row_end)/2;
    while (row_start <= row_end) {
        if (col_idx[row_middle] == b) {
            return true;
        }else if (col_idx[row_middle] < b){
            row_start = row_middle + 1;
        }else{
            row_end = row_middle - 1;
        }
        row_middle = (row_start + row_end)/2;
    }
    return false;

}


/*
 *  find_triangles function
 *  Finds vector c3 of a CSR graph
 *  Written for CSR. To use for SYMMETRIC CSC just call as: find_triangles(col_ptr[], row_idx[], n, c3).
*/
void find_triangles(uint32_t row_ptr[], uint32_t col_idx[], uint32_t n, int* c3){ 
 
    cilk_for(int i = 0; i < n; i++){                                     
        for(uint32_t idx = row_ptr[i]; idx < row_ptr[i + 1]; idx++){        
            uint32_t j = col_idx[idx];                                       // Here, there is an edge between node i and node j.
                for(uint32_t idx2 = idx + 1; idx2 < row_ptr[i + 1]; idx2++){ // We will check all nodes k > j that are connected with node i, to find out if they are connected with node j too.
                    uint32_t k = col_idx[idx2];                              // Here, there is an edge between node i and node k... All we need to do is to check for edge between j and k.
                    if(check_edge(j, k, row_ptr, col_idx)){                  // If there is an edge between j and k, we have found a triangle
                        c3[i]++;
                    }       
                }
        } 
    } 

}


/*
 *  \brief COO to CSC conversion
 *  Converts a square matrix from COO to CSC format.
 *  Note: The routine assumes the input COO and the output CSC matrix
 *  to be square.
 */
void coo2csc(
  uint32_t       * const row,       /*!< CSC row start indices */
  uint32_t       * const col,       /*!< CSC column indices */
  uint32_t const * const row_coo,   /*!< COO row indices */
  uint32_t const * const col_coo,   /*!< COO column indices */
  uint32_t const         nnz,       /*!< Number of nonzero elements */
  uint32_t const         n,         /*!< Number of rows/columns */
  uint32_t const         isOneBased /*!< Whether COO is 0- or 1-based */) {

  // ----- cannot assume that input is already 0!
  for (uint32_t l = 0; l < n+1; l++) col[l] = 0;
  // ----- find the correct column sizes
  for (uint32_t l = 0; l < nnz; l++)
    col[col_coo[l] - isOneBased]++;
  // ----- cumulative sum
  for (uint32_t i = 0, cumsum = 0; i < n; i++) {
    uint32_t temp = col[i];
    col[i] = cumsum;
    cumsum += temp;
  }
  col[n] = nnz;
  // ----- copy the row indices to the correct place
  for (uint32_t l = 0; l < nnz; l++) {
    uint32_t col_l;
    col_l = col_coo[l] - isOneBased;
    uint32_t dst = col[col_l];
    row[dst] = row_coo[l] - isOneBased;
    col[col_l]++;
  }
  // ----- revert the column pointers
  for (uint32_t i = 0, last = 0; i < n; i++) {
    uint32_t temp = col[i];
    col[i] = last;
    last = temp;
  }

}



int main(int argc, char** argv){

    uint32_t nzz;
    uint32_t n;
    string graph = argv[1];
    string file = "../../graphs/" + graph + ".mtx";

    readmtxvalues(file, n, nzz);
    
    uint32_t *coo_row = new uint32_t[2*nzz];
    uint32_t *coo_col = new uint32_t[2*nzz];

    openmtxfile(file, coo_row, coo_col, n, nzz);

    uint32_t isOneBased = 1;
    uint32_t * csc_row = new uint32_t[2*nzz];
    uint32_t * csc_col = new uint32_t[n+1];

    //Call coo2csc to convert COO matrix to CSC.
    coo2csc(csc_row, csc_col, coo_row, coo_col, 2*nzz, n, isOneBased);

    // Initialize variables.
    int triangles = 0;
    int c3[n];
    for(int i = 0; i < n; i++){
        c3[i] = 0;
    }

    // Call find_triangles for CSC matrix.
    chrono::steady_clock sc;   
    auto start = sc.now();     

    find_triangles(csc_col, csc_row, n, c3);

    auto end = sc.now();       
    auto time_span = static_cast<chrono::duration<double>>(end - start);  

    for(int i = 0; i < n; i++){
        triangles += c3[i];
    }
    triangles /= 3;

    cout << endl << "Workers: " <<  __cilkrts_get_nworkers() << endl << "Graph: " << graph <<"\nParallel operation time (Cilk): "<<time_span.count()<<" seconds\n";
    printf("Triangles: %d\n\n", triangles);

    return 0;

}