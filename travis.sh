#!/bin/bash

TARGET=NUCLEO_F401RE

mbed-cli deploy
mbed compile -m ${TARGET} -t GCC_ARM

exit $?
