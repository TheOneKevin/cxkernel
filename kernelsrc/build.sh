#!/bin/bash
#Build the operating system
echo Do not use this! Are you absolutly sure you know what you\'re doing?
read -p "Are you sure? " -n 1 -r
echo    # (optional) move to a new line
if [[ $REPLY =~ ^[Yy]$ ]]
then
    # do dangerous stuff
    echo Compiling kernel...
    make
    
    echo Cleaning previous mess...
    make clean
fi