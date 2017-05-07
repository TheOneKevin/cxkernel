#include "gdb/gdb.h"
#include "drivers/serial.h"
#include "system/kprintf.h"
#include "display/tdisplay.h"

extern uint32_t _gdb_internal_timeout;
void initDbg(uint32_t t)
{
    kprintf("GDB will hang if COM ports are disabled in an emulator!\n");
    kprintf("Initiatlizing the GNU Debugger on COM1 port...\n");
    _gdb_internal_timeout = t;
    kprintf("Timeouts set to %u ms\n", t);
    init_serial(COM1, 9600, UART_DATA_BITS_8, UART_STOP_BIT_1, UART_PARITY_NONE);
    set_debug_traps();

    console_setfg(COLOR_LIGHT_GREEN);
    kprintf("Waiting for GDB connection at port: %X\n", COM1);
    //__asm__("int $0x3"); // Breakpoint
    breakpoint();

    console_setfg(COLOR_WHITE);
    if(isInited())
        kprintf("Successfully connected to GDB at COM1 (%X)\n", COM1);
    else
        kprintf("Connection timed out at COM1\n");

    _gdb_internal_timeout = -1;
}
