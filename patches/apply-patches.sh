#!/bin/bash
echo "===== Patching newlib now ====="
echo "Removing previous versions of newlib..."
rm -rf newlib/
echo "Extracting newlib..."; tar -xzf newlib.tar.gz; echo "Patching newlib...";
(cd newlib && patch -p 1 < ../newlib.patch)
