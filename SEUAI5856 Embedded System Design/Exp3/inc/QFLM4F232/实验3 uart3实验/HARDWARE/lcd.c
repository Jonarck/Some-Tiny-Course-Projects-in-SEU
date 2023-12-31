#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "grlib/grlib.h"

#include "lcd.h"
#include "font.h"
#include "sys.h"

//LCD�Ļ�����ɫ�ͱ���ɫ	   
unsigned int POINT_COLOR=Red;	//������ɫ
unsigned int BACK_COLOR= Black;  //����ɫ 
unsigned char abx=0;

//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev;

/************************************************************************************
** Function Name : WriteData                                                       **
** Description   : Writes a data word to the ILI932x.                              **
** Arguments     :                                                                 **
** Return        :                                                                 **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2011-10-20                                                      **
** Notes         :                                                                 **
************************************************************************************/
void lcd_write_data(unsigned short usData)
{
	LCD_SET_RS(LCD_RS_PIN);

	HWREG(LCD_DATAH_BASE + GPIO_O_DATA + (LCD_DATAH_PINS << 2)) = (usData >> 8);
	HWREG(LCD_DATAL_BASE + GPIO_O_DATA + (LCD_DATAL_PINS << 2)) = (usData);   

	LCD_SET_WR(0);
	LCD_SET_WR(LCD_WR_PIN);
}

/************************************************************************************
** Function Name : ReadData                                                        **
** Description   : Reads a data word from the ILI932x.                             **
** Arguments     :                                                                 **
** Return        :                                                                 **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2011-10-20                                                      **
** Notes         :                                                                 **
************************************************************************************/
static unsigned short lcd_read_data(void)
{
    unsigned short usData;

    // Make the data bus be an input.
	ROM_GPIODirModeSet(LCD_DATAH_BASE, LCD_DATAH_PINS, GPIO_DIR_MODE_IN);
	ROM_GPIODirModeSet(LCD_DATAL_BASE, LCD_DATAL_PINS, GPIO_DIR_MODE_IN);



    // Assert the read signal.  This is done multiple times (though only the
    // first has an affect on the pin) in order to meet the timing requirements
    // of the ILI932x.
	LCD_SET_RD(0);
	LCD_SET_RD(0);
	LCD_SET_RD(0);

    // Read the data from the data bus.
    //usData = LCD_READ_DATA_16BIT();
	usData = HWREG(LCD_DATAH_BASE + GPIO_O_DATA + (0xff << 2)) << 8;
    usData |= HWREG(LCD_DATAL_BASE + GPIO_O_DATA + (0xff << 2));

    // Deassert the read signal.
    LCD_SET_RD(LCD_RD_PIN)
    LCD_SET_RD(LCD_RD_PIN)
 
    // Change the data bus back to an output.
	ROM_GPIODirModeSet(LCD_DATAH_BASE, LCD_DATAH_PINS, GPIO_DIR_MODE_OUT);
	ROM_GPIODirModeSet(LCD_DATAL_BASE, LCD_DATAL_PINS, GPIO_DIR_MODE_OUT);
    


    // Return the data that was read.
    return(usData);
}

/************************************************************************************
** Function Name : WriteCommand                                                    **
** Description   : Writes a command to the ILI932x.                                **
** Arguments     :                                                                 **
** Return        :                                                                 **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2011-10-20                                                      **
** Notes         :                                                                 **
************************************************************************************/
void lcd_write_command(unsigned char ucData)
{
  // Set the RS signal low, indicating a command.
	LCD_SET_RS(0);

	// Write the command to the data bus.
	HWREG(LCD_DATAH_BASE + GPIO_O_DATA + (LCD_DATAH_PINS << 2)) = (ucData >> 8);   
	HWREG(LCD_DATAL_BASE + GPIO_O_DATA + (LCD_DATAL_PINS << 2)) = (ucData); 	

	LCD_SET_WR(0);
  LCD_SET_WR(LCD_WR_PIN);
}

