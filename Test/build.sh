#!/bin/sh

export PATH=/home/7e12/Documents/Projects/EmbeddedAndroid/CrossCompile/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf/bin:$PATH
arm-linux-gnueabihf-gcc Test.c -o Test.so
adb push Test.so /system/usr/trieunvt

exit