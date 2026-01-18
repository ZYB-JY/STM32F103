/* startup.s - STM32F1 (Cortex-M3) 向量表与复位处理（简化版） */
    .syntax unified
    .cpu cortex-m3
    .thumb
    .section .isr_vector, "a", %progbits
    .align 2
    .word __stack_end
    .word Reset_Handler
    .word NMI_Handler
    .word HardFault_Handler
    .word MemManage_Handler
    .word BusFault_Handler
    .word UsageFault_Handler
    .word 0
    .word 0
    .word 0
    .word 0
    .word SVC_Handler
    .word DebugMon_Handler
    .word 0
    .word PendSV_Handler
    .word SysTick_Handler
    /* 设备中断向量（按需在此展开/增加） */
    .word WWDG_IRQHandler
    .word PVD_IRQHandler
    .word TAMPER_IRQHandler
    .word RTC_IRQHandler
    .word FLASH_IRQHandler
    .word RCC_IRQHandler
    .word EXTI0_IRQHandler
    .word EXTI1_IRQHandler
    .word EXTI2_IRQHandler
    .word EXTI3_IRQHandler
    .word EXTI4_IRQHandler
    .word DMA1_Channel1_IRQHandler
    /* ... 其余中断可按需要添加或在用户代码中实现弱符号覆盖 ... */

    .section .text.Reset_Handler
    .weak Reset_Handler
    .type Reset_Handler, %function
Reset_Handler:
    /* 复制 .data */
    ldr r0, =__data_start
    ldr r1, =__data_end
    ldr r2, =__etext
1:
    cmp r0, r1
    beq 2f
copy_loop:
    ldr r3, [r2], #4
    str r3, [r0], #4
    cmp r0, r1
    bne copy_loop
2:
    /* 清零 .bss */
    ldr r0, =__bss_start
    ldr r1, =__bss_end
    movs r2, #0
clear_bss:
    cmp r0, r1
    beq bss_done
    str r2, [r0], #4
    b clear_bss
bss_done:

    /* 调用系统初始化 */
    bl SystemInit
    bl main

hang:
    b hang

/* 异常默认处理（弱定义，用户可在 C 中重写这些函数）*/
    .weak NMI_Handler
NMI_Handler:
    b .

    .weak HardFault_Handler
HardFault_Handler:
    b .

    .weak MemManage_Handler
MemManage_Handler:
    b .

    .weak BusFault_Handler
BusFault_Handler:
    b .

    .weak UsageFault_Handler
UsageFault_Handler:
    b .

    .weak SVC_Handler
SVC_Handler:
    b .

    .weak DebugMon_Handler
DebugMon_Handler:
    b .

    .weak PendSV_Handler
PendSV_Handler:
    b .

    .weak SysTick_Handler
SysTick_Handler:
    b .

    /* Device IRQ weak defaults (示例几项) */
    .weak WWDG_IRQHandler
WWDG_IRQHandler: b .
    .weak PVD_IRQHandler
PVD_IRQHandler: b .
    .weak TAMPER_IRQHandler
TAMPER_IRQHandler: b .
    .weak RTC_IRQHandler
RTC_IRQHandler: b .
    .weak FLASH_IRQHandler
FLASH_IRQHandler: b .
    .weak RCC_IRQHandler
RCC_IRQHandler: b .
    .weak EXTI0_IRQHandler
EXTI0_IRQHandler: b .
    .weak EXTI1_IRQHandler
EXTI1_IRQHandler: b .
    .weak EXTI2_IRQHandler
EXTI2_IRQHandler: b .
    .weak EXTI3_IRQHandler
EXTI3_IRQHandler: b .
    .weak EXTI4_IRQHandler
EXTI4_IRQHandler: b .
    .weak DMA1_Channel1_IRQHandler
DMA1_Channel1_IRQHandler: b .
