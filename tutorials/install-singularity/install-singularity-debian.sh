#!/bin/bash

echo "Install dependencies ..."
apt update -y
apt install build-essential libssl-dev uuid-dev libgpgme11-dev squashfs-tools libseccomp-dev wge pkg-config git -y

echo "Install Go v1.16.3 ..."
wget https://dl.google.com/go/go1.16.3.linux-amd64.tar.gz
tar -C /usr/local -zxvf go1.16.3.linux-amd64.tar.gz
rm -f go1.16.3.linux-amd64.tar.gz

export GOROOT=/usr/local/go
export GOPATH=$GOROOT/work
export PATH=$PATH:$GOROOT/bin:$GOPATH/bin

echo "Verify Go version ..."
go version

echo "Install Singularity v3.3.0 ..."
git clone https://github.com/sylabs/singularity.git
cd singularity
git checkout v3.3.0    
./mconfig
make -C ./builddir
make -C ./builddir install
cd ..
rm -rf singularity
echo 1 > /proc/sys/kernel/unprivileged_userns_clone

echo "Verify Singularity version ..."
singularity version

# save it in file to make available efter reboot
GOPROFILE=/etc/profile.d/go.sh
touch ${GOPROFILE}
echo "#/bin/bash" >> ${GOPROFILE}
echo "export GOROOT=/usr/local/go" >> ${GOPROFILE}
echo "export GOPATH=$GOROOT/work" >> ${GOPROFILE}
echo "export PATH=$PATH:$GOROOT/bin:$GOPATH/bin" >> ${GOPROFILE}
echo "echo 1 > /proc/sys/kernel/unprivileged_userns_clone" >> ${GOPROFILE}
