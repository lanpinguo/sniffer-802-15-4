STM32-F4/GD32F4 project based gcc


## Build
### To compile the project with STM32F4:
    make 
or
	make CPU=STM32

### To compile the project with GD32F4:
    make CPU=GD32
or for parallel compilation:
	make -j4

## Load
To load the program into the STM32F4:
	make program

Or,us st-flash to flash the target:
example:
sudo st-flash write main.bin 0x8000000

Debugging is accomplished with openocd, which supports the stm32f4discovery natively.
Run this to start openocd:
in windows:
	openocd-0.6.1.exe -f board/stm32f4discovery.cfg
in linux:
	openocd -f board/stm32f4discovery.cfg
	openocd -f board/gd32f407vgt6_npp_jlink.cfg

And then start GDB:
	arm-none-eabi-gdb build/main.elf

And in GDB:
	target remote :3333
	monitor halt
	load
	monitor reset init

Set some breakpoints, etc, and then run the program:
	continue




NOTE: On Windows, do not install libusb drivers for the ST-Link. Use ST's native drivers. Openocd works just fine with the native drivers.
NOTE: On Windows, run arm-none-eabi-gdb from a normal Command Prompt (not within cygwin), otherwise Ctrl+C won't work (it'll close gdb instead of sending SIGINT to the program being debugged).
