
/***************************************************************************/
/*                                                                         */
/* This subroutine computes next values in subdomain of current process me */
/*                                                                         */
/***************************************************************************/

void computeNext(double** x0, double** x, double dt, double hx, double hy,
                 double* diff, int me, int* xs, int* ys, int* xe, int* ye, double k0) {

   /* Index variables */
   int i, j;

   /* Factors for the stencil */
   double diagx, diagy, weightx, weighty;

   /* Local variable for computing difference */
   double ldiff;

   /*
      The stencil of the explicit operator for the heat equation
      on a regular rectangular grid using a five point finite difference
      scheme in space is :

      |                                    weightx * x[i-1][j]                                   |
      |                                                                                          |
      | weighty * x[i][j-1]   (diagx * weightx + diagy * weighty) * x[i][j]  weighty * x[i][j+1] |
      |                                                                                          |
      |                                    weightx * x[i+1][j]                                   | */

   diagx = -2.0 + hx*hx/(2*k0*dt);
   diagy = -2.0 + hy*hy/(2*k0*dt);
   weightx = k0*dt/(hx*hx);
   weighty = k0*dt/(hy*hy);

   /* Perform an explicit update on the points within the domain.
      Optimization : inner loop on columns index (second index) since
      C is row major */
   for (i=xs[me];i<=xe[me];i++)
      for (j=ys[me];j<=ye[me];j++)
         x[i][j] = weightx*(x0[i-1][j] + x0[i+1][j] + x0[i][j]*diagx)
                 + weighty*(x0[i][j-1] + x0[i][j+1] + x0[i][j]*diagy);

   /* Compute the difference into domain for convergence.
      Update the value x0(i,j).
      Optimization : inner loop on columns index (second index) since
      C is row major */
   *diff = 0.0;
   for (i=xs[me];i<=xe[me];i++)
      for (j=ys[me];j<=ye[me];j++) {
         ldiff = x0[i][j] - x[i][j];
         *diff += ldiff*ldiff;
         x0[i][j] = x[i][j];
      }
}

/**************************************************************************/
/*                                                                        */
/* This subroutine sets up the initial temperatures on borders and inside */
/*                                                                        */
/**************************************************************************/

void initValues(double** x0, int size_total_x, int size_total_y, double temp1_init, double temp2_init) {

   /* Index variables */
   int i, j;

   /* Setup temp1_init on borders */
   for (i=0;i<=size_total_x-1;i++) {
      x0[i][0] = temp1_init;
      x0[i][size_total_y-1] = temp1_init;
   }

   for (j=0;j<=size_total_y-1;j++) {
      x0[0][j] = temp1_init;
      x0[size_total_x-1][j] = temp1_init;
   }

   for (i=0;i<=size_total_x-2;i++) {
      x0[i][1] = temp1_init;
      x0[i][size_total_y-2] = temp1_init;
   }

   for (j=1;j<=size_total_y-2;j++) {
      x0[1][j] = temp1_init;
      x0[size_total_x-2][j] = temp1_init;
   }

   /* Setup temp2_init inside */
   for (i=2;i<=size_total_x-3;i++)
      for (j=2;j<=size_total_y-3;j++)
         x0[i][j] = temp2_init;
}

/************************************************************/
/*                                                          */
/* This subroutine computes the coordinates xs, xe, ys, ye, */
/* for each cell on the grid, respecting processes topology */
/*                                                          */
/************************************************************/

void processToMap(int *xs, int *ys, int *xe, int *ye, int xcell, int ycell, int x_domains, int y_domains) {

   /* Index variables */
   int i, j;

   /* Computation of starting ys,ye on (Ox) standard axis
      for the first column of global domain,
      Convention x(i,j) with i row and j column */
   for (i=0;i<x_domains;i++) {
      ys[i] = 2;
      /* Here, ye(0:(x_domains-1)) = 2+ycell-1 */
      ye[i] = ys[i]+ycell-1;
   }

   /* Computation of ys,ye on (Ox) standard axis
      for all other cells of global domain */
   for (i=1;i<y_domains;i++)
      for (j=0;j<x_domains;j++) {
         ys[i*x_domains+j] = ys[(i-1)*x_domains+j]+ycell+2;
         ye[i*x_domains+j] = ys[i*x_domains+j]+ycell-1;
      }

   /* Computation of starting xs,xe on (Oy) standard axis
      for the first row of global domain,
      Convention x(i,j) with i row and j column */
   for (i=0;i<y_domains;i++) {
      xs[i*x_domains] = 2;
      /* Here, xe(i*x_domains) = 2+xcell-1 */
      xe[i*x_domains] = xs[i*x_domains]+xcell-1;
   }

   /* Computation of xs,xe on (Oy) standard axis
      for all other cells of global domain */
   for (i=1;i<=y_domains;i++)
      for (j=1;j<x_domains;j++) {
         xs[(i-1)*x_domains+j] = xs[(i-1)*x_domains+(j-1)]+xcell+2;
         xe[(i-1)*x_domains+j] = xs[(i-1)*x_domains+j]+xcell-1;
      }
}
