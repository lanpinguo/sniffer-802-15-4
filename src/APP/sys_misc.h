#ifndef _SYS_MISC_H
#define _SYS_MISC_H

void sys_gpio_init();
void run_led_set(uint32_t val);
void alarm_set(uint32_t val);
void run_led_toggle();
int test_bit_state_get();
void sys_wdg_init(void);
void feedIWdg(void);
void enableIWdg(void);
void IWdg_ConfigModeEnter();
void IWdg_NormalModeEnter();

#endif