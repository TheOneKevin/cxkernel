# LiquiDOS
---
[![Build Status](https://travis-ci.org/TheCool1Kevin/LiquiDOS.svg?branch=master)](https://travis-ci.org/TheCool1Kevin/LiquiDOS)
[![MIT Licence](https://badges.frapsoft.com/os/mit/mit.png?v=103)](https://opensource.org/licenses/mit-license.php)
[![Open Source Love](https://badges.frapsoft.com/os/v2/open-source.png?v=103)](https://opensource.org/)

Here is a source tree with comments (in square brackets). Kind of takes a while to be updated because its updated manually.

```
.                        [I AM ROOT]
|-- bin                  [Compiled binaries]
|-- grub                 [GRUB config]
|   `-- ramfs            [GRUB module contents]
|-- kernelsrc            [Kernel source code]
|   |-- arch             [Architecture specific code]
|   |   |-- i386         [i386/i686 platform code]
|   |   |   `-- asm      [x86 assembly files]
|   |   `-- raspi        [Raspberry pi 1 platform]
|   |-- core             [Kernel core/boot code]
|   |   `-- arch         [Boot time architecture specific code]
|   |       |-- i386     [Self explanatory]
|   |       |   `-- asm  [Assembly. Contains start.s]
|   |       `-- raspi    [Self explanatory]
|   |           `-- asm  [Also contains start.s + other shit]
|   `-- lib              [Kernel library. Has stuff like memset()]
|       `-- tui          [i386/i686 only. Platform dependent code.]
|   |-- include          [Self explanatory]
|   |   |-- arch
|   |   |   `-- i386
|   |   |-- lib
|   |   |   `-- tui
|   |   `-- sys          [System header files.]
|-- obj                  [Object files & linker files]
`-- tests                [Unit tests]
```