/************************************************************************************
** Function Name : LCD_ReadRegister                                                **
** Description   : Read the value of a register from the ILI932x display controller**
** Arguments     :                                                                 **
** Return        :                                                                 **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2011-10-20                                                      **
** Notes         :                                                                 **
************************************************************************************/
__forceinline unsigned short LCD_ReadRegister(unsigned char ucIndex)
{
    lcd_write_command(ucIndex);
    return(lcd_read_data());
}

/************************************************************************************
** Function Name : WriteRegister                                                   **
** Description   : Write a particular controller register with a value             **
** Arguments     :                                                                 **
** Return        :                                                                 **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2011-10-20                                                      **
** Notes         :                                                                 **
************************************************************************************/
__forceinline void	LCD_WriteRegister(unsigned char ucIndex, unsigned short usValue)
{
    lcd_write_command(ucIndex);
    lcd_write_data(usValue);
}

/************************************************************************************
** Function Name : LCD_InitGpio                                                    **
** Description   :                                                                 **
** Arguments     :                                                                 **
** Return        :                                                                 **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2011-10-20                                                      **
** Notes         :                                                                 **
************************************************************************************/
static void lcd_init_gpio(void)
{
	// Enable the GPIO peripherals used to interface to the ILI932x.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
	  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	
    // Configure the pins that connect to the LCD as GPIO outputs.
    ROM_GPIOPinTypeGPIOOutput(LCD_DATAH_BASE, LCD_DATAH_PINS);
	  ROM_GPIOPinTypeGPIOOutput(LCD_DATAL_BASE, LCD_DATAL_PINS);
	  ROM_GPIOPinTypeGPIOOutput(GPIO_PORTJ_BASE, LCD_LEDK_PIN );					//����
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, LCD_RS_PIN | LCD_RD_PIN);
	  ROM_GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, LCD_CS_PIN | LCD_WR_PIN);
	  ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_6);							//reset
 
	  ROM_GPIOPinWrite(LCD_LEDK_BASE, LCD_LEDK_PIN, 0x00);   		    	// LCD_LED_K1 = 0

		// PD.7 default as NMI Port
		HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY_DD;
		HWREG(GPIO_PORTD_BASE + GPIO_O_CR)  = 0x80;
			
		ROM_GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, 0x80);
		
		HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY_DD;
		HWREG(GPIO_PORTD_BASE + GPIO_O_CR)  = 0x00;
		HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0;
}


//*****************************************************************************
//
//! Determines whether an ILI9320, ILI9325 or ILI9328 controller is present.
//!
//! This function queries the ID of the display controller in use and returns
//! it to the caller.  This driver supports both ILI9320, ILI9325 and ILI9328.
//! These are very similar but the sense of the long display axis is reversed
//! in the Formike KWH028Q02-F03 using an ILI9320 relative to the other two
//! supported displays and this information is needed by the touchscreen driver
//! to provide correct touch coordinate information.
//!
//! \return Returns 0x9320 if an ILI9320 controller is in use, 0x9325 if an
//! ILI9325 is present or 0x9328 if an ILI9328 is detected.
//
//*****************************************************************************
unsigned short lcd_get_control(void)
{
    unsigned short usController;

    // Determine which version of the display controller we are using.
    usController = LCD_ReadRegister(0x00);

    return(usController);
}

void LCD_Clear(unsigned short Color)
{
    unsigned int count;

	lcd_write_command(0x2a);	
	lcd_write_data(0);	    
	lcd_write_data(0);
	lcd_write_data(HDP>>8);	    
	lcd_write_data(HDP&0x00ff);
    lcd_write_command(0x2b);	
	lcd_write_data(0);	    
	lcd_write_data(0);
	lcd_write_data(VDP>>8);	    
	lcd_write_data(VDP&0x00ff);
	lcd_write_command(0x2c);	
	lcd_write_command(0x2c);


	for(count=0;count<130560;count++){
		
		lcd_write_data(Color);
	}
}

