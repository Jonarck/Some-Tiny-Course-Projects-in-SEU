#include "tm1638.h"
#include "utils/uartstdio.h"	  

unsigned char const tab[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};//数码管0到F的显示
unsigned char const keytab[]={0x01,0x02,0x10,0x0f,0x11,0x12,0x00,0x00,0x03,0x04,0x0e,0x0d,0x13,0x14,0x00,0x00,0x05,0x06,0x0c,0x0b,0x15,0x16,0x00,0x00,0x07,0x08,0x0a,0x09,0x17,0x18,00};														

u8 key_tmp=0xff;

//TM1638 STB接PF2  CLK接PK2    DIO接PK0     NEW
void delay_tim(unsigned int tim)   //8us/unit
{
	unsigned int i;
	for(;tim>0;tim--)
	 for(i=0;i<28;i++);
}

//读数据延时函数
static Delay_1ms(int counter)
{
	int i,j;
	for(i=0;i<counter;i++)
		for(j = 0; j < 4000; j++);
}



void TM1638_GPIO_Configuration(void)
{		
	//PF2 ->STB	
	ROM_SysCtlPeripheralEnable(STB_PERIPH);	  // 使能外设GPIOF
	ROM_GPIOPinTypeGPIOOutput(STB_GPIO, STB_GPIO_PIN);	//PF2 设为输出
	STB_ONOFF(STB_GPIO_PIN);
	
	//PK2 ->CLK
  ROM_SysCtlPeripheralEnable(CLK_PERIPH);	  // 使能外设GPIOF
	ROM_GPIOPinTypeGPIOOutput(CLK_GPIO, CLK_GPIO_PIN);	//PK2 设为输出
	ROM_GPIOPinTypeGPIOOutput(DIO_GPIO, DIO_GPIO_PIN);	//PK0 设为输出	
	//DIO_OUT();	
	DIO_ONOFF(DIO_GPIO_PIN);
}

void DIO_OUT() //配置TM1638DIO引脚为推挽输出
{
  //PK0 ->DIO
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);	//PK0 设为输出
	ROM_GPIOPinTypeGPIOOutput(DIO_GPIO, DIO_GPIO_PIN);	//PK0 设为输出		
}

void DIO_IN()  //配置TM1638DIO引脚为上拉输入
{
	 //PK0 ->DIO
	 ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
	 ROM_GPIOPinTypeGPIOInput(DIO_GPIO, DIO_GPIO_PIN);	//PK0 设为输入	
}

void TM1638_Write(unsigned char	DATA)			//写数据函数
{
   unsigned char i;
   DIO_OUT();
				
   for(i=0;i<8;i++)
   {
   	//GPIO_ResetBits(GPIOF,CLK_GPIO_PIN);
		 CLK_ONOFF(0);
	  if(DATA&0X01)
		{
	    //GPIO_SetBits(GPIOB,DIO_GPIO_PIN);
			DIO_ONOFF(DIO_GPIO_PIN);
		}
	  else 
		{
	    //GPIO_ResetBits(GPIOB,DIO_GPIO_PIN);
			DIO_ONOFF(0);
		}
	  DATA>>=1;
	  //GPIO_SetBits(GPIOF,CLK_GPIO_PIN);
		CLK_ONOFF(CLK_GPIO_PIN);
	  delay_tim(1);
   }
}

unsigned char TM1638_Read(void)					//读数据函数
{
	unsigned char i;
	unsigned char temp=0;
	DIO_IN();
	delay_tim(1);
	for(i=0;i<8;i++)
	{
	  temp>>=1;
	 // GPIO_ResetBits(GPIOF,CLK_GPIO_PIN);
		CLK_ONOFF(0);
		delay_tim(10);
		//if(GPIO_ReadInputDataBit(GPIOB,DIO_GPIO_PIN) == Bit_SET)
		if(ROM_GPIOPinRead(GPIO_PORTK_BASE, CLK_GPIO_PIN) != 0)
	    temp|=0x80;
	  //GPIO_SetBits(GPIOF,CLK_GPIO_PIN);
		CLK_ONOFF(CLK_GPIO_PIN);
		
		delay_tim(1);
	}
	DIO_OUT();

	return temp;
}

