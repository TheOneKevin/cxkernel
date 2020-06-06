## Toolchain Troubles for VSCode Users
---
**Which VSCode extensions should I have installed?**
- [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
- [CMake Test Explorer](https://marketplace.visualstudio.com/items?itemName=fredericbonnet.cmake-test-adapter)
- [psioniq File Header](https://marketplace.visualstudio.com/items?itemName=psioniq.psi-header)

**What about `i686-elf-gcc`?**  

VSC should automatically add the binary to your terminal `PATH` variable. If it doesn't work, well it should work, reload restart then file an issue.

**A note on CMake Tests**  

You need the latest version of CMake. Since `apt` can't be trusted to download the latest version on 18.04, the latest `.sh` installed from [here](https://cmake.org/download/) for Linux (you'll need something > 3.14). Then:  

`sudo sh cmake-3.17.3-Linux-x86_64.sh --prefix=/opt/cmake`  

Make the symlinks:
- `sudo ln -s /opt/cmake/bin/ctest /usr/local/bin/cmake`
- `sudo ln -s /opt/cmake/bin/ctest /usr/local/bin/ctest`

Check if `ctest --version` is about 3.14.

**How do I run tests?**
- Configure `Unit Test` for tests to show in the Test Explorer
- You can debug the Unity unit tests by passing in an `argc` and `argv` where `argv[1]` represents the test number you want to run.
