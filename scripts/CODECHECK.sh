make noecho CHECK="~/smatch/smatch --full-path" CC="~/smatch/cgcc" CFLAGS="-ffreestanding -O3 -Wall -Wextra -Iinclude -std=gnu99 -fno-omit-frame-pointer" > ../CODECHECK.txt 2>&1
