void readParam(int* iconf, double* conf);

void processToMap(int* xs, int* xe, int* ys, int* ye, int xcell, int ycell, int x_domains, int y_domains);

void initValues(double** x0, int size_x, int size_y, double temp1, double temp2);

void updateBound(double** x0, int* neighBor, MPI_Comm comm, MPI_Datatype datatype, int me, int* xs, int* ys, int* xe, int* ye, int ycell);

void computeNext(double** x0, double** x, double dt, double hx, double hy, double* diff, int me, int* xs, int* ys, int* xe, int* ye, double k0);
