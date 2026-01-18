#ifndef UART_H
#define UART_H

#include <stdint.h>

void UART_Init(uint32_t baudRate);
void UART_Send(uint8_t data);
uint8_t UART_Receive(void);

#endif // UART_H
