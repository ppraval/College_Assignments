#!/bin/bash

# SPDX-License-Identifier: BSD 3-Clause
# Copyright (c) 2020 The Regents of the University of California

# Install SPARC64 cross-compilation tools
echo "12345" | sudo DEBIAN_FRONTEND=noninteractive apt-get install -y \
    gcc-sparc64-linux-gnu \
    g++-sparc64-linux-gnu \
    gfortran-sparc64-linux-gnu

# Mount SPEC2017 ISO
mkdir -p /home/gem5/mnt
mount -o loop -t iso9660 /home/gem5/cpu2017-1.1.0.iso /home/gem5/mnt
mkdir -p /home/gem5/spec2017

# Install for SPARC64
echo "y" | /home/gem5/mnt/install.sh -d /home/gem5/spec2017 -u linux-sparc
cd /home/gem5/spec2017
. ./shrc

# Cleanup ISO
umount /home/gem5/mnt
rm -f /home/gem5/cpu2017-1.1.0.iso

# Create SPARC-specific config
cp config/Example-gcc-linux-x86.cfg config/myconfig.sparc64.cfg

# SPARC-specific modifications
sed -i "s//opt/rh/devtoolset-7/root/usr//usr/bin/sparc64-linux-gnu-/g" config/myconfig.sparc64.cfg
sed -i "s/-march=native/-mcpu=ultrasparc/g" config/myconfig.sparc64.cfg  # SPARC-specific CPU flag
sed -i "s/-mtune=native//g" config/myconfig.sparc64.cfg
sed -i "s/cc = gcc/cc = sparc64-linux-gnu-gcc/g" config/myconfig.sparc64.cfg
sed -i "s/CXX = g++/CXX = sparc64-linux-gnu-g++/g" config/myconfig.sparc64.cfg
sed -i "s/FC = gfortran/FC = sparc64-linux-gnu-gfortran/g" config/myconfig.sparc64.cfg

# Disable sysinfo as before
sed -i "s/command_add_redirect = 1/sysinfo_program =\ncommand_add_redirect = 1/g" config/myconfig.sparc64.cfg

# Build with SPARC toolchain
runcpu --config=myconfig.sparc64.cfg \
       --define build_ncpus=$(nproc) \
       --define bits=64 \
       --action=build all

# Cleanup logs
rm -f /home/gem5/spec2017/result/*