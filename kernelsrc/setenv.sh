#!/bin/bash
#Script to set up environment
cd ../
echo Downloading toolchain...
git clone https://github.com/rm-hull/barebones-toolchain.git
cd barebones-toolchain
. ./setenv.sh
cd ../
echo You need to run setenv.sh everytime you start a shell, or add the PATH to the .bashrc file!