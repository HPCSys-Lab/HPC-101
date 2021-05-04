# Run a Job on UFSCar's cluster

To submit jobs to the cluster, you need to write a [job script](https://github.com/HPCSys-Lab/HPC-101/blob/main/tutorials/cluster-ufscar/job.sh), which is basically a shell script file with a header (resources and job configuration) and a body (commands of your job).

After that, you submit your job using the `sbatch` command:

`sbatch job.sh`

Make sure your application is running on a singularity container.

