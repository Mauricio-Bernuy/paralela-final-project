#include <mpi.h>
#include <iostream>
using namespace std;

#define N 8
#define M 8

int main(int argc, char **argv)
{
  int size, rank;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int matrix_1[N][M];
  int matrix_2[N][M];
  int result[N][M];
  int sub_N = N / (size / 2);
  int sub_M = M / (size / 2);

  if (rank == 0)
  {
    srand(123456);
    for (int i = 0; i < N; i++)
    {
      for (int j = 0; j < M; j++)
      {
        matrix_1[i][j] = i + j;
        matrix_2[i][j] = -i - j;
      }
    }

    for (int i = 0; i < N; i++)
    {
      for (int j = 0; j < M; j++)
      {
        printf("%d ", matrix_1[i][j]);
      }
      printf("\n");
    }

    MPI_Datatype subarray;
    // int j=0;
    // int si = 8;
    // for (int i = 0; i < si; i++){
    //   if ((i+1)%(si) == si-1)
    //     j++;
    //   printf("%d,%d\n",sub_N*(j),sub_M*i);
    // }

    for (int i = 1; i < size; i++)
    {
      int start[2] = {0, 0};
      int subsize[2] = {sub_N, sub_M};
      int size[2] = {N, M};

      MPI_Type_create_subarray(2, size, subsize, start, MPI_ORDER_C, MPI_INT, &subarray);
      MPI_Type_commit(&subarray);

      MPI_Send(&(matrix_1[0][0]), 1, subarray, i, i, MPI_COMM_WORLD);
      MPI_Type_free(&subarray);
      break; // debugging
    }
    // crear propio subarray
  }

  else if (rank == 1)
  {
    int sub_matrix_1[sub_N][sub_M];
    MPI_Recv(&(sub_matrix_1[0][0]), (sub_N) * (sub_M), MPI_INT, 0, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    printf("Received submatrix:\n");
    for (int i = 0; i < sub_N; i++)
    {
      for (int j = 0; j < sub_M; j++)
      {
        printf("%d ", sub_matrix_1[i][j]);
      }
      printf("\n");
    }
  }

  // int loc_matrix[N][M];

  // MPI_Scatter(&matrix[0][0], (N * M) / size, MPI_INT, &loc_matrix[0][0], (N * M) / size, MPI_INT, 0, MPI_COMM_WORLD);
  // MPI_Bcast(&vector[0], M, MPI_INT, 0, MPI_COMM_WORLD);

  // int loc_result[N];
  // for (int i = 0; i < N / size; i++)
  // {
  //   loc_result[i] = 0;
  //   for (int j = 0; j < M; j++)
  //   {
  //     loc_result[i] += loc_matrix[i][j] * vector[j];
  //   }
  // }

  // MPI_Gather(&loc_result[0], N / size, MPI_INT, &result[0], N / size, MPI_INT, 0, MPI_COMM_WORLD);
  // if (rank == 0)
  // {
  //   for (int i = 0; i < N; i++)
  //   {
  //     cout << result[i] << " ";
  //   }
  // }

  MPI_Finalize();
  return 0;
}

// #include <stdio.h>
// #include <stdlib.h>
// #include <mpi.h>

// #define COLS  12
// #define ROWS  8

// int main(int argc, char **argv) {

//     MPI_Init(&argc, &argv);
//     int p, rank;
//     MPI_Comm_size(MPI_COMM_WORLD, &p);
//     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//     char i;

//     char a[ROWS*COLS];
//     const int NPROWS=COLS/p;  /* number of rows in _decomposition_ */
//     const int NPCOLS=ROWS/p;  /* number of cols in _decomposition_ */
//     const int BLOCKROWS = p;  /* number of rows in _block_ */
//     const int BLOCKCOLS = p; /* number of cols in _block_ */

//     // if (p != NPROWS*NPCOLS) {
//     //     fprintf(stderr,"Error: number of PEs %d != %d x %d\n", p, NPROWS, NPCOLS);
//     //     MPI_Finalize();
//     //     exit(-1);
//     // }

//     char b[BLOCKROWS*BLOCKCOLS];
//     for (int ii=0; ii<BLOCKROWS*BLOCKCOLS; ii++) b[ii] = 0;

//     MPI_Datatype blocktype;
//     MPI_Datatype blocktype2;

//     MPI_Type_vector(BLOCKROWS, BLOCKCOLS, COLS, MPI_INT, &blocktype2);
//     MPI_Type_create_resized( blocktype2, 0, sizeof(char), &blocktype);
//     MPI_Type_commit(&blocktype);

//     int disps[NPROWS*NPCOLS];
//     int counts[NPROWS*NPCOLS];
//     for (int ii=0; ii<NPROWS; ii++) {
//         for (int jj=0; jj<NPCOLS; jj++) {
//             disps[ii*NPCOLS+jj] = ii*COLS*BLOCKROWS+jj*BLOCKCOLS;
//             counts [ii*NPCOLS+jj] = 1;
//         }
//     }

//     MPI_Scatterv(a, counts, disps, blocktype, b, BLOCKROWS*BLOCKCOLS, MPI_INT, 0, MPI_COMM_WORLD);
//     /* each proc prints it's "b" out, in order */
//     for (int proc=0; proc<p; proc++) {
//         if (proc == rank) {
//             printf("Rank = %d\n", rank);
//             if (rank == 0) {
//                 printf("Global matrix: \n");
//                 for (int ii=0; ii<ROWS; ii++) {
//                     for (int jj=0; jj<COLS; jj++) {
//                         printf("%3d ",(int)a[ii*COLS+jj]);
//                     }
//                     printf("\n");
//                 }
//             }
//             printf("Local Matrix:\n");
//             for (int ii=0; ii<BLOCKROWS; ii++) {
//                 for (int jj=0; jj<BLOCKCOLS; jj++) {
//                     printf("%3d ",(int)b[ii*BLOCKCOLS+jj]);
//                 }
//                 printf("\n");
//             }
//             printf("\n");
//         }
//         MPI_Barrier(MPI_COMM_WORLD);
//     }

//     MPI_Finalize();

//     return 0;
// }