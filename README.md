# STM32F103 裸机 C 项目（中文说明）

简介
- 本工程模板面向 STM32F103（Cortex‑M3），提供可直接编译并在常见工具链/调试器上烧录的最小裸机框架。
- 无 POSIX、pthread、libuv、libcurl 等依赖；使用精简合作式调度器与 UART HAL 示例。

默认假设
- MCU: STM32F103C8 (Flash 64KB, RAM 20KB)。如非该型号请修改 `linker.ld`。
- Toolchain: GNU Arm Embedded Toolchain (arm-none-eabi-)
- 串口: 使用 USART1（PA9 TX / PA10 RX）

快速使用
1. 安装工具链（示例）
   - GNU Arm Embedded Toolchain: https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm
2. 编译：
   make
   （或显式指定工具链前缀： `make TOOLCHAIN=arm-none-eabi`）
3. 烧录（example）：
   - 使用 st-flash（stlink）： `st-flash write build/firmware.bin 0x8000000`
   - 或使用 OpenOCD： 
     `openocd -f interface/stlink.cfg -f target/stm32f1x.cfg -c "program build/firmware.elf verify reset exit"`
4. 调试：使用你常用的调试器（ST‑Link/J‑Link）和 OpenOCD / IDE。

端口与移植提示
- 若使用其他串口/引脚或 MCU，请修改 `src/hal/uart.c` 与 `linker.ld` 中的地址与寄存器设置。
- SystemInit() 中应完成时钟/PLL 等低级初始化（本模板给出一个常见的 72MHz 配置示例）。

文件关键点
- `src/startup.s`：向量表与复位代码
- `linker.ld`：STM32F103C8 的 FLASH/RAM 区间（请按需修改）
- `src/hal/uart.c`：USART1（PA9/PA10）最小 HAL，包含时钟/引脚/波特率初始化（示例）
- `Makefile`：编译与两个烧录目标（st-flash / openocd）示例

许可
- MIT（请按需补全 LICENSE）

如需我把整个工程提交到你的 GitHub 仓库或生成 ZIP 下载包，请提供仓库信息或确认。若你使用不同 STM32 型号（如 F103RB），告诉我型号我会把 linker.ld 和 SystemInit 更精确地适配.
