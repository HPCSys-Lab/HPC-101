#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "explicitSeq.h"

#define min(a,b) ((a) <= (b) ? (a) : (b))

int main(void) {

   /* Index variables */
   int i, j;

   /* Output file descriptor */
   FILE* file;

   /* Dimensions parameters */
   int size_x, size_y, size_total_x, size_total_y;

   /* Arrays */
   double **x;
   double **x0;

   /* Space and time steps */
   double dt1, dt2, dt, hx, hy;

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
   clock_t time_init, time_final;
   double elapsed_time;

   /* Physical parameters */
   double temp1_init, temp2_init, k0;

   /* temp1_init: temperature init on borders */
   temp1_init = 10.0;

   /* temp2_init: temperature init inside */
   temp2_init = -10.0;

   /* Diffusivity coefficient */
   k0 = 1;

   /* Get input parameters */
   printf("Size x of the square \n");
   scanf("%d", &size_x);
   printf("Size y of the square \n");
   scanf("%d", &size_y);
   printf("Max. number of steps \n");
   scanf("%d", &maxStep);
   printf("Time step\n");
   scanf("%lf", &dt1);
   printf("Convergence \n");
   scanf("%lf", &epsilon);

   /* Define total sizes */
   size_total_x = size_x+2;
   size_total_y = size_y+2;

   /* Compute space and time steps */
   hx = 1.0/(double)(size_total_x);
   hy = 1.0/(double)(size_total_y);
   dt2 = 0.25*(min(hx,hy)*min(hx,hy))/k0;

   /* Take a right time step for convergence */
   if (dt1>=dt2) {
     printf("\n");
     printf("  Time step too large in 'param' file -"
            " Taking convergence criterion\n");
     dt = dt2;
   }
   else dt = dt1;

   /* Allocate 2D arrays x and x0 :
      size_total_x rows and size_total_y columns */
   x = malloc(size_total_x*sizeof(*x));
   x0 = malloc(size_total_x*sizeof(*x0));
   for (i=0;i<size_total_x;i++) {
      x[i] = malloc(size_total_y*sizeof(**x));
      x0[i] = malloc(size_total_y*sizeof(**x0));
   }

   /* Initialize values */
   initValues(x0, size_x, size_y, temp1_init, temp2_init);

   /* Initialize step and time */
   step = 0;
   t = 0.0;

   /* Starting time */
   time_init = clock();

   /* Main loop : until convergence */
   while (!convergence) {
      /* Increment step and time */
      step = step+1;
      t = t+dt;
      /* Perform one step of the explicit scheme */
      computeNext(x0, x, size_x, size_y, dt, hx, hy, &result, k0);
      /* Current global difference */
      result = sqrt(result);
      /* Break if convergence reached or step greater than maxStep */
      if ((result<epsilon) || (step>=maxStep)) break;
   }

   /* Ending time */
   time_final = clock();
   /* Elapsed time */
   elapsed_time = (time_final - time_init)*1e-6;

   /* Print results */
   printf("\n");
   printf("  Time step = %.9e\n",dt);
   printf("\n");
   printf("  Convergence = %.9f after %d steps\n",epsilon,step);
   printf("\n");
   printf("  Problem size = %d\n",size_x*size_y);
   printf("\n");
   printf("  Wall Clock = %.9f\n",elapsed_time);
   printf("\n");
   printf("  Computed solution in outputSeq.dat\n");
   printf("\n");

   /* Store solution into output file :
      size_total_x = width
      size_total_y = height */
   file=fopen("outputSeq.dat","w");
   for (j=0;j<size_total_y;j++) {
      for (i=0;i<size_total_x;i++) {
         if (i) fputc(' ',file);
         fprintf(file,"%15.11f",x0[i][j]);
      }
      fprintf(file,"\n");
   }
   fclose(file);

   /* Free all arrays */
   for (i=0;i<size_total_x;i++) {
      free(x[i]);
      free(x0[i]);
   }
   free(x);
   free(x0);

   return 0;
}
