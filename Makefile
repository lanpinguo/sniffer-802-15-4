PROJ_NAME=main
PRJ_ROOT = $(realpath .)

ifeq ($(CPU),GD32)
CHIP_NAME = GD32F4xx
else ifeq ($(CPU),STM32)
CHIP_NAME = STM32F4xx
else
CHIP_NAME = STM32F4xx
endif

PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

OBJCOPY=arm-none-eabi-objcopy

OBJDIR = build

CFLAGS  = -g -Wall -Wno-missing-braces -std=c99
CFLAGS += -mthumb -mcpu=cortex-m4
#CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
#CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS += -mfloat-abi=hard -mfpu=auto
#CFLAGS += -mfloat-abi=soft 
#CFLAGS += --specs=nosys.specs
# TODO: hard float was causing an exception; see what's up.

ifeq ($(CHIP_NAME),STM32F4xx)
CFLAGS += -DSTM32F40_41xxx -DUSE_STDPERIPH_DRIVER
LDFLAGS = -lm -lc -lgcc -Wl,-Map,$(OBJDIR)/$(PROJ_NAME).map -g -Tstm32f4_flash.ld
LDFLAGS += --specs=nosys.specs
else
CFLAGS += -DGD32F40_41xxx -DGD32F407  -DUSE_STDPERIPH_DRIVER 
LDFLAGS = -lm -lc -lgcc -Wl,-Map,$(OBJDIR)/$(PROJ_NAME).map -g -Tgd32f4_flash.ld
LDFLAGS += --specs=nosys.specs
endif

APP_DIR = APP

ifeq ($(CHIP_NAME),STM32F4xx)
CHIP_DIR = ST
CPU_DIR = CPU/STM32
INCLUDE_DIR = 	-Isrc \
				-Ilibraries/STM32F4xx_StdPeriph_Driver/inc \
				-Ilibraries/CMSIS/ST/STM32F4xx/Include \
				-Ilibraries/CMSIS/Include \
				-Isrc/$(CPU_DIR) \
				-Isrc/SYSTEM/delay \
				-Isrc/SYSTEM/sys \
				-Isrc/HARDWARE/KEY \
				-Isrc/HARDWARE/LED
SRCS = 	src/SYSTEM/delay/delay.c \
		src/$(CPU_DIR)/system_stm32f4xx.c \
		src/$(CPU_DIR)/stm32f4xx_it.c \
		src/$(CPU_DIR)/startup_stm32f4xx.s \
		src/$(APP_DIR)/main.c \
		src/$(APP_DIR)/startup_config.c \
		src/$(APP_DIR)/command.c \
		src/$(APP_DIR)/shell.c \
		src/$(APP_DIR)/sys_misc.c 
else
CHIP_DIR = GD
CPU_DIR = CPU/GD32
INCLUDE_DIR = 	-Isrc \
				-Ilibraries/GD32F4xx_standard_peripheral/Include/ \
				-Ilibraries/CMSIS/GD/GD32F4xx/Include \
				-Ilibraries/CMSIS/Include \
				-Isrc/CPU/GD32 \
				-Isrc/SYSTEM/delay \
				-Isrc/SYSTEM/sys \
				-Isrc/HARDWARE/KEY \
				-Isrc/HARDWARE/LED
SRCS = 	src/SYSTEM/delay/delay.c \
		src/$(CPU_DIR)/system_gd32f4xx.c \
		src/$(CPU_DIR)/gd32f4xx_it.c \
		src/$(CPU_DIR)/startup_gd32f4xx.s \
		src/$(APP_DIR)/main.c \
		src/$(APP_DIR)/startup_config.c \
		src/$(APP_DIR)/command.c \
		src/$(APP_DIR)/shell.c \
		src/$(APP_DIR)/sys_misc.c 
		
endif



include $(PRJ_ROOT)/src/HARDWARE/Makefile 
include $(PRJ_ROOT)/libraries/Makefile 


CFLAGS +=  $(INCLUDE_DIR)



OBJS := $(SRCS:.c=.o)
OBJS := $(OBJS:.s=.o)
OBJS := $(addprefix $(OBJDIR)/,$(OBJS))


all: $(OBJDIR)/$(PROJ_NAME).elf $(OBJDIR)/$(PROJ_NAME).hex $(OBJDIR)/$(PROJ_NAME).bin

debug:
	@echo SRCS: $(SRCS)


$(OBJDIR)/%.elf: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	$(SZ) $@

%.hex: %.elf
	$(OBJCOPY) -O ihex $^ $@

%.bin: %.elf
	$(OBJCOPY) -O binary  $^ $@

$(OBJDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: %.s
	$(CC) -c $(CFLAGS) -o $@ $^

$(OBJDIR):
	mkdir -p $@

clean:
	rm -f $(OBJDIR)/$(PROJ_NAME).elf
	rm -f $(OBJDIR)/$(PROJ_NAME).hex
	rm -f $(OBJDIR)/$(PROJ_NAME).bin
	rm -f $(OBJDIR)/$(PROJ_NAME).map
	find $(OBJDIR) -type f -name '*.o' -print0 | xargs -0 -r rm


program: $(OBJDIR)/$(PROJ_NAME).elf
	openocd -f program.cfg


# Dependdencies
$(OBJDIR)/$(PROJ_NAME).elf: $(OBJS) | $(OBJDIR)