void Write_COM(unsigned char cmd)		//发送命令字
{
  //GPIO_ResetBits(GPIOF,STB_GPIO_PIN);
	STB_ONOFF(0);
	TM1638_Write(cmd);
	//GPIO_SetBits(GPIOF,STB_GPIO_PIN);
	STB_ONOFF(STB_GPIO_PIN);
}

unsigned char c[4];
unsigned char Read_key(void)
{
	unsigned char i,key_value=0;
	unsigned char tmp_num[4]={0,0,0,0};
	int* key;
	unsigned char temp[1] = {0};
	
   //GPIO_ResetBits(GPIOF,STB_GPIO_PIN);
	STB_ONOFF(0);
	TM1638_Write(0x42);
	//Delay(1);
	Delay_1ms(1);
	/*for(i=0;i<200;i++)
	delay_tim(200);*/
	for(i=0;i<4;i++)
	{
		c[i]=TM1638_Read();
		
		temp[0] = c[i];
		temp[0] = temp[0] + '0';
		UARTprintf(temp);
		temp[0] = 0;		
		//delay_tim(10);
	}
	//GPIO_SetBits(GPIOF,STB_GPIO_PIN);				//4个字节数据合成一个字节
	STB_ONOFF(STB_GPIO_PIN);
	for(i=0;i<4;i++)
		{
			if(c[i]!=0)
				switch(c[i])
			{
				case 0x01:tmp_num[i]=i*8+1; break;
				case 0x10:tmp_num[i]=i*8+2; break;
				case 0x02:tmp_num[i]=i*8+3; break;
				case 0x20:tmp_num[i]=i*8+4; break;
				case 0x04:tmp_num[i]=i*8+5; break;
				case 0x40:tmp_num[i]=i*8+6; break;
				case 0x08:tmp_num[i]=i*8+7; break;
				case 0x80:tmp_num[i]=i*8+8; break;
				default : tmp_num[i]=0;break;
			}
		}
		key_value=tmp_num[0]+tmp_num[1]+tmp_num[2]+tmp_num[3];   
  return (key_value);
}


void Write_DATA(unsigned char add,unsigned char DATA)
{
    Write_COM(0x44);
   // GPIO_ResetBits(GPIOF,STB_GPIO_PIN);
	STB_ONOFF(0);
	TM1638_Write(0xc0|add);
	TM1638_Write(DATA);
	//GPIO_SetBits(GPIOF,STB_GPIO_PIN);
	STB_ONOFF(STB_GPIO_PIN);
}

void Write_oneLED(unsigned char num,unsigned char flag)	//单独控制一个LED函数，num为需要控制的led序号，flag为0时熄灭，不为0时点亮
{
	Write_DATA(num,flag);
}
void Init_TM1638(void)
{	
	unsigned char i;
	
	TM1638_GPIO_Configuration();   //IO口初始化

	Write_COM(0x8a);//脉冲宽度为全亮度的4/16
	Write_COM(0x40);
	//GPIO_ResetBits(GPIOF,STB_GPIO_PIN);
	STB_ONOFF(0);
	TM1638_Write(0xc0);
	for(i=0;i<16;i++)
	TM1638_Write(0x00);
	//GPIO_SetBits(GPIOF,STB_GPIO_PIN);
	STB_ONOFF(STB_GPIO_PIN);

	DIO_OUT();
}

/****************************************************************************
* 名    称：void TM1638Test(void)
* 功    能：显示主菜单
* 入口参数：无
* 出口参数：无
* 说    明：在LCD上显示8LED测试菜单
* 调用方法：TM1638Test();
****************************************************************************/

