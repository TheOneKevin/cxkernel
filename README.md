# LiquiDOS
A DOS-Style Operating System created from scratch. It uses GRUB to boot. Currently these are the only features implemented:  

1. Hardware interrupts (IRQs). Currently, only keyboard and PIT (timer) are supported. Added extendability for keymaps (if anyone wants to remap the keyboard, just look at the en_scan.h).  
2. GDT and IDTs. Exceptions are displayed on a pink error screen with a little Dalek ASCII art.  
3. Paging works. Not in optimal condition. Probably will be revisioned several times.
4. ACPI works! You can shutdown and reboot sucessfully. Working on handling a shutdown signal :smiley:
5. Primitive kmalloc from JMolloy's tutorials. Horribe code.

That's all folks.  

## TODO
1. Set up a kernel heap. Add more sophisticated kmalloc and stuff.
2. Add a kprint thing.
3. Make sure the ACPI shutdown signal works.
2. Set up a initrd (ramdisk) for GRUB to load all the drivers and such
3. Set up a filesystem driver. I. NEED. ONE. BADLY.
4. Usermode, multitasking and whatnot... But not until the FS is all up and happy.  

I'm mainly aiming for the OS to be a "Lino Commando" thing (for now).  
Reference http://wiki.osdev.org/Lino_Commando
