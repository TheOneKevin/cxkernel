#! /bin/bash

cd lib/openlibm
make clean
make TOOLPREFIX=i686-elf-
cp libopenlibm.a ../../build/obj/libopenlibm.a
