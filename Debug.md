
# Part I
### Step 1:
	STM32 DISCOVERY BOARD:
	openocd -f board/stm32f4discovery.cfg
	
	STM32 NPP BOARD:
	openocd -f board/stm32f407vgt6_npp.cfg
	
### Step 2:
on another terminal
	arm-none-eabi-gdb build/main.elf

And in GDB:
	target remote :3333
	monitor halt
	load
	monitor reset init

# Part II 
## 2.1
Set some breakpoints, etc, and then run the program:
(gdb) continue
	
Delete breakpoints:
(gdb) delete breakpoints [NUM]


## 2.2 GDB debug command
bt check function call stack:
(gdb) bt

# Part III
## 3.1
Map address into code：
addr2line -a 0x08000378 -e main.elf  


arm-none-eabi-objdump -s -D main.elf > main.asm