/************************************************************************************
** Function Name : S95196x240x320x16_ILI9320Init                                   **
** Description   : Initializes the display driver.                                 **
** Arguments     : This function initializes the ILI9320, ILI9325 or ILI9328       **
**                 display controller on the panel, preparing it to display data.  **
** Return        : None                                                            **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2011-10-20                                                      **
** Notes         :                                                                 **
************************************************************************************/
void lcd_init(void)
{
   unsigned long ulClockMS,ulCount;
    unsigned short usController;

    // Get the current processor clock frequency.
    ulClockMS = ROM_SysCtlClockGet() / (3 * 1000);
 	
	//===========================================================
    lcd_init_gpio();
	
	ROM_GPIOPinWrite(LCD_LEDK_BASE, LCD_LEDK_PIN, LCD_LEDK_PIN);   	// LCD_LED_K1 = 0
				
	lcddev.bus16=1;//���߽ӿڣ�0-8λ���ߣ�1-16λ���ߣ��޸�8λ����֮ǰ��ȷ������������Ƿ���8λ�ӿڣ�Ŀǰֻ��2.4��2.8�����֧��8λ�İ汾��3.2�����Ͼ���֧��8λ���� 
	lcddev.id=0x1943;//ָ��ID,0x1234Ϊ�Զ���ID,�м�ע�ⲻ�����е������ܶ���ID,ֻ���ٲ�������IC�ܶ�ID,����4.3���Լ�4.3�����ϵĶ����ܶ�ID.

	ROM_GPIOPinWrite(LCD_CS_BASE, LCD_CS_PIN, 0x00);						    	// LCD_CS = 0
			// Reset LCD
	ROM_GPIOPinWrite(LCD_RST_BASE, LCD_RST_PIN, LCD_RST_PIN);					// LCD_RST = 1
	ROM_SysCtlDelay(20 * ulClockMS);
	ROM_GPIOPinWrite(LCD_RST_BASE, LCD_RST_PIN, 0);						        // LCD_RST = 0
	ROM_SysCtlDelay(20 * ulClockMS);
	ROM_GPIOPinWrite(LCD_RST_BASE, LCD_RST_PIN, LCD_RST_PIN);					// LCD_RST = 1
	ROM_GPIOPinWrite(LCD_CS_BASE, LCD_CS_PIN, LCD_CS_PIN);						// LCD_CS = 1
  ROM_GPIOPinWrite(LCD_RD_BASE, LCD_RD_PIN, LCD_RD_PIN);						// LCD_RD = 1
  ROM_GPIOPinWrite(LCD_WR_BASE, LCD_WR_PIN, LCD_WR_PIN);						// LCD_WR = 1
 	ROM_SysCtlDelay(5 * ulClockMS); 
	ROM_GPIOPinWrite(LCD_CS_BASE, LCD_CS_PIN, 0);											// LCD_CS = 0
			
	if(lcddev.id==0x1943)
	{
		lcddev.width=480;
		lcddev.height=272;
		lcddev.wramcmd=0X2C;
		lcddev.setxcmd=0X2A;
		lcddev.setycmd=0X2B;  
	}			
			
	lcd_write_command(0x002b);	
	lcd_write_data(0);

	ROM_SysCtlDelay(5 * ulClockMS); // delay 50 ms 
	lcd_write_command(0x00E2);					//PLL multiplier, set PLL clock to 120M
	lcd_write_data(0x001D);					//N=0x36 for 6.5M, 0x23 for 10M crystal
	lcd_write_data(0x0002);
	lcd_write_data(0x0004);
	
	lcd_write_command(0x00E0);					//PLL enable
	lcd_write_data(0x0001);
	ROM_SysCtlDelay(1 * ulClockMS);
	lcd_write_command(0x00E0);
	lcd_write_data(0x0003);
	ROM_SysCtlDelay(5 * ulClockMS);
	lcd_write_command(0x0001);  					//software reset
	ROM_SysCtlDelay(5 * ulClockMS);
	lcd_write_command(0x00E6);					//PLL setting for PCLK, depends on resolution
//	LCD_WriteRAM(0x0001);
//	LCD_WriteRAM(0x0033);
//	LCD_WriteRAM(0x0032);
	lcd_write_data(0x0000);
	lcd_write_data(0x00D9);
	lcd_write_data(0x0016);

	lcd_write_command(0x00B0);					//LCD SPECIFICATION
	lcd_write_data(0x0020);
	lcd_write_data(0x0000);
	lcd_write_data((HDP>>8)&0X00FF);			//Set HDP
	lcd_write_data(HDP&0X00FF);
  lcd_write_data((VDP>>8)&0X00FF);			//Set VDP
	lcd_write_data(VDP&0X00FF);
  lcd_write_data(0x0000);

	lcd_write_command(0x00B4);					//HSYNC
	lcd_write_data((HT>>8)&0X00FF); 			//Set HT
	lcd_write_data(HT&0X00FF);
	lcd_write_data((HPS>>8)&0X00FF);			//Set HPS
	lcd_write_data(HPS&0X00FF);
	lcd_write_data(HPW);						//Set HPW
	lcd_write_data((LPS>>8)&0X00FF); 			//Set HPS
	lcd_write_data(LPS&0X00FF);
	lcd_write_data(0x0000);

	lcd_write_command(0x00B6);					//VSYNC
	lcd_write_data((VT>>8)&0X00FF);   		//Set VT
	lcd_write_data(VT&0X00FF);
	lcd_write_data((VPS>>8)&0X00FF); 			//Set VPS
	lcd_write_data(VPS&0X00FF);
	lcd_write_data(VPW);						//Set VPW
	lcd_write_data((FPS>>8)&0X00FF);			//Set FPS
	lcd_write_data(FPS&0X00FF);

	//=============================================

	//=============================================
	lcd_write_command(0x00BA);
	lcd_write_data(0x0005);//0x000F);    //GPIO[3:0] out 1

	lcd_write_command(0x00B8);
	lcd_write_data(0x0007);    //GPIO3=input, GPIO[2:0]=output
	lcd_write_data(0x0001);    //GPIO0 normal

	lcd_write_command(0x0036); //rotation
	lcd_write_data(0x0000);

	ROM_SysCtlDelay(50 * ulClockMS);

	lcd_write_command(0x00BE); //set PWM for B/L
	lcd_write_data(0x0006);
	lcd_write_data(0x0080);
	
	lcd_write_data(0x0001);
	lcd_write_data(0x00f0);
	lcd_write_data(0x0000);
	lcd_write_data(0x0000);

	lcd_write_command(0x00d0);//���ö�̬����������� 
	lcd_write_data(0x000d);
   
	lcd_write_command(0x00F0); //pixel data interface
	lcd_write_data(0x0003); //03:16λ   02:24λ

	lcd_write_command(0x0029); //display on
}

