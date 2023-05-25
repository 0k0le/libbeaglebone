#!/bin/bash

cd i2c-tools/
make CC=../../beaglebone-crosscompiler/arm-linux-gnueabihf-cross/bin/arm-linux-gnueabihf-gcc \
SYSROOT=../beaglebone-crosscompiler/arm-linux-gnueabihf-cross/
cd ..

make CXX=../beaglebone-crosscompiler/arm-linux-gnueabihf-cross/bin/arm-linux-gnueabihf-g++ \
AR=../beaglebone-crosscompiler/arm-linux-gnueabihf-cross/bin/arm-linux-gnueabihf-ar \
SYSROOT=../beaglebone-crosscompiler/arm-linux-gnueabihf-cross/
