# Copyright (c) 2019 The cxkernel Authors. All rights reserved.
# Use of this source code is governed by a MIT-style
# license that can be found in the LICENSE file or at
# https://opensource.org/licenses/MIT
# 
# @file   launch_qemu.sh
# @author TheCool1Kevin \<you@you.you\>
# @date   Created on December 30 2019, 10:16 PM

if grep -q Microsoft /proc/version; then
  (sleep 1; vncviewer.exe localhost:0) &
  qemu-system-i386 -S -gdb tcp::1234 -vnc :0 -boot d -cdrom build/bin/LiquiDOS.iso -m 128 -serial file:logs/COM1.log -monitor stdio
else
  qemu-system-i386 -S -gdb tcp::1234 -boot d -cdrom build/bin/LiquiDOS.iso -m 128 -serial file:logs/COM1.log -monitor stdio
fi
