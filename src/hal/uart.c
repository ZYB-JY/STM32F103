#include "hal/uart.h"
#include <stdint.h>

/* hal/uart.c - STM32F1 USART1 最小驱动实现（PA9 TX / PA10 RX）
   说明：基于 STM32F1 的寄存器地址，示例仅用于教学与移植。
   假设 PCLK2 = 72MHz，默认波特率通过 UART_Init 计算设置。
*/

#define RCC_BASE       0x40021000U
#define RCC_APB2ENR    (*(volatile uint32_t *)(RCC_BASE + 0x18))

#define GPIOA_BASE     0x40010800U
#define GPIOA_CRL      (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_CRH      (*(volatile uint32_t *)(GPIOA_BASE + 0x04))

#define USART1_BASE    0x40013800U
#define USART1_SR      (*(volatile uint32_t *)(USART1_BASE + 0x00))
#define USART1_DR      (*(volatile uint32_t *)(USART1_BASE + 0x04))
#define USART1_BRR     (*(volatile uint32_t *)(USART1_BASE + 0x08))
#define USART1_CR1     (*(volatile uint32_t *)(USART1_BASE + 0x0C))
#define USART1_CR2     (*(volatile uint32_t *)(USART1_BASE + 0x10))
#define USART1_CR3     (*(volatile uint32_t *)(USART1_BASE + 0x14))

/* RCC APB2ENR bits */
#define RCC_APB2ENR_AFIOEN   (1U << 0)
#define RCC_APB2ENR_IOPAEN   (1U << 2)
#define RCC_APB2ENR_USART1EN (1U << 14)

/* USART SR bits */
#define USART_SR_TXE (1U << 7)
#define USART_SR_TC  (1U << 6)
#define USART_SR_RXNE (1U << 5)

static void short_delay(void)
{
    for (volatile int i = 0; i < 1000; ++i) { __asm__ volatile ("nop"); }
}

static void gpioa_config_pa9_pa10(void)
{
    uint32_t crh = GPIOA_CRH;
    /* PA9 (TX) -> CRH bits [7:4] set to AF push-pull, max speed */
    crh &= ~(0xF << 4);
    crh |= (0xB << 4); /* 0b1011: MODE=11(50MHz) CNF=10(AF PP) */
    /* PA10 (RX) -> CRH bits [11:8] set to floating input */
    crh &= ~(0xF << 8);
    crh |= (0x4 << 8); /* 0b0100: MODE=00 CNF=01 (floating) */
    GPIOA_CRH = crh;
}

void UART_Init(uint32_t baudRate)
{
    /* 1) 使能 AFIO、GPIOA、USART1 时钟 */
    RCC_APB2ENR |= RCC_APB2ENR_AFIOEN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_USART1EN;
    short_delay();

    /* 2) 配置 GPIOA PA9/PA10 */
    gpioa_config_pa9_pa10();

    /* 3) 配置波特率：USARTDIV = PCLK2 / baudRate
       假定 PCLK2 = 72 MHz；BRR 使用整数简化（更精确实现需分频器计算） */
    uint32_t usartdiv = 72000000U / baudRate;
    USART1_BRR = usartdiv;

    /* 4) 使能发送与接收并打开 USART (UE bit) */
    USART1_CR1 = (1U << 13) | /* UE */
                 (1U << 3)  | /* TE */
                 (1U << 2);   /* RE */
}

void UART_Send(uint8_t data)
{
    while (!(USART1_SR & USART_SR_TXE)) { ; }
    USART1_DR = (uint32_t)data;
    while (!(USART1_SR & USART_SR_TC)) { ; }
}

uint8_t UART_Receive(void)
{
    while (!(USART1_SR & USART_SR_RXNE)) { ; }
    return (uint8_t)(USART1_DR & 0xFF);
}

void UART_SendString(const char *s)
{
    while (*s) {
        if (*s == '\n') {
            UART_Send('\r');
        }
        UART_Send((uint8_t)*s++);
    }
}