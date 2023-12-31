#include "MMA7455.h"
#include "bmp085.h"
#include  <math.h>    //Keil library
#include "BMP085.h"

//    MMA7455
uchar table4[10]="0123456789";

uchar xposivalue;			//x������������ʾgֵ
uchar xnegavalue;			//x������������ʾgֵ
uchar yposivalue;			//y������������ʾgֵ
uchar ynegavalue;			//y������������ʾgֵ

void MMA7455_delay50us(void)
{
		
   int i=500;  
   while(i) 
   { 
     i--; 
   }  
}

//******************************************************************************************************
//��ʼ��MMA7455
//******************************************************************************************************
void MMA7455_data_init(void)
{
	xposivalue=50;
	xnegavalue=50;
	yposivalue=50;
	ynegavalue=50;
}

uint8_t MMA7455_self_test7455(void)					 //�������ܣ����7455��û�в��
{										 											 //          ���û�в�ã�1602������ʾ
	uchar j;							 								   //			 ��No acceleration!"
	char t, status, name;

	MMA7455_delay50us();
  Single_WriteI2C(MMA7455_IIC_WRITE, 0x16,0x15);	
	status = Single_ReadI2C(MMA7455_IIC_READ, 0x09);
	name = Single_ReadI2C(MMA7455_IIC_READ, 0x0f); //whoami
	MMA7455_delay50us();
	t = Single_ReadI2C(MMA7455_IIC_READ, 0x16);

	Single_WriteI2C(MMA7455_IIC_WRITE, 0x10,0x15);
	Single_WriteI2C(MMA7455_IIC_WRITE, 0x12,0x25);	
	Single_WriteI2C(MMA7455_IIC_WRITE, 0x14,0x10);	
	
	if(t!=0x15)
	{
		//printf("No acceleration!");
		return 0;
	}
	else
	{
		//printf("X= 0.00,Y= 0.00, Z= 0.00,t=%c\n",t);
		return 1;
	}
}

////////////////////////////////////////////////////////////////////////////////////////
void MMA7455_sendxyz(void)				
{
	u16 x1,x2,x3,xsign;
	uchar x_data[4], status;
	u16 x,y,z;
	u8 X_Angle[] = {"X:000"}, Y_Angle[] = {"Y:000"}, Z_Angle[] = {"Z:000"}; 

	x =Single_ReadI2C(MMA7455_IIC_READ, 0x06);
	y =Single_ReadI2C(MMA7455_IIC_READ, 0x07);
	z =Single_ReadI2C(MMA7455_IIC_READ, 0x08);

	X_Angle[2]=(x/63)+'0';
	X_Angle[3]=((x*100/63)%100)/10+'0';
	X_Angle[4]=(x*100/63)%10+'0';		
	//printf("X_angle %c%c%c ", X_Angle[2], X_Angle[3], X_Angle[4]);	
	LCD_ShowString(150, 200,X_Angle);		

	Y_Angle[2]=(y/63)+'0';
	Y_Angle[3]=((y*100/63)%100)/10+'0';
	Y_Angle[4]=(y*100/63)%10+'0';		
	//printf("Y_angle %c%c%c ", Y_Angle[2], Y_Angle[3], Y_Angle[4]);
	LCD_ShowString(200, 200,Y_Angle);	
	
	Z_Angle[2] = (z/63)+'0';
	Z_Angle[3] = ((z*100/63)%100)/10+'0';
	Z_Angle[4] =( z*100/63)%10+'0';			
	//printf("Z_angle %c%c%c\n", Z_Angle[2], Z_Angle[2], Z_Angle[2]);
	LCD_ShowString(250, 200,Z_Angle);	
}



void MMA7455_Test(void)
{
	MMA7455_self_test7455();	
	
	if(MMA7455_self_test7455())	
	{					
		MMA7455_sendxyz();
		delay5ms();	
	}
}
