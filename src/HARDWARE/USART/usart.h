
#ifndef _USART_H_
#define _USART_H_


#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2



void usartInit(void);
int read(int fd, char* ptr, int len) ;



#endif /* _USART_H_ */