# cxkernel

This is the main repository of cxkernel, an experimental microkernel; the aim of which was to complete the operating system before I graduate high school.

The acronym was derived from "**C** is not \*ni**x**", which was shortened to **cx**kernel as CIN kernel wasn't as appealing.

## Building

Please use Linux to build as I will not be paying for your therapy.

**Run commands:**
1. Clone this repository
2. `mkdir logs` in root of the repository
3. `cd build` and run `. ./scripts/install_prereq.sh` as root
4. Export `build/toolchain/cross/x86_64/bin/` to `PATH` (maybe add to `~/.bashrc` too). Type `i686-elf-gcc` to confirm.
5. Install `cmake` (unless you're using VSCode then see the note below).
6. Install QEMU, if you're under WSL install the *native Windows* version. Add QEMU to your path.

We use the CMake build system. Simply configure `TARGET` and `BUILD_TYPE` (I also recommend `CUSTOM_FILENAME=ON`) to generate the Makefiles. Then run `make` and the artefacts should be generated under `build/bin/`.

**If you are using VSCode...**
1. See [this](docs/toolchain.md) first.
2. Run the above commands and open the repository root folder in VSCode
3. Run the task named "Configure" and follow instructions
4. Build!

## Old Branches
As the kernel has gone through many revisions and rewrites, old releases and sources are accessible through branches.
