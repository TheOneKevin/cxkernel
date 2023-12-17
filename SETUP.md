Install the latest LLVM toolchain
```
sudo su
bash -c "$(wget -O - https://apt.llvm.org/llvm.sh)"
LLVM_VERSION=16 # Change this!
update-alternatives --install /usr/bin/clang clang /usr/bin/clang-$LLVM_VERSION 200
update-alternatives --install /usr/bin/cc cc /usr/bin/clang-$LLVM_VERSION 200
update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++-$LLVM_VERSION 200
update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-$LLVM_VERSION 200
update-alternatives --install /usr/bin/lld lld /usr/bin/lld-$LLVM_VERSION 200
update-alternatives --install /usr/bin/ld.lld ld.lld /usr/bin/ld.lld-$LLVM_VERSION 200
update-alternatives --install /usr/bin/ld ld /usr/bin/ld.lld 200
update-alternatives --install /usr/bin/clangd clangd /usr/bin/clangd-$LLVM_VERSION 200
update-alternatives --install /usr/bin/llvm-profdata llvm-profdata /usr/bin/llvm-profdata-$LLVM_VERSION 200
update-alternatives --install /usr/bin/llvm-cov llvm-cov /usr/bin/llvm-cov-$LLVM_VERSION 200
```
