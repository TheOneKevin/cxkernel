# LiquiDOS
[![Build Status](https://travis-ci.org/TheCool1James/LiquiDOS.svg?branch=master)](https://travis-ci.org/TheCool1James/LiquiDOS)
[![MIT Licence](https://badges.frapsoft.com/os/mit/mit.svg?v=103)](https://opensource.org/licenses/mit-license.php)
[![Open Source Love](https://badges.frapsoft.com/os/v2/open-source.svg?v=103)](https://github.com/ellerbrock/open-source-badges/)


A DOS-Style Operating System created from scratch. It uses GRUB to boot. Currently these are the only features implemented:  

1. Hardware interrupts (IRQs). Currently, only keyboard and PIT (timer) are supported. Added extendability for keymaps (if anyone wants to remap the keyboard, just look at the en_scan.h). Kprintf works too! GDT and IDTs. Exceptions are displayed on a pink error screen with a little Dalek ASCII art.  
2. Initrd and VFS (basic, may be buggy). Supports only tarfs for now.
3. Paging is revised and awesome
4. ACPI works! You can shutdown and reboot sucessfully. Working on handling a shutdown signal :smiley:
5. Added kernel heap + physical mm. Physical memory manager is revised.
6. Kernel boots into higher half. Working on remapping to higher 3.5 GiB. Currently at 3 GiB.

That's all folks.  

## TODO

1. Port ACPICA
2. Usermode, multitasking and whatnot...  
3. Continue editing and revising the kernel memory manager(s)
4. Add in-house debug (int 0x3) thing

I'm mainly aiming for the OS to be a "Lino Commando" thing (for now).  
Reference http://wiki.osdev.org/Lino_Commando
