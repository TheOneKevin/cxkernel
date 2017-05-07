/*
 * File:   serial.h
 * Author: Kevin
 *
 * Created on May 3, 2017, 7:28 AM
 */

#include "common.h"

 #ifndef SERIAL_H
 #define SERIAL_H

 #ifdef __cplusplus
 extern "C" {
 #endif

#define UART_DATA_BITS_5 0b00
#define UART_DATA_BITS_6 0b01
#define UART_DATA_BITS_7 0b10
#define UART_DATA_BITS_8 0b11

#define UART_STOP_BIT_1 0b000
#define UART_STOP_BIT_2 0b100

#define UART_PARITY_NONE  0b000000
#define UART_PARITY_ODD   0b001000
#define UART_PARITY_EVEN  0b011000
#define UART_PARITY_MARK  0b101000
#define UART_PARITY_SPACE 0b111000

void init_serial(uint16_t port, uint32_t baud, uint8_t data_bits, uint8_t stop, uint8_t parity);

bool serialRecieved(uint16_t port);
char readSerial(uint16_t port);
char readSerialAsync(uint16_t port);

bool isTransmitEmpty(uint16_t port);
void writeSerial(uint16_t port, char a);
void writeSerialString(uint16_t port, char* aStr);

void setTimeout(int newTime);

 #ifdef __cplusplus
 }
 #endif

 #endif /* SERIAL_H */
