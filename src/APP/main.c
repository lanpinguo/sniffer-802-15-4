#if 0
#include <stm32f4xx_conf.h>


int main (void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

	GPIOD->MODER = (1 << 26);

	while (1)
	{
		GPIOD->ODR ^= (1 << 13);
	}
}

#else

#include "stdio.h"
#include "delay.h"
#include "sys.h"
#include "sys_misc.h"
#include "usart.h"
#include "shell.h"
#include <string.h>

typedef enum{
    SYS_RUN_MODE_NORMAL = 0,
    SYS_RUN_MODE_TEST,
    SYS_RUN_MODE_ERROR
}SYS_RUN_MODE_e;

uint32_t run_mode = SYS_RUN_MODE_NORMAL;



#define true        1
#define false       0


int main(void)
{	
    
  	sys_gpio_init();

    run_led_set(0);

    usartInit();

    printf("sniffer 802.15.4  V1.0\r\n");
   

    //初始化延时函数
    delay_init(168);    


    //run_led_set(1);
	//alarm_set(1);

    sys_wdg_init();   


    shell_init();

    //enableIWdg();

   
    //main loop
    while(1)
    {

#if 1
        if(run_mode == SYS_RUN_MODE_NORMAL){
        }
        else if(run_mode == SYS_RUN_MODE_TEST){
        }
        else{

        }
#endif
        process_input();

        run_led_toggle();

        feedIWdg();
    }
}



#endif
