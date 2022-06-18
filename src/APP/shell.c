
#include "stdio.h"
#include <stdlib.h>
#include "string.h"
#include "usart.h"
#include "command.h"

#define SHELL_CMD_MAX_LEN   200

#define BACK_SPACE          0x8
#define SPACE               0x20

char command[SHELL_CMD_MAX_LEN];
int cmd_index = 0;

typedef int (*cmd_proc_func)(
    int argc,
    char *argv[],
    char * result);

#define COMMAND_MAX 	5
typedef struct CMD_MAP_S
{
	int				valid;
	char 			cmd_name[20];
	cmd_proc_func	proc_func;
}CMD_MAP_ENTRY_t;

static CMD_MAP_ENTRY_t cmd_map[COMMAND_MAX] = 
{
	  /*[0] = {1,"spi",0},*/
};


int str_split(char * str,char delim[],char* argv[],int argc_max)
{
	int argc = 0;
	char *ap;

	for (;((ap = strsep(&str, " \t")) != NULL) && (argc < argc_max); argc++)
	{       
		argv[argc] = ap;
	}
	return argc;
}

void shell_prompt()
{
    printf("npp#");
}

int process_cmd(char * cmd, int len)
{
	char *result;
	char* argv[20];
	int argc;
	CMD_MAP_ENTRY_t *pItem;
	int cmd_not_found = 1;

    //printf("\r\ninput cmd(%d): %s \r\n", cmd_index, command);
	
	argc = str_split((char*)cmd,(char*)" ",argv,20);
	result = (char *)calloc(200,1);	
	for(int i = 0 ; i < COMMAND_MAX; i++){
		pItem = &cmd_map[i];

		if(pItem->valid == 0){
			continue;
		}
		if(strcmp(argv[0],pItem->cmd_name) == 0){
			pItem->proc_func(argc,argv,result);
			cmd_not_found = 0;
		}
	}

	if(cmd_not_found){
		printf("command not found\r\n");
	}
    else{
        printf("\r\n%s", result);
    }

    /* prompt */
    shell_prompt();


    free(result);
    return 0;
}

int command_register(const char * name, void* p )
{
	CMD_MAP_ENTRY_t *pItem;
    cmd_proc_func func = p;
	
	for(int i = 0 ; i < COMMAND_MAX; i++){
		pItem = &cmd_map[i];
		if(pItem->valid == 0){
			strcpy(pItem->cmd_name,name);
			pItem->proc_func = func;
			pItem->valid = 1;
			return 0;
		}
	}

	return -1;

}




void process_input(void)
{
    uint8_t buf[2];
    size_t bytes;
    int cmd_valid = 0;
    int echo = 1;

    while(read(STDIN_FILENO, buf, 1) > 0){
        //printf(": %02x \r\n", buf[0]);
        echo = 1;
        if(buf[0] == '\r' || buf[0] == '\n'){
            printf("\r\n");
            cmd_valid = 1;
            break;
        }
        else{
            if(cmd_index < SHELL_CMD_MAX_LEN - 1){
                if(buf[0] == '\r' || buf[0] == '\n'){
                    continue;
                }

                if(BACK_SPACE == buf[0]){
                    if(cmd_index == 0){
                        echo = 0;
                    }
                    else{
                        command[--cmd_index] = '\0';
                    }
                }
                else{
                    command[cmd_index++] = buf[0];
                }
            }
            else{
                cmd_valid = 1;
                break;
            }
        }

        if(echo){
            /* Echo reply */
            putchar(buf[0]);

            if(BACK_SPACE == buf[0]){
                putchar(SPACE);
                putchar(BACK_SPACE);
            }

        }
    }
    
    if(cmd_valid && cmd_index > 0){
        //printf("input(%d): %s \r\n", cmd_index, command);
        process_cmd(command, cmd_index);
        cmd_index = 0;
        memset(command, 0, SHELL_CMD_MAX_LEN);
    }
    else{
        if(cmd_valid){
            shell_prompt();
        }
    }

}

int help_cmd(int argc, char* argv[],char* res)
{

	CMD_MAP_ENTRY_t *pItem;
	
    //printf("Usage: \r\nOptions:\r\n");
    
    printf("Supported cmd list: \r\n");
	for(int i = 0 ; i < COMMAND_MAX; i++){
		pItem = &cmd_map[i];
		if(pItem->valid != 0){
            printf("%s\r\n",pItem->cmd_name);
		}
	}

    return 0;
}


int shell_init(void)
{
    /* pre-installed command */
	command_register("help",help_cmd);

    /* customable commands */
    command_init();

    return 0;
}