
#include "shell.h"
#include <stdio.h>
#include "startup_config.h"
#include "sys_misc.h"


int test_cmd(int argc, char* argv[], char* res)
{

    /* watchdog test*/
    while(1);
    
    return 0;

}



int info_cmd(int argc, char* argv[], char* res)
{

    return 0;

}



int config_cmd_init(void)
{

	command_register("info",info_cmd);

	command_register("test",test_cmd);


   return 0;
}




int command_init(void)
{
    config_cmd_init();
    return 0;
}