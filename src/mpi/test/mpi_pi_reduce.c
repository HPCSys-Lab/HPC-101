/**********************************************************************
 * FILE: mpi_pi_reduce.c
 * OTHER FILES: dboard.c
 * DESCRIPTION:  
 *   MPI pi Calculation Example - C Version 
 *   Collective Communication example:  
 *   This program calculates pi using a "dartboard" algorithm.  See
 *   Fox et al.(1988) Solving Problems on Concurrent Processors, vol.1
 *   page 207.  All processes contribute to the calculation, with the
 *   master averaging the values for pi. This version uses mpc_reduce to 
 *   collect results
 * AUTHOR: Blaise Barney. Adapted from Ros Leibensperger, Cornell Theory
 *   Center. Converted to MPI: George L. Gusciora, MHPCC (1/95) 
 * LAST REVISED: 04/13/05 
**********************************************************************/
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

void srandom (unsigned seed);
double dboard (int darts);
#define DARTS 50000     /* number of throws at dartboard */
#define ROUNDS 10       /* number of times "darts" is iterated */
#define MASTER 0        /* task ID of master task */

int main (int argc, char *argv[])
{
	double homepi,	/* value of pi calculated by current task */
	pisum,	        /* sum of tasks' pi values */
	pi,	        /* average of pi after "darts" is thrown */
	avepi;	        /* average pi value for all iterations */
	int taskid,	        /* task ID - also used as seed number */
	numtasks,       /* number of tasks */
	rc,             /* return code */
	i;
	MPI_Status status;

	/* Obtain number of tasks and task ID */
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
	printf ("MPI task %d has started...\n", taskid);
	if (taskid == MASTER) 
		printf ("Using %d tasks to compute pi (3.1415926535)\n",numtasks);

	/* Set seed for random number generator equal to task ID */
	srandom (taskid);

	avepi = 0;
	for (i = 0; i < ROUNDS; i++) {
		/* All tasks calculate pi using dartboard algorithm */
		homepi = dboard(DARTS);

   /* Use MPI_Reduce to sum values of homepi across all tasks 
    * Master will store the accumulated value in pisum 
    * - homepi is the send buffer
    * - pisum is the receive buffer (used by the receiving task only)
    * - the size of the message is sizeof(double)
    * - MASTER is the task that will receive the result of the reduction
    *   operation
    * - MPI_SUM is a pre-defined reduction function (double-precision
    *   floating-point vector addition).  Must be declared extern.
    * - MPI_COMM_WORLD is the group of tasks that will participate.
    */

		rc = MPI_Reduce(&homepi, &pisum, 1, MPI_DOUBLE, MPI_SUM, MASTER, MPI_COMM_WORLD);
		if (rc != MPI_SUCCESS)
			printf("%d: failure on mpc_reduce\n", taskid);

		/* Master computes average for this iteration and all iterations */
		if (taskid == MASTER) {
			pi = pisum/numtasks;
			avepi = ((avepi * i) + pi)/(i + 1); 
			printf("   After %8d throws, average value of pi = %10.8f\n", (DARTS * (i + 1)),avepi);
		}    
	} 

	MPI_Finalize();
	return 0;
}

