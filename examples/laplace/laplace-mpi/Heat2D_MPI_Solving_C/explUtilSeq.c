
/*************************************************************/
/*                                                           */
/* This subroutine computes next values within global domain */
/*                                                           */
/*************************************************************/

void computeNext(double** x0, double** x, int size_x,
          int size_y, double dt, double hx,
                 double hy, double* diff, double k0) {

   /* Index variables */
   int i, j;

   /* Factors for the stencil */
   double diagx, diagy, weightx, weighty;

   /* Local variable for computing difference */
   double ldiff;

   /* The stencil of the explicit operator for the heat equation
      on a regular rectangular grid using a five point finite difference
      scheme in space is :

      |                                    weightx * x[i-1][j]                                    |
      |                                                                                           |
      | weighty * x[i][j-1]   (diagx * weightx + diagy * weighty) * x[i][j]   weighty * x[i][j+1] |
      |                                                                                           |
      |                                    weightx * x[i+1][j]                                    | */

   diagx = -2.0 + hx*hx/(2*k0*dt);
   diagy = -2.0 + hy*hy/(2*k0*dt);
   weightx = k0*dt/(hx*hx);
   weighty = k0*dt/(hy*hy);

   /* Perform an explicit update on the points within the domain.
      Optimization : inner loop on columns index (second index) since
      C is row major */
   for (i=1;i<=size_x;i++)
      for (j=1;j<=size_y;j++)
         x[i][j] = weightx*(x0[i-1][j] + x0[i+1][j] + x0[i][j]*diagx)
                 + weighty*(x0[i][j-1] + x0[i][j+1] + x0[i][j]*diagy);

   /* Compute the difference into domain for convergence.
      Update the value x0(i,j).
      Optimization : inner loop on columns index (second index) since
      C is row major */
   *diff = 0.0;
   for (i=1;i<=size_x;i++)
      for (j=1;j<=size_y;j++) {
         ldiff = x0[i][j] - x[i][j];
         *diff += ldiff*ldiff;
         x0[i][j] = x[i][j];
      }
}

/*************************************************************************/
/*                                                                       */
/* This subroutine sets up the initial temperature on borders and inside */
/*                                                                       */
/*************************************************************************/

void initValues(double** x0, int x_dim, int y_dim, double temp1_init, double temp2_init) {

   /* Index variables */
   int i, j;

   /* Setup temp1_init on borders */
   for (i=0;i<=x_dim+1;i++) {
      x0[i][0] = temp1_init;
      x0[i][y_dim+1] = temp1_init;
   }

   for (j=0;j<=y_dim+1;j++) {
      x0[0][j] = temp1_init;
      x0[x_dim+1][j] = temp1_init;
   }

   /* Setup temp2_init inside */
   for (i=1;i<=x_dim;i++)
      for (j=1;j<=y_dim;j++)
         x0[i][j] = temp2_init;
}
