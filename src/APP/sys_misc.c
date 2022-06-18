#ifdef GD32F40_41xxx
#include "gd32f4xx_gpio.h"
#include "gd32f4xx_rcu.h"
#include "gd32f4xx_exti.h"
#include "gd32f4xx_syscfg.h"
#include "gd32f4xx_fwdgt.h"
#else
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_iwdg.h"
#endif
#include <stdio.h>


#ifdef GD32F40_41xxx
#define RUN_LED_GPIO_PIN    GPIO_PIN_4
#define ALARM_GPIO_PIN      GPIO_PIN_0
#define TEST_GPIO_PIN       GPIO_PIN_1
#else
#define RUN_LED_GPIO_PIN    GPIO_Pin_4
#define ALARM_GPIO_PIN      GPIO_Pin_0
#define TEST_GPIO_PIN       GPIO_Pin_1
#endif


// run led indicate
void run_led_set(uint32_t val)
{
#ifdef GD32F40_41xxx
    gpio_bit_write(GPIOA, RUN_LED_GPIO_PIN, val ? SET : RESET);
#else
    GPIO_WriteBit(GPIOA, RUN_LED_GPIO_PIN, val ? Bit_RESET : Bit_SET);
#endif    
}


// toggle run led indicate
void run_led_toggle()
{
#ifdef GD32F40_41xxx
    gpio_bit_toggle(GPIOA, RUN_LED_GPIO_PIN);
#else
    GPIO_ToggleBits(GPIOA, RUN_LED_GPIO_PIN);
#endif    
}

void alarm_set(uint32_t val)
{
#ifdef GD32F40_41xxx
    gpio_bit_write(GPIOA, ALARM_GPIO_PIN, val ? SET : RESET);
#else
    GPIO_WriteBit(GPIOA, ALARM_GPIO_PIN, val ? Bit_SET : Bit_RESET);
#endif    
}

int test_bit_state_get()
{
#ifdef GD32F40_41xxx
     if(gpio_input_bit_get(GPIOA, TEST_GPIO_PIN) == SET){
         return 1;
     }
     else{
         return 0;
     }
#else
     if(GPIO_ReadInputDataBit(GPIOA, TEST_GPIO_PIN) == Bit_SET){
         return 1;
     }
     else{
         return 0;
     }
#endif     
}


void sys_gpio_init()
{
#ifdef GD32F40_41xxx
    /* enable the LEDs GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);

    /* configure RUN LED GPIO port */ 
    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, RUN_LED_GPIO_PIN);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, RUN_LED_GPIO_PIN);
    /* reset RUN LED GPIO pin */
    gpio_bit_reset(GPIOA, RUN_LED_GPIO_PIN);


    /* configure ALARM GPIO port */ 
    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, ALARM_GPIO_PIN);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, ALARM_GPIO_PIN);
    /* reset ALARM GPIO pin */
    gpio_bit_reset(GPIOA, ALARM_GPIO_PIN);

    /* configure TEST GPIO port */ 
    gpio_mode_set(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, TEST_GPIO_PIN);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, TEST_GPIO_PIN);
    /* reset TEST GPIO pin */
    gpio_bit_reset(GPIOA, TEST_GPIO_PIN);


#else
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA,GPIOE时钟

    GPIO_InitStructure.GPIO_Pin = TEST_GPIO_PIN; //TEST 对应引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = ALARM_GPIO_PIN | RUN_LED_GPIO_PIN; //ALARM对应引脚PA0，RUN led 对应PA4
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//100M
    GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
}


void IWdg_NormalModeEnter()
{
#ifdef GD32F40_41xxx
   /* confiure FWDGT counter clock: 32KHz(IRC32K) / 64 = 0.5 KHz */
    fwdgt_config(2*500,FWDGT_PSC_DIV64);
    /* reload FWDGT counter */
    fwdgt_counter_reload();
#else
    /* IWDG timeout equal to 250 ms (the timeout may varies due to LSI frequency
        dispersion) */
    /* Enable write access to IWDG_PR and IWDG_RLR registers */
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

    /* IWDG counter clock: LSI/256 */
    IWDG_SetPrescaler(IWDG_Prescaler_256);

    /* Set counter reload value to obtain 250ms IWDG TimeOut.
        Counter Reload Value = 250ms/IWDG counter clock period
                            = 250ms / (LSI/32)
                            = 0.25s / (LsiFreq/32)
                            = LsiFreq/(32 * 4)
                            = LsiFreq/128
    */
    IWDG_SetReload(25);
    /* Reload IWDG counter */
    IWDG_ReloadCounter();
#endif
}

void sys_wdg_init(void)
{
#ifdef GD32F40_41xxx
     /* check if the system has resumed from FWDGT reset */
    if (RESET != rcu_flag_get(RCU_FLAG_FWDGTRST))
#else
    /* Check if the system has resumed from IWDG reset */
    if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
#endif    
    {   
        /* IWDGRST flag set */
        /* Clear reset flags */
        printf("Device reset by IWDG\r\n");
        
#ifdef GD32F40_41xxx
        /* clear the FWDGT reset flag */
        rcu_all_reset_flag_clear();
#else
        RCC_ClearFlag();
#endif    
    }
    else
    {
        /* IWDGRST flag is not set */
        printf("Device reset by External\r\n");
    }

    IWdg_NormalModeEnter();
}


void IWdg_ConfigModeEnter()
{
#ifdef GD32F40_41xxx
   /* confiure FWDGT counter clock: 32KHz(IRC32K) / 128 = 0.25 KHz */
    fwdgt_config(2*500,FWDGT_PSC_DIV128);
    /* reload FWDGT counter */
    fwdgt_counter_reload();
#else
    /* IWDG timeout equal to 250 ms (the timeout may varies due to LSI frequency
        dispersion) */
    /* Enable write access to IWDG_PR and IWDG_RLR registers */
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

    /* IWDG counter clock: LSI/256 */
    IWDG_SetPrescaler(IWDG_Prescaler_256);

    /* Set counter reload value to obtain 250ms IWDG TimeOut.
        Counter Reload Value = 250ms/IWDG counter clock period
                            = 250ms / (LSI/32)
                            = 0.25s / (LsiFreq/32)
                            = LsiFreq/(32 * 4)
                            = LsiFreq/128
    */
    IWDG_SetReload(0xFFF);
    /* Reload IWDG counter */
    IWDG_ReloadCounter();
#endif
}


void enableIWdg(void)
{
#ifdef GD32F40_41xxx
    /* reload FWDGT counter */
    fwdgt_counter_reload();

    /* After 2 seconds to generate a reset */
    fwdgt_enable();
#else
    /* Reload IWDG counter */
    IWDG_ReloadCounter();

    /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
    IWDG_Enable();
#endif
}


void feedIWdg(void)
{
#ifdef GD32F40_41xxx
    /* reload FWDGT counter */
    fwdgt_counter_reload();
#else
    /* Reload IWDG counter */
    IWDG_ReloadCounter();  
#endif    
}
