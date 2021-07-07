#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"
#include "explicitPar.h"

#define min(a,b) ((a) <= (b) ? (a) : (b))

int main(int argc, char *argv[]) {

   /* Index variables */
   int i, j, k, l;

   /* Output file descriptor */
   FILE* file;

   /* Various parameters of dimensions */
   int size_x, size_y, x_domains, y_domains;
   int size_global_x, size_global_y;
   int size_total_x, size_total_y;

   /* Current process */
   int me;

   /* Arrays */
   double **x;
   double **x0;
   double *xtemp;
   double *xfinal;

   /* For reading parameters */
   int iconf[5];
   double conf[2];

   /* Space and time steps */
   double dt, dt1, dt2, hx, hy;

   /* Current local square difference */
   double localDiff;

   /* Current global difference and limit convergence */
   double result, epsilon;

   /* Convergence pseudo-boolean */
   int convergence = 0;

   /* Time and step variables */
   double t;
   int step;

   /* Max step */
   int maxStep;

   /* Variables for clock */
   double time_init, time_final, elapsed_time;

   /* Various variables for MPI implementation */
   int nproc, ndims;
   MPI_Comm comm, comm2d;
   int dims[2];
   int periods[2];
   int reorganisation = 0;
   MPI_Datatype column_type;
   int S = 0, E = 1, N = 2, W = 3;
   int neighBor[4];
   int xcell, ycell;
   int *xs, *ys, *xe, *ye;

   /* Physical parameters */
   double temp1_init, temp2_init, k0;

   /* temp1_init: temperature init on borders */
   temp1_init = 10.0;

   /* temp2_init: temperature init inside */
   temp2_init = -10.0;

   /* Diffusivity coefficient */
   k0 = 1;

   /* MPI Initialization */
   MPI_Init(&argc, &argv);
   comm = MPI_COMM_WORLD;
   MPI_Comm_size(comm,&nproc);
   MPI_Comm_rank(comm,&me);

   /* Get input parameters */
   if (me==0)
     readParam(iconf,conf);

   /* Broadcast input parameters */
   MPI_Bcast(iconf,5,MPI_INT,0,comm);
   MPI_Bcast(conf,2,MPI_DOUBLE,0,comm);

   /* Assign input parameters to variables */
   size_x    = iconf[0];
   size_y    = iconf[1];
   x_domains = iconf[2];
   y_domains = iconf[3];
   maxStep   = iconf[4];
   dt1       = conf[0];
   epsilon   = conf[1];

   /* Warning message if dimensions and number of processes don't match */
   if ((me==0) && (nproc!=(x_domains*y_domains)))
     printf("Number of processes not equal to Number of subdomains\n");

   /* Various other variables */
   size_global_x = size_x+2;
   size_global_y = size_y+2;
   hx = 1.0/(double)(size_global_x);
   hy = 1.0/(double)(size_global_y);
   dt2 = 0.25*(min(hx,hy)*min(hx,hy))/k0;
   size_total_x = size_x+2*x_domains+2;
   size_total_y = size_y+2*y_domains+2;

   /* Take a right time step for convergence */
   if (dt1>=dt2) {
     if (me==0) {
       printf("\n");
       printf("  Time step too large in 'param' file -"
              " Taking convergence criterion\n");
     }
     dt = dt2;
   }
   else dt = dt1;

   /* Allocate final 1D array */
   xfinal = malloc(size_x*size_y*sizeof(*xfinal));

   /* Allocate 2D contiguous arrays x and x0 */
   /* Allocate size_total_x rows */
   x = malloc(size_total_x*sizeof(*x));
   x0 = malloc(size_total_x*sizeof(*x0));
   /* Allocate x[0] and x0[0] for contiguous arrays */
   x[0] = malloc(size_total_x*size_total_y*sizeof(**x));
   x0[0] = malloc(size_total_x*size_total_y*sizeof(**x0));
   /* Loop on rows */
   for (i=1;i<size_total_x;i++) {
      /* Increment size_total_x block on x[i] and x0[i] address */
      x[i] = x[0] + i*size_total_y;
      x0[i] = x0[0] + i*size_total_y;
   }

   /* Allocate coordinates of processes */
   xs = malloc(nproc*sizeof(int));
   xe = malloc(nproc*sizeof(int));
   ys = malloc(nproc*sizeof(int));
   ye = malloc(nproc*sizeof(int));

   /* Create 2D cartesian grid */
   periods[0] = 0;
   periods[1] = 0;
   /* Number of dimensions */
   ndims = 2;
   /* Invert (Ox,Oy) classic convention */
   dims[0] = y_domains;
   dims[1] = x_domains;
   MPI_Cart_create(comm, ndims, dims, periods, reorganisation, &comm2d);

   /* Identify neighBors */
   neighBor[0] = MPI_PROC_NULL;
   neighBor[1] = MPI_PROC_NULL;
   neighBor[2] = MPI_PROC_NULL;
   neighBor[3] = MPI_PROC_NULL;

   /* Left/West and Right/East neighBors */
   MPI_Cart_shift(comm2d, 0, 1, &neighBor[W], &neighBor[E]);

   /* Bottom/South and Upper/North neighBors */
   MPI_Cart_shift(comm2d, 1, 1, &neighBor[S], &neighBor[N]);

   /* Size of each cell */
   xcell = (size_x/x_domains);
   ycell = (size_y/y_domains);

   /* Allocate subdomain */
   xtemp = malloc(xcell*ycell*sizeof(*xtemp));

   /* Compute xs, xe, ys, ye for each cell on the grid */
   processToMap(xs, ys, xe, ye, xcell, ycell, x_domains, y_domains);

   /* Create column data type to communicate with East and West neighBors */
   MPI_Type_vector(xcell, 1, size_total_y, MPI_DOUBLE, &column_type);
   MPI_Type_commit(&column_type);

   /* Initialize values */
   initValues(x0, size_total_x, size_total_y, temp1_init, temp2_init);

   /* Update the boundaries */
   updateBound(x0, neighBor, comm2d, column_type, me, xs, ys, xe, ye, ycell);

   /* Initialize step and time */
   step = 0;
   t = 0.0;

   /* Starting time */
   time_init = MPI_Wtime();

   /* Main loop : until convergence */
   while (!convergence) {
      /* Increment step and time */
      step = step+1;
      t = t+dt;
      /* Perform one step of the explicit scheme */
      computeNext(x0, x, dt, hx, hy, &localDiff, me, xs, ys, xe, ye, k0);
      /* Update the partial solution along the interface */
      updateBound(x0, neighBor, comm2d, column_type, me, xs, ys, xe, ye, ycell);
      /* Sum reduction to get global difference */
      MPI_Allreduce(&localDiff, &result, 1, MPI_DOUBLE, MPI_SUM, comm);
      /* Current global difference with convergence */
      result= sqrt(result);
      /* Break if convergence reached or step greater than maxStep */
      if ((result<epsilon) || (step>maxStep)) break;
   }

   /* Ending time */
   time_final = MPI_Wtime();
   /* Elapsed time */
   elapsed_time = time_final - time_init;

   /* Gather all subdomains :
      inner loop on columns index (second index)
      to optimize since C is row major */
   j=1;
   for (i=xs[me];i<=xe[me];i++) {
      for (k=0;k<ycell;k++)
         xtemp[(j-1)*ycell+k] = x0[i][ys[me]+k];
      j=j+1;
   }

   /* Perform gathering */
   MPI_Gather(xtemp, xcell*ycell, MPI_DOUBLE, xfinal, xcell*ycell, MPI_DOUBLE, 0, comm);

   /* Print results */
   if (me == 0) {
     printf("\n");
     printf("  Time step = %.9e\n",dt);
     printf("\n");
     printf("  Convergence = %.9f after %d steps\n",epsilon,step);
     printf("\n");
     printf("  Problem size = %d\n",size_x*size_y);
     printf("\n");
     printf("  Wall Clock = %.9f\n",elapsed_time);
     printf("\n");
     printf("  Computed solution in outputPar.dat\n");
     printf("\n");

     /* Store solution into output file :
        x_domains = width
        y_domains = height */
     file=fopen("outputPar.dat","w");
     for (i=1;i<=size_x+1;i++)
        fprintf(file,"%15.11f ",temp1_init);
     fprintf(file,"%15.11f\n",temp1_init);
     for (i=1;i<=y_domains;i++)
        for (j=0;j<ycell;j++) {
           fprintf(file,"%15.11f ",temp1_init);
           for (k=1;k<=x_domains;k++) {
              for (l=0;l<xcell;l++)
                 fprintf(file,"%15.11f ",xfinal[(i-1)*x_domains*xcell*ycell+
                         (k-1)*xcell*ycell+l*ycell+j]);
           }
           fprintf(file,"%15.11f\n",temp1_init);
        }
     for (i=1;i<=size_x+1;i++)
        fprintf(file,"%15.11f ",temp1_init);
     fprintf(file,"%15.11f\n",temp1_init);
     fclose(file);
   }

   /* Free all arrays */
   free(x[0]);
   free(x);
   free(x0[0]);
   free(x0);
   free(xtemp);
   free(xfinal);
   free(xs);
   free(ys);
   free(xe);
   free(ye);

   /* Free column type */
   MPI_Type_free(&column_type);

   /* Finish MPI */
   MPI_Finalize();

   return 0;
}
