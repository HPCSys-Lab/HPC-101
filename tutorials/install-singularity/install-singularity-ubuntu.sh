#!/bin/bash

echo 'deb http://download.opensuse.org/repositories/home:/afsmaira:/ufscar-hpc/xUbuntu_20.04/ /' \
| sudo tee /etc/apt/sources.list.d/home:afsmaira:ufscar-hpc.list

curl -fsSL \
https://download.opensuse.org/repositories/home:afsmaira:ufscar-hpc/xUbuntu_20.04/Release.key \
| gpg --dearmor \
| sudo tee /etc/apt/trusted.gpg.d/home:afsmaira:ufscar-hpc.gpg > /dev/null

sudo apt update -y

sudo apt install singularity-container -y

singularity version
