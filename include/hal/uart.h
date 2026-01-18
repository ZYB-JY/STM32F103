#ifndef HAL_UART_H
#define HAL_UART_H

#include <stdint.h>

/* hal/uart.h - STM32F1 USART1 (PA9 TX / PA10 RX) 最小 HAL 接口 */

void UART_Init(uint32_t baudRate);
void UART_Send(uint8_t data);
uint8_t UART_Receive(void);
void UART_SendString(const char *s);

#endif /* HAL_UART_H */
