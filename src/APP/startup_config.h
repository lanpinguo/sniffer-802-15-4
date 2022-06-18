
#ifndef _STARTUP_CONFIG_H_
#define _STARTUP_CONFIG_H_


typedef struct CONFIG_PARAM_s
{
    int     type;
    int     flag;
    int     alarmHighTh;
    int     alarmLowTh;
    float   fsr_Low;
    float   fsr_High;
    float   offset;
    float   voltageAt4mA;
    float   voltageAt20mA;
}CONFIG_PARAM_t;


void * startup_config_data_get();
void startup_config_update(CONFIG_PARAM_t *new_config);
void config_dump(CONFIG_PARAM_t *cfg);
int startup_config_get(CONFIG_PARAM_t *cfg);
void startup_background_update(char *newBackground);


#endif /* _STARTUP_CONFIG_H_ */


