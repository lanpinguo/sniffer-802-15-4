
ifeq ($(CHIP_NAME),STM32F4xx)
CHIP_DIR = ST
FIRMWARE_DIR = STM32F4xx_StdPeriph_Driver
else
CHIP_DIR = GD
FIRMWARE_DIR = GD32F4xx_standard_peripheral
endif


INCLUDE_DIR += 	-I$(PRJ_ROOT)/src/HARDWARE/SPI \
				-I$(PRJ_ROOT)/src/HARDWARE/USART \


SRCS += $(PRJ_ROOT)/src/HARDWARE/LED/led.c \
		$(PRJ_ROOT)/src/HARDWARE/SPI/spi.c \
		$(PRJ_ROOT)/src/HARDWARE/USART/usart.c 







