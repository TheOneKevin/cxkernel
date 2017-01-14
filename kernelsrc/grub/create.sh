#!/bin/bash

echo Creating ramdisk right now...
rm -rf ramfs.tar
cd ramfs
tar -cvf ramfs.tar *.* **/*.*
mv -f ramfs.tar ../ramfs.tar
cd ../
echo Finished creating ramdisk!