/************************************************************************************
** Function Name : lcd_backlight_on                                                **
** Description   : Turns on the backlight.                                         **
**				   This function turns on the backlight on the display.            **
** Arguments     :                                                                 **
** Return        : None                                                            **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2011-10-20                                                      **
** Notes         :                                                                 **
************************************************************************************/
void lcd_backlight_on(void)
{
    // Deassert the signal that turns on the backlight.
	ROM_GPIOPinWrite(LCD_LEDK_BASE, LCD_LEDK_PIN, LCD_LEDK_PIN);
}

/************************************************************************************
** Function Name : lcd_backlight_off                                               **
** Description   : Turns off the backlight.                                        **
**				   This function turns off the backlight on the display.           **
** Arguments     :                                                                 **
** Return        : None                                                            **
** Output        :                                                                 **
=====================================================================================
** Author        : Triton.zhang                                                    **
** Data          : 2011-10-20                                                      **
** Notes         :                                                                 **
************************************************************************************/
void lcd_backlight_off(void)
{
    // Deassert the signal that turns off the backlight.
	ROM_GPIOPinWrite(LCD_LEDK_BASE, LCD_LEDK_PIN, 0);
}


//д�Ĵ���
//LCD_Reg:�Ĵ�����ַ
//LCD_RegValue:Ҫд�������
void LCD_WR_REG_DATA(unsigned char LCD_Reg, unsigned int LCD_RegValue)
{
	lcd_write_command(LCD_Reg);
	lcd_write_data(LCD_RegValue);
}	

