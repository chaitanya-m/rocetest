#!/bin/bash

OFED_VERSION = "4.5.1.0.1.0"
DIST_ARCH = "ubuntu18.04-x86_64"

wget http://www.mellanox.com/downloads/ofed/MLNX_OFED-$OFED_VERSION/MLNX_OFED_LINUX-$OFED_VERSION-$DIST_ARCH.tgz .

wget http://www.mellanox.com/downloads/ofed/MLNX_OFED-$OFED_VERSION/MLNX_OFED_LINUX-$OFED_VERSION-$DIST_ARCH.tgz .

tar -xvf MLNX_OFED_LINUX-$OFED_VERSION-$DIST_ARCH.tgz

cd MLNX_OFED_LINUX-$OFED_VERSION-$DIST_ARCH/

./mlnxofedinstall 

/etc/init.d/openibd restart


