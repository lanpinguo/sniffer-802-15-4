
ifeq ($(CHIP_NAME),STM32F4xx)
CHIP_DIR = ST
FIRMWARE_DIR = STM32F4xx_StdPeriph_Driver
else
CHIP_DIR = GD
FIRMWARE_DIR = GD32F4xx_standard_peripheral
endif

ifeq ($(CHIP_NAME),STM32F4xx)
CHIP_DIR = ST
FIRMWARE_DIR = STM32F4xx_StdPeriph_Driver
SRCS += $(PRJ_ROOT)/libraries/$(FIRMWARE_DIR)/src/misc.c \
       $(PRJ_ROOT)/libraries/$(FIRMWARE_DIR)/src/stm32f4xx_rcc.c \
       $(PRJ_ROOT)/libraries/$(FIRMWARE_DIR)/src/stm32f4xx_gpio.c \
       $(PRJ_ROOT)/libraries/$(FIRMWARE_DIR)/src/stm32f4xx_adc.c \
       $(PRJ_ROOT)/libraries/$(FIRMWARE_DIR)/src/stm32f4xx_exti.c \
       $(PRJ_ROOT)/libraries/$(FIRMWARE_DIR)/src/stm32f4xx_syscfg.c \
       $(PRJ_ROOT)/libraries/$(FIRMWARE_DIR)/src/stm32f4xx_usart.c \
       $(PRJ_ROOT)/libraries/$(FIRMWARE_DIR)/src/stm32f4xx_flash.c \
       $(PRJ_ROOT)/libraries/$(FIRMWARE_DIR)/src/stm32f4xx_iwdg.c \
       $(PRJ_ROOT)/libraries/$(FIRMWARE_DIR)/src/stm32f4xx_spi.c 
else
CHIP_DIR = GD
FIRMWARE_DIR = GD32F4xx_standard_peripheral
SRCS += $(PRJ_ROOT)/libraries/$(FIRMWARE_DIR)/Source/gd32f4xx_misc.c \
       $(PRJ_ROOT)/libraries/$(FIRMWARE_DIR)/Source/gd32f4xx_rcu.c \
       $(PRJ_ROOT)/libraries/$(FIRMWARE_DIR)/Source/gd32f4xx_gpio.c \
       $(PRJ_ROOT)/libraries/$(FIRMWARE_DIR)/Source/gd32f4xx_exti.c \
       $(PRJ_ROOT)/libraries/$(FIRMWARE_DIR)/Source/gd32f4xx_syscfg.c \
       $(PRJ_ROOT)/libraries/$(FIRMWARE_DIR)/Source/gd32f4xx_usart.c \
       $(PRJ_ROOT)/libraries/$(FIRMWARE_DIR)/Source/gd32f4xx_fmc.c \
       $(PRJ_ROOT)/libraries/$(FIRMWARE_DIR)/Source/gd32f4xx_fwdgt.c \
       $(PRJ_ROOT)/libraries/$(FIRMWARE_DIR)/Source/gd32f4xx_spi.c 
endif