void Address_set1121(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{ 
	LCD_WR_REG_DATA(0x0046,(((unsigned int)(x2))<<8)+x1);	  
    LCD_WR_REG_DATA(0x0047,y2);  
    LCD_WR_REG_DATA(0x0048,y1);
  	LCD_WR_REG_DATA(0x0020,x1);	  
    LCD_WR_REG_DATA(0x0021,y1); 				 
}


//���ù��λ��
//Xpos:������
//Ypos:������
void LCD_SetCursor(unsigned int Xpos, unsigned int Ypos)
{	 
 	if(lcddev.id==0X9341||lcddev.id==0X6804)
	{		    
		lcd_write_command(lcddev.setxcmd); 
		lcd_write_data(Xpos>>8); 
		lcd_write_data(Xpos&0XFF);	 
		lcd_write_command(lcddev.setycmd); 
		lcd_write_data(Ypos>>8); 
		lcd_write_data(Ypos&0XFF);
	}else if(lcddev.id==0x1907||lcddev.id==0x1905)
	{
			lcd_write_command(0x002A);	
			lcd_write_data(Xpos>>8);	    
			lcd_write_data(Xpos&0x00ff);
			lcd_write_data(799>>8);	    
			lcd_write_data(799&0x00ff);
		    lcd_write_command(0x002b);	
			lcd_write_data(Ypos>>8);	    
			lcd_write_data(Ypos&0x00ff);
			lcd_write_data(479>>8);	    
			lcd_write_data(479&0x00ff);
	}else if(lcddev.id==0x1943)		//in
	{
			lcd_write_command(0x002A);	
			//lcd_write_data(Xpos>>8);	    
			//lcd_write_data(Xpos&0x00ff);
		lcd_write_data(Xpos);
			//lcd_write_data(479>>8);	    
			//lcd_write_data(479&0x00ff);
		lcd_write_data(479);
		    lcd_write_command(0x002b);	
			//lcd_write_data(Ypos>>8);	    
			//lcd_write_data(Ypos&0x00ff);
		lcd_write_data(Ypos);
			//lcd_write_data(271>>8);	    
			//lcd_write_data(271&0x00ff);
		lcd_write_data(271);
	}else if(lcddev.id==0x1121)			
	{
		Address_set1121(Xpos,Ypos,239,319);
	}else if(lcddev.id==0x0065)
	{
		LCD_WR_REG_DATA(0x06,Ypos>>8);
		LCD_WR_REG_DATA(0x07,Ypos);
	//	LCD_WR_REG_DATA(0x08,y2>>8);
	//	LCD_WR_REG_DATA(0x09,y2);
		LCD_WR_REG_DATA(0x02,Xpos>>8);
		LCD_WR_REG_DATA(0x03,Xpos);
	//	LCD_WR_REG_DATA(0x04,x2>>8);
	//	LCD_WR_REG_DATA(0x05,x2); 
	
		LCD_WR_REG_DATA(0x80, Xpos>>8);
	    LCD_WR_REG_DATA(0x81, Xpos);    
	    LCD_WR_REG_DATA(0x82, Ypos>>8);
	    LCD_WR_REG_DATA(0x83, Ypos);    
	}else if(lcddev.id==0x0052)
	{
		LCD_WR_REG_DATA(0x06,Ypos>>8);
		LCD_WR_REG_DATA(0x07,Ypos);
	//	LCD_WR_REG_DATA(0x08,y2>>8);
	//	LCD_WR_REG_DATA(0x09,y2);
		LCD_WR_REG_DATA(0x02,Xpos>>8);
		LCD_WR_REG_DATA(0x03,Xpos);
	//	LCD_WR_REG_DATA(0x04,x2>>8);
	//	LCD_WR_REG_DATA(0x05,x2); 

	}else if(lcddev.id==0x0047)
	{
		LCD_WR_REG_DATA(0x0002,Xpos>>8);	   // Column address start2
		LCD_WR_REG_DATA(0x0003,Xpos);	   // Column address start1
	//	LCD_WR_REG_DATA(0x0004,x2>>8);	   // Column address end2
	//	LCD_WR_REG_DATA(0x0005,x2);	   // Column address end1
		LCD_WR_REG_DATA(0x0006,Ypos>>8);	   // Row address start2
		LCD_WR_REG_DATA(0x0007,Ypos);	   // Row address start1
	  //  LCD_WR_REG_DATA(0x0008,y2>>8);	   // Row address end2
	//	LCD_WR_REG_DATA(0x0009,y2);	   // Row address end1
	  //  lcd_write_command(0x0022);	
	}else if(lcddev.id==0x9325||lcddev.id==0x9328)
	{	
		LCD_WR_REG_DATA(0x0020,Xpos);
		LCD_WR_REG_DATA(0x0021,Ypos);	 
		LCD_WR_REG_DATA(0x0050,Xpos);
		LCD_WR_REG_DATA(0x0052,Ypos);
	}else
	{
		LCD_WR_REG_DATA(lcddev.setxcmd, Xpos);
		LCD_WR_REG_DATA(lcddev.setycmd, Ypos);
	}	 
} 
 
//��ʼдGRAM
void LCD_WriteRAM_Prepare(void)
{
	lcd_write_command(lcddev.wramcmd);
}	 

void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{ 
	lcd_write_command(0x002A);	
	lcd_write_data(x1>>8);	    
	lcd_write_data(x1&0x00ff);
	lcd_write_data(x2>>8);	    
	lcd_write_data(x2&0x00ff);
    lcd_write_command(0x002b);	
	lcd_write_data(y1>>8);	    
	lcd_write_data(y1&0x00ff);
	lcd_write_data(y2>>8);	    
	lcd_write_data(y2&0x00ff);
	lcd_write_command(0x002c);					 						 
}

//����
//POINT_COLOR:�˵����ɫ
void LCD_DrawPoint(unsigned int x,unsigned int y)
{
	Address_set(x,y,x,y);//���ù��λ�� 
	lcd_write_data(POINT_COLOR); 	    
} 	

//����
//x1,y1:�������
//x2,y2:�յ�����  
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		LCD_DrawPoint(uRow,uCol);//���� 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    


//������	  
//(x1,y1),(x2,y2):���εĶԽ�����
void LCD_DrawRectangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}