void TM1638Test(void)
{
	u8 i,j;
	u8 keyDat;
//	u8 tempcount=0;
	u8 temp_key1=0x00;
	u8 temp_key2=0x00;
	u8 temp_key3=0x00;
	
	Init_TM1638();	
	

	for(i=0;i<16;i++)
	{
	  for(j=0;j<8;j++)
		{
			Write_DATA(j<<1,tab[i]); 
			delay_tim(1000);
		}
	}
	delay_tim(100);
	
	for(j=0;j<8;j++)
	  Write_DATA(j<<1,tab[0]); 

	for(i=0;i<16;i++)
	{ i++;
	  for(j=0;j<3;j++)
		{
			Write_oneLED(i,j);
			delay_tim(1000);
		}
	}
	
	//while(tempcount<24)
	//while(1)
	temp_key1=0x00;
	temp_key2=0x00;
	temp_key3=0x00;
	
	//while((temp_key1!=0xff)||(temp_key2!=0xff)||(temp_key3!=0xff))
	while(0)
	{
		keyDat=Read_key();
		
#if 1		
		keyDat=keytab[keyDat-1];
		if(keyDat<0x1f)
		{   	//printf("%c\n",keyDat);
					//for(j=0;j<8;j++)
			Write_DATA(2,tab[keyDat&0x0f]);
			Write_DATA(0,tab[(keyDat&0xf0)>>4]);
			if(keyDat!=key_tmp)
			{
			//tempcount++;
				key_tmp=keyDat;
			}
			switch (keyDat){
				case 0x01:
				LCD_ShowString(120, 120, "KEY_ID:KEY01");
				temp_key1=temp_key1|0x01;
				break;
				case 0x02:
				LCD_ShowString(120, 120, "KEY_ID:KEY02");
				temp_key1=temp_key1|0x02;
				break;
				case 0x03:
				LCD_ShowString(120, 120, "KEY_ID:KEY03");
				temp_key1=temp_key1|0x04;
				break;
				case 0x04:
				LCD_ShowString(120, 120, "KEY_ID:KEY04");
				temp_key1=temp_key1|0x08;
				break;
				case 0x05:
				LCD_ShowString(120, 120, "KEY_ID:KEY05");
				temp_key1=temp_key1|0x10;
				break;
				case 0x06:
				LCD_ShowString(120, 120, "KEY_ID:KEY06");
				temp_key1=temp_key1|0x20;
				break;
				case 0x07:
				LCD_ShowString(120, 120, "KEY_ID:KEY07");
				temp_key1=temp_key1|0x40;
				break;
				case 0x08:
				LCD_ShowString(120, 120, "KEY_ID:KEY08");
				temp_key1=temp_key1|0x80;
				break;
				//
				case 0x09:
				LCD_ShowString(120, 120, "KEY_ID:KEY09");
				temp_key2=temp_key2|0x01;
				break;
				case 0x0a:
				LCD_ShowString(120, 120, "KEY_ID:KEY0A");
				temp_key2=temp_key2|0x02;
				break;
				case 0x0b:
				LCD_ShowString(120, 120, "KEY_ID:KEY0B");
				temp_key2=temp_key2|0x04;
				break;
				case 0x0c:
				LCD_ShowString(120, 120, "KEY_ID:KEY0C");
				temp_key2=temp_key2|0x08;
				break;
				case 0x0d:
				LCD_ShowString(120, 120, "KEY_ID:KEY0D");
				temp_key2=temp_key2|0x10;
				break;
				case 0x0e:
				LCD_ShowString(120, 120, "KEY_ID:KEY0E");
				temp_key2=temp_key2|0x20;
				break;
				case 0x0f:
				LCD_ShowString(120, 120, "KEY_ID:KEY0F");
				temp_key2=temp_key2|0x40;
				break;
				case 16:
				LCD_ShowString(120, 120, "KEY_ID:KEY10");
				temp_key2=temp_key2|0x80;
				break;
			//
				case 0x11:
				LCD_ShowString(120, 120, "KEY_ID:KEY11");
				temp_key3=temp_key3|0x01;
				break;
				case 0x12:
				LCD_ShowString(120, 120, "KEY_ID:KEY12");
				temp_key3=temp_key3|0x02;
				break;
				case 0x13:
				LCD_ShowString(120, 120, "KEY_ID:KEY13");
				temp_key3=temp_key3|0x04;
				break;
				case 0x14:
				LCD_ShowString(120, 120, "KEY_ID:KEY14");
				temp_key3=temp_key3|0x08;
				break;
				case 0x15:
				LCD_ShowString(120, 120, "KEY_ID:KEY15");
				temp_key3=temp_key3|0x10;
				break;
				case 0x16:
				LCD_ShowString(120, 120, "KEY_ID:KEY16");
				temp_key3=temp_key3|0x20;
				break;
				case 0x17:
				LCD_ShowString(120, 120, "KEY_ID:KEY17");
				temp_key3=temp_key3|0x40;
				break;
				case 0x18:
				LCD_ShowString(120, 120, "KEY_ID:KEY18");
				temp_key3=temp_key3|0x80;
				break;
				
								
				default:
				break;
			}
		}
#endif
	}

}