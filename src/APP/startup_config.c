#ifdef GD32F40_41xxx
#include "gd32f4xx_fmc.h"
#else
#include "stm32f4xx_flash.h"
#endif
#include <stdio.h>
#include "startup_config.h"
#include <string.h>


/* Base address of the Flash sectors */
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbytes */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbytes */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbytes */

#ifdef GD32F40_41xxx
#define FLASH_Sector_0 		CTL_SECTOR_NUMBER_0   /*!< sector 0   */
#define FLASH_Sector_1 		CTL_SECTOR_NUMBER_1   /*!< sector 1   */
#define FLASH_Sector_2 		CTL_SECTOR_NUMBER_2   /*!< sector 2   */
#define FLASH_Sector_3 		CTL_SECTOR_NUMBER_3   /*!< sector 3   */
#define FLASH_Sector_4 		CTL_SECTOR_NUMBER_4   /*!< sector 4   */
#define FLASH_Sector_5 		CTL_SECTOR_NUMBER_5   /*!< sector 5   */
#define FLASH_Sector_6 		CTL_SECTOR_NUMBER_6   /*!< sector 6   */
#define FLASH_Sector_7 		CTL_SECTOR_NUMBER_7   /*!< sector 7   */
#define FLASH_Sector_8 		CTL_SECTOR_NUMBER_8   /*!< sector 8   */
#define FLASH_Sector_9 		CTL_SECTOR_NUMBER_9   /*!< sector 9   */
#define FLASH_Sector_10		CTL_SECTOR_NUMBER_10  /*!< sector 10  */
#define FLASH_Sector_11		CTL_SECTOR_NUMBER_11  /*!< sector 11  */

#define FLASH_FLAG_EOP        FMC_STAT_END          /*!< FLASH End of Operation flag */
#define FLASH_FLAG_OPERR      FMC_STAT_OPERR        /*!< FLASH operation Error flag */
#define FLASH_FLAG_WRPERR     FMC_STAT_WPERR        /*!< FLASH Write protected error flag */
#define FLASH_FLAG_PGAERR     FMC_STAT_PGMERR       /*!< FLASH Programming Alignment error flag */
#define FLASH_FLAG_PGPERR     FMC_STAT_PGSERR       /*!< FLASH Programming Parallelism error flag  */
#define FLASH_FLAG_PGSERR     FMC_STAT_RDDERR       /*!< FLASH Programming Sequence error flag  */
#define FLASH_FLAG_BSY        FMC_STAT_BUSY         /*!< FLASH Busy flag */ 

#define FLASH_Unlock()                      fmc_unlock()
#define FLASH_Lock()                        fmc_lock()
#define FLASH_ClearFlag(FLASH_FLAG)         fmc_flag_clear(FLASH_FLAG)
#define FLASH_EraseSector(sector,v)         fmc_sector_erase(sector)
#define FLASH_ProgramWord(address, data)    fmc_word_program(address, data)       
#define FLASH_COMPLETE                      FMC_READY   

#endif


/**
  * @brief  Gets the sector of a given address
  * @param  None
  * @retval The sector of a given address
  */
uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;
  
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_Sector_0;  
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_Sector_1;  
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_Sector_2;  
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_Sector_3;  
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_Sector_4;  
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_Sector_5;  
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_Sector_6;  
  }
  else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
  {
    sector = FLASH_Sector_7;  
  }
  else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
  {
    sector = FLASH_Sector_8;  
  }
  else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
  {
    sector = FLASH_Sector_9;  
  }
  else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
  {
    sector = FLASH_Sector_10;  
  }
  else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11))*/
  {
    sector = FLASH_Sector_11;  
  }
  return sector;
}


__attribute__((__section__(".param"), used))
const CONFIG_PARAM_t startup_config = {
    .type = 0,
    .flag = (1 << 1)| 1,
    .alarmHighTh = 90,
    .alarmLowTh = 10,
    .fsr_High = 2.4,
    .fsr_Low = 0,
    .offset = 0
};


extern uint32_t CPU_SR_Save();
extern void CPU_SR_Restore(uint32_t);


void config_dump(CONFIG_PARAM_t *cfg)
{
    printf("\r\n");
    printf("Config from %p:\r\n", cfg);
    printf("%-15s: %d\r\n", "type", cfg->type);
    printf("%-15s: %d\r\n", "flag", cfg->flag);
    printf("%-15s: %d\r\n", "alarmHighTh", cfg->alarmHighTh);
    printf("%-15s: %d\r\n", "alarmLowTh", cfg->alarmLowTh);
    printf("%-15s: %f\r\n", "FSR_High", cfg->fsr_High);
    printf("%-15s: %f\r\n", "FSR_Low", cfg->fsr_Low);
    printf("%-15s: %f\r\n", "offset", cfg->offset);
    printf("%-15s: %f\r\n", "voltageAt4mA", cfg->voltageAt4mA);
    printf("%-15s: %f\r\n", "voltageAt20mA", cfg->voltageAt20mA);
    printf("\r\n");

}


void startup_config_update(CONFIG_PARAM_t *new_config)
{
    uint32_t cpu_sr;
    int startup_config_sector;
    uint32_t startAddress = 0;
    uint32_t totalBytes = 0;
    uint32_t *pVal;


    cpu_sr = CPU_SR_Save();

    /* Unlock the Flash to enable the flash control register access *************/ 
    FLASH_Unlock();

    /* Erase the user Flash area
    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

    /* Clear pending flags (if any) */  
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                    FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 

    /* Get the number of the start and end sectors */
    startup_config_sector = GetSector((uint32_t)(&startup_config));

    /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
        be done by word */ 
    if (FLASH_EraseSector(startup_config_sector, VoltageRange_3) != FLASH_COMPLETE)
    { 
        printf("Erase error\r\n");
    }

    startAddress = (uint32_t)(&startup_config) ;
    totalBytes = sizeof(CONFIG_PARAM_t);
    for(int i = 0; i < totalBytes; ) 
    {
        pVal = (uint32_t*)((uint8_t *)(new_config) + i);
        if (FLASH_ProgramWord(startAddress + i, *pVal) == FLASH_COMPLETE)
        {
            i += 4;
        }
        else
        { 
            /* Error occurred while writing data in Flash memory. 
                User can add here some code to deal with this error */
            printf("program error\r\n");
        }
    }


    /* Lock the Flash to disable the flash control register access (recommended
        to protect the FLASH memory against possible unwanted operation) *********/
    FLASH_Lock(); 

    CPU_SR_Restore(cpu_sr);

    printf("update startup_config done, total bytes %d\r\n", totalBytes);
    //config_dump(new_config);

}

void startup_background_update(char *newBackground)
{

}


void * startup_config_data_get()
{
    return &startup_config;
}


int startup_config_get(CONFIG_PARAM_t *cfg)
{

    if(cfg == NULL){
        return -1;
    }

    memcpy(cfg, &startup_config, sizeof(startup_config));

    return 0;

}


