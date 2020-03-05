# cxkernel

This is the main repository of cxkernel, an experimental microkernel; the aim of which was to complete the operating system before I graduate high school.

The acronym was derived from "**C** is not \*ni**x**", which was shortened to **cx**kernel as CIN kernel wasn't as appealing.

## Building

Please use Linux to build as I will not be paying for your therapy.

Run commands:
1. Clone this repository
2. `mkdir logs` in root of the repository
3. `cd build` and run `. ./scripts/install_prereq.sh` as root
4. Export `build/toolchain/cross/x86_64/bin/` to `PATH`. Type `i686-elf-gcc` to confirm

We use the CMake build system. Simply configure `TARGET` and `BUILD_TYPE` (I also recommend `CUSTOM_FILENAME=ON`) to generate the Makefiles. Then run `make` and the artefacts should be generated under `build/bin/`.

If you are using VSCode...
1. Run the above commands and open the repository root folder in VSCode
2. Run the task named "Configure" and follow instructions
3. Build!

**Note**: If you are on WSL under VSCode, set the C/C++ configuration to `WSL` instead of `Linux` (extension name: `C/C++`, see lower-right corner when you open a C/C++ file).

## Old Branches
As the kernel has gone through many revisions and rewrites, old releases and sources are accessible through branches.