#define MAX_HZ_POSX 480
#define MAX_HZ_POSY 227 


//��ָ��λ����ʾһ���ַ�

//num:Ҫ��ʾ���ַ�:" "--->"~"
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
//��ָ��λ����ʾһ���ַ�

//num:Ҫ��ʾ���ַ�:" "--->"~"

//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void LCD_ShowChar(unsigned int x,unsigned int y,unsigned char num,unsigned char mode)
{
    unsigned char temp;
    unsigned char pos,t;
	unsigned int x0=x;
	unsigned int colortemp=POINT_COLOR;      
    if(x>LCD_W-16||y>LCD_H-16)return;	    
	//���ô���		   
	num=num-' ';//�õ�ƫ�ƺ��ֵ
	Address_set(x,y,x+8-1,y+16-1);      //���ù��λ�� 
	if(!mode) //�ǵ��ӷ�ʽ
	{
		for(pos=0;pos<16;pos++)
		{ 
			temp = asc2_1608[(unsigned int)num * 16 + pos];		 //����1608����
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)POINT_COLOR=colortemp;
				else POINT_COLOR=BACK_COLOR;
				lcd_write_data(POINT_COLOR);	
				temp>>=1; 
				x++;
		    }
			x=x0;
			y++;
		}	
	}else//���ӷ�ʽ
	{
		for(pos=0;pos<16;pos++)
		{
		    temp = asc2_1608[(unsigned int)num*16+pos];		 //����1608����
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)LCD_DrawPoint(x+t,y+pos);//��һ����     
		        temp>>=1; 
		    }
		}
	}
	POINT_COLOR=colortemp;	    	   	 	  
}   

//��ʾ�ַ���
//x,y:�������
//width,height:�����С  
//size:�����С
//*p:�ַ�����ʼ��ַ		  
void LCD_ShowString(unsigned int x0,unsigned int y0,const unsigned char *pcStr)
{ 
	//while(*pcStr!='\0')
	while(*pcStr)
	{
	 //	if(*pcStr>0x80) /*??��?oo��?*/0xa1
		if(*pcStr>0xa1) /*??��?oo��?*/
		{
			//LCD_ShowHzString(x0, y0, pcStr);
			//pcStr += 2;
			//x0 += 16;	
		}
		else           /*??��?��?��?*/
		{
			LCD_ShowChar(x0, y0, *pcStr, 0);	
			pcStr +=1;
			x0+= 8;
		}	
	}        
}
