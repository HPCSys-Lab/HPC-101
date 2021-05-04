# Build Singularity

First, you need to write a singularity definition file. For detailed information on writing Singularity definition files, please see the [Container definition docs](https://sylabs.io/guides/3.0/user-guide/definition_files.html).

For this class, you can use this [example](https://github.com/HPCSys-Lab/HPC-101/blob/main/tutorials/build-singularity/container.def) as templete.

Once you have a definition file, you can create the container using the command bellow:

`singularity build -f container.sif container.def`

where:

- `container.sif` is the name of the container that will be created.

- `container.def` is the definition file.

After that, you can run the container with:

`singularity run container.sif`
