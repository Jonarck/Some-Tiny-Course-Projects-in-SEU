#ifndef __FLASH_H
#define __FLASH_H			    
#include "sys.h" 
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"

#define SET   1
#define RESET 0

#define SPI_FLASH_ANALOG  /*SPI_FLASH_HAL*/

// SPI_SCK
#define SCK_PERIPH           SYSCTL_PERIPH_GPIOH
#define SCK_BASE             GPIO_PORTH_BASE
#define SCK_PIN              GPIO_PIN_0

// SPI_CS
#define CS_PERIPH           SYSCTL_PERIPH_GPIOH
#define CS_BASE             GPIO_PORTH_BASE
#define CS_PIN              GPIO_PIN_1

// SPI_MISO
#define MISO_PERIPH           SYSCTL_PERIPH_GPIOH
#define MISO_BASE             GPIO_PORTH_BASE
#define MISO_PIN              GPIO_PIN_3

// SPI_MOSI
#define MOSI_PERIPH           SYSCTL_PERIPH_GPIOH
#define MOSI_BASE             GPIO_PORTH_BASE
#define MOSI_PIN              GPIO_PIN_2

// io ctrl
#define SPI_FLASH_SCK_S_1      ROM_GPIOPinWrite(SCK_BASE, SCK_PIN, SCK_PIN)
#define SPI_FLASH_SCK_S_0      ROM_GPIOPinWrite(SCK_BASE, SCK_PIN, 0)

#define SPI_FLASH_MISO_S_G     ROM_GPIOPinRead(MISO_BASE, MISO_PIN)

#define SPI_FLASH_NSS_S_1    	 ROM_GPIOPinWrite(CS_BASE, CS_PIN, CS_PIN)
#define SPI_FLASH_NSS_S_0      ROM_GPIOPinWrite(CS_BASE, CS_PIN, 0)
   
#define SPI_FLASH_MISO_S_1     ROM_GPIOPinWrite(MISO_BASE, MISO_PIN, MISO_PIN)
#define SPI_FLASH_MISO_S_0     ROM_GPIOPinWrite(MISO_BASE, MISO_PIN, 0)

#define SPI_FLASH_MOSI_S_1     ROM_GPIOPinWrite(MOSI_BASE, MOSI_PIN, MOSI_PIN)
#define SPI_FLASH_MOSI_S_0     ROM_GPIOPinWrite(MOSI_BASE, MOSI_PIN, 0)

void SPI_FLASH_Write_Enable(void);
void SPI_Flash_Init(void);
void SPI_FLASH_SectorErase(u32 SectorAddr);
void SPI_FLASH_BulkErase(void);
void SPI_FLASH_PageWrite(const u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_Flash_Write(const u8* pBuffer, u32 WriteAddr, u32 NumByteToWrite);
void SPI_Flash_Read(u8* pBuffer, u32 ReadAddr, u32 NumByteToRead);
u32 SPI_FLASH_ReadID(void);
u32 SPI_FLASH_ReadDeviceID(void);
void SPI_FLASH_StartReadSequence(u32 ReadAddr);
void SPI_Flash_PowerDown(void);
void SPI_Flash_WAKEUP(void);

#ifdef SPI_FLASH_ANALOG
void SPI_FLASH_SendByte(u8 byte);
#else
u8 SPI_FLASH_SendByte(u8 byte);
#endif

u8 SPI_FLASH_ReadByte(void);
u16 SPI_FLASH_SendHalfWord(u16 HalfWord);
void SPI_FLASH_Write_Enable(void);
void SPI_FLASH_WaitForWriteEnd(void);


/*	  
//W25Xϵ��/Qϵ��оƬ�б�	   
//W25Q80 ID  0XEF13
//W25Q16 ID  0XEF14
//W25Q32 ID  0XEF15
//W25Q32 ID  0XEF16	
#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16

extern u16 SPI_FLASH_TYPE;//��������ʹ�õ�flashоƬ�ͺ�

#define	SPI_FLASH_CS PBout(12)  //ѡ��FLASH	
				 
////////////////////////////////////////////////////////////////////////////
 
//ָ���
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 

void SPI_Flash_Init(void);
u16  SPI_Flash_ReadID(void);  	    //��ȡFLASH ID
u8	 SPI_Flash_ReadSR(void);        //��ȡ״̬�Ĵ��� 
void SPI_FLASH_Write_SR(u8 sr);  	//д״̬�Ĵ���
void SPI_FLASH_Write_Enable(void);  //дʹ�� 
void SPI_FLASH_Write_Disable(void);	//д����
void SPI_FLASH_PageWrite(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void SPI_Flash_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void SPI_Flash_Read(u8* pBuffer,u32 ReadAddr,u32 NumByteToRead);   //��ȡflash
void SPI_Flash_Write(u8* pBuffer,u32 WriteAddr,u32 NumByteToWrite);//д��flash
void SPI_Flash_Erase_Chip(void);    	  //��Ƭ����
void SPI_Flash_Erase_Sector(u32 Dst_Addr);//��������
void SPI_Flash_Wait_Busy(void);           //�ȴ�����
void SPI_Flash_PowerDown(void);           //�������ģʽ
void SPI_Flash_WAKEUP(void);			  //����
*/
#endif
