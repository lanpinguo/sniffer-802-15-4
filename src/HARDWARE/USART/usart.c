
#include "stdio.h"
#ifdef GD32F40_41xxx
#include "gd32f4xx_libopt.h"
#else

#include "misc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_syscfg.h"

#endif

#include <_ansi.h>
#include <_syslist.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/times.h>
#include <limits.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>

#include "usart.h"

typedef enum{
    USART_RX_NORMAL = 0,
    USART_RX_FULL,
}USART_STATE_e;

static uint32_t status = USART_RX_NORMAL;
typedef struct __IO_BUFFER
{
    #define BUFFER_LEN  256
    int in;
    int out;
    char data[BUFFER_LEN];
} IO_BUFFER_t;


IO_BUFFER_t usart_recv;

void USART_PutChar(uint8_t ch)
{
#ifdef GD32F40_41xxx
    while (RESET == usart_flag_get(USART0, USART_FLAG_TBE));
    usart_data_transmit(USART0, (uint8_t)ch);
#else
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, (uint16_t) ch);
#endif
}



void retargetInit()
{

  /* Disable I/O buffering for STDOUT stream, so that
   * chars are sent out as soon as they are printed. */
  setvbuf(stdout, NULL, _IONBF, 0);
}

int _isatty(int fd) {
  if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
    return 1;

  errno = EBADF;
  return 0;
}

int _write(int fd, char* ptr, int len) 
{

  if (fd == STDOUT_FILENO || fd == STDERR_FILENO) {
    int DataIdx;

    for(DataIdx= 0; DataIdx< len; DataIdx++)
    {
        USART_PutChar(*ptr++);
    }
    return len;
    //return EIO;
  }
  errno = EBADF;
  return -1;
}


int bufferIn_Get()
{
    int validBufCnt;

    if(usart_recv.in > usart_recv.out){
        validBufCnt = BUFFER_LEN - usart_recv.in + usart_recv.out;
    }
    else if(usart_recv.in == usart_recv.out){
        validBufCnt = BUFFER_LEN;
    }
    else{
        validBufCnt = usart_recv.out - usart_recv.in ;
    }

    if(validBufCnt){
        return (usart_recv.in++) % BUFFER_LEN;
    }
    else{
        return -1;
    }
}

int bufferOut_Get()
{
    int validCharCnt;

    if(usart_recv.out > usart_recv.in){
        validCharCnt = BUFFER_LEN - usart_recv.out + usart_recv.in;
    }
    else if(usart_recv.in == usart_recv.out){
        validCharCnt = 0;
    }
    else{
        validCharCnt = usart_recv.in - usart_recv.out ;
    }


    if(validCharCnt){
        return (usart_recv.out++) % BUFFER_LEN;
    }
    else{
        return -1;
    }
}



int _close(int fd)
{
  if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
    return 0;

  errno = EBADF;
  return -1;
}

int _lseek(int fd, int ptr, int dir) 
{
  (void) fd;
  (void) ptr;
  (void) dir;

  errno = EBADF;
  return -1;
}

int _read(int fd, char* ptr, int len) 
{

  if (fd == STDIN_FILENO) {
    int index = 0;
    int i = 0;
    
    for(i = 0 ; i < len ; i++){
        index = bufferOut_Get();
        if(index < 0){
            break;
        }
        else{
            *ptr++ = usart_recv.data[index];
        }
    }

    return i;
    //return EIO;
  }
  errno = EBADF;
  return -1;
}

int _fstat(int fd, struct stat* st) 
{
  if (fd >= STDIN_FILENO && fd <= STDERR_FILENO) {
    st->st_mode = S_IFCHR;
    return 0;
  }

  errno = EBADF;
  return 0;
}


#ifdef GD32F40_41xxx
/**
  * @brief  This function handles USART1 interrupt request.
  * @param  None
  * @retval None
  */
void USART0_IRQHandler(void)
{
    int index = bufferIn_Get();

    if(index < 0){
        status = USART_RX_FULL;
    }
    else{
        usart_recv.data[index] = usart_data_receive(USART0);
        status = USART_RX_NORMAL;
    }
}


#else
/**
  * @brief  This function handles USART1 interrupt request.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{
    int index = bufferIn_Get();

    if(index < 0){
        status = USART_RX_FULL;
    }
    else{
        usart_recv.data[index] = USART_ReceiveData(USART1);
        status = USART_RX_NORMAL;
    }
}
#endif

int read(int fd, char* ptr, int len) 
{
  return _read(fd, ptr, len);
}

void usartInit(void) 
{
#ifdef GD32F40_41xxx
    /* enable GPIO clock */
    rcu_periph_clock_enable( RCU_GPIOA );

    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART0 );

    /* connect port to USARTx_Tx */
    gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_9);

    /* connect port to USARTx_Rx */
    gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_10);

    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP,GPIO_PIN_9);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_9);

    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP,GPIO_PIN_10);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_10);

    /* USART configure */
    usart_deinit(USART0);
    usart_baudrate_set(USART0,115200U);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);

    usart_interrupt_enable(USART0, USART_INT_RBNE);

    nvic_irq_enable(USART0_IRQn, 0, 1);
    
    usart_enable(USART0);
   
#else
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef   NVIC_InitStructure;


   /* For output on GPIOA */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    /* Output pins configuration, change M and N index! */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; // Push - pull
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl =  USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);


    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    /* Enable  Interrupt to the lowest priority */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART1, ENABLE);

#endif
    retargetInit();
}


