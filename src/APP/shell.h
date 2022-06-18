

#ifndef _SHELL_H_
#define _SHELL_H_



int shell_init(void);

void process_input(void);

int command_register(const char * name, void* p );


#endif /* _SHELL_H_ */