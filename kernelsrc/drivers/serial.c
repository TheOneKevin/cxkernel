/*
 * Code for the serial port stuff
 */

#include "system/kprintf.h"
#include "drivers/serial.h"
#include "arch/x86/pit.h"

#define UART_BAUD_RATE 115200

signed int timeout = -1;

void init_serial(uint16_t port, uint32_t baud, uint8_t data_bits, uint8_t stop, uint8_t parity)
{
    uint16_t divisor = UART_BAUD_RATE / baud;
    uint8_t line = data_bits & stop & parity;

    uint8_t hi = divisor >> 4;
    uint8_t lo = divisor & 0x0F;
    outb(port + 1, 0x00);    // Disable all interrupts
    outb(port + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(port + 0, lo);      // Set divisor to 3 (lo byte) 38400 baud
    outb(port + 1, hi);      //                  (hi byte)
    outb(port + 3, line);    // 8 bits, no parity, one stop bit
    outb(port + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outb(port + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

bool serialRecieved(uint16_t port)
{
    return inb(port + 5) & 1;
}

char readSerial(uint16_t port)
{
    if(timeout >= 0)
    {
        int _time = 0;
        while((!serialRecieved(port)) && (_time <= timeout))
        {
            delay(1);
            _time++;
        }

        if(_time > timeout)
            return 0;
    }
    else
        while(!serialRecieved(port));

    return inb(port);
}

char readSerialAsync(uint16_t port)
{
    return inb(port);
}

bool isTransmitEmpty(uint16_t port)
{
    return inb(port + 5) & 0x20;
}

void writeSerial(uint16_t port, char a)
{
    if(timeout >= 0)
    {
        int _time = 0;
        while((!isTransmitEmpty(port)) && (_time <= timeout))
        {
            delay(1);
            _time++;
        }

        if(_time > timeout)
            return;
    }
    else
        while(!isTransmitEmpty(port));
    outb(port, a);
}

void writeSerialString(uint16_t port, char* aStr)
{
    for (uint32_t i = 0; i < strlen(aStr); ++ i)
		writeSerial(port, aStr[i]);
}

void setTimeout(int newTime)
{
    timeout = newTime;
}
