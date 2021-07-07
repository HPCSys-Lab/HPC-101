#include "mpi.h"

/*******************************************************************/
/*             Update Bounds of subdomain with me process          */
/*******************************************************************/

void updateBound(double** x, int neighBor[], MPI_Comm comm2d, MPI_Datatype column_type, int me, int* xs, int* ys,
                 int* xe, int* ye, int ycell) {

   int S = 0, E = 1, N = 2, W = 3;
   int flag;
   MPI_Status status;

   /****************** North/South communication ******************/
   flag = 1;
   /* Send my boundary to North and receive from South */
   MPI_Sendrecv(&x[xe[me]][ys[me]], ycell, MPI_DOUBLE, neighBor[N], flag,
     &x[xs[me]-1][ys[me]], ycell, MPI_DOUBLE, neighBor[S], flag, comm2d, &status);

   /* Send my boundary to South and receive from North */
   MPI_Sendrecv(&x[xs[me]][ys[me]], ycell, MPI_DOUBLE, neighBor[S], flag,
     &x[xe[me]+1][ys[me]], ycell, MPI_DOUBLE, neighBor[N], flag, comm2d, &status);

   /****************** East/West communication ********************/
   flag = 2;
   /* Send my boundary to East and receive from West */
   MPI_Sendrecv(&x[xs[me]][ye[me]], 1, column_type, neighBor[E], flag,
     &x[xs[me]][ys[me]-1], 1, column_type, neighBor[W], flag, comm2d, &status);

   /* Send my boundary to West and receive from East */
   MPI_Sendrecv(&x[xs[me]][ys[me]], 1, column_type, neighBor[W], flag,
     &x[xs[me]][ye[me]+1], 1, column_type, neighBor[E], flag, comm2d, &status);
}
