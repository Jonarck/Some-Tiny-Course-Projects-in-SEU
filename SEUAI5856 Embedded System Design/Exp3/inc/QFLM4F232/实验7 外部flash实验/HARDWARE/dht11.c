#include "dht11.h"

//PG7  -> DHT11

typedef unsigned char  U8;       //defined for unsigned 8-bits integer variable 	  �޷���8λ���ͱ���  
typedef unsigned int   U16;      // defined for unsigned 16-bits integer variable 	  �޷���16λ���ͱ��� 
unsigned char  U8FLAG,k;
unsigned char  U8count,U8temp;
unsigned char  U8T_data_H,U8T_data_L,U8RH_data_H,U8RH_data_L,U8checkdata;
unsigned char  U8T_data_H_temp,U8T_data_L_temp,U8RH_data_H_temp,U8RH_data_L_temp,U8checkdata_temp;
unsigned char  U8comdata;

unsigned char  indata[5];
unsigned char  count_r=0;
unsigned char  str[5]={"RS232"};
unsigned char  U16temp1,U16temp2;
u8 tdata[4]={0x00,0x00,0x00,0x00};
u8 sbuf;

void DTH11_OUT() 
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
	ROM_GPIOPinTypeGPIOOutput(DHT11_BASE, DHT11_PIN);	
	
	ROM_GPIOPinWrite(DHT11_BASE, DHT11_PIN, DHT11_PIN);
}

void DTH11_IN()  
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
	ROM_GPIOPinTypeGPIOInput(DHT11_BASE, DHT11_PIN);	
}

void DTH11_Delay(U16 j)
    {     
			U8 i;
	    for(;j>0;j--)
	  { 	
			for(i=0;i<18;i++);
	  }
    } 
		
void  DTH11_Delay_10us(void)
{
	unsigned char i;
	i--;
	i--;
	i--;
	i--;
	i--;
	i--;
 }

void  DTH11_COM(void)
{

	unsigned char i;
	char count = 0;

	U8comdata=0;
	for(i=0;i<8;i++)	   
	{
		DTH11_IN();	

		while(!READ_DEHT11);
		DTH11_Delay(4);
		U8temp=0;
		if(READ_DEHT11)U8temp=1;

		while(READ_DEHT11);
		// while((P2_0)&&U8FLAG++);
		//��ʱ������forѭ��		  
		//	 if(U8FLAG==1)break;
		//�ж�����λ��0����1	 
			 
		// ����ߵ�ƽ�߹�Ԥ��0�ߵ�ƽֵ������λΪ 1 
	  U8comdata<<=1;
	  U8comdata|=U8temp;        //0
	}
//U8comdata=9;
}

void DTH11_RH(void)
	{
	  //��������18ms 
		DTH11_OUT();
		DHT11_L;
		DTH11_Delay(2850);
		DHT11_H;
#if 1	
	 //������������������ ������ʱ20us
		DTH11_Delay(1);
	 //������Ϊ���� �жϴӻ���Ӧ�ź� 
	//DHT11_H;
	   //P2_0=1;
		//�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������	
			DTH11_IN();
		if(!READ_DEHT11)
	   //if(!P2_0)		 //T !	  
	   {
	   U8FLAG=2;
	 //�жϴӻ��Ƿ񷢳� 80us �ĵ͵�ƽ��Ӧ�ź��Ƿ����	 
			 //while((!READ_DEHT11)&&U8FLAG++);
			 while(!READ_DEHT11);  //xie
	   U8FLAG=2;
	 //�жϴӻ��Ƿ񷢳� 80us �ĸߵ�ƽ���緢����������ݽ���״̬

			//while((READ_DEHT11)&&U8FLAG++);	
			while(READ_DEHT11);		//xie			 
			//while((P2_0)&&U8FLAG++);
	 //���ݽ���״̬		 
	   DTH11_COM();
	   U8RH_data_H_temp=U8comdata;
	   DTH11_COM();
	   U8RH_data_L_temp=U8comdata;
	   DTH11_COM();
 	   U8T_data_H_temp=U8comdata;
	   DTH11_COM();
	   U8T_data_L_temp=U8comdata;
	   DTH11_COM();
	   U8checkdata_temp=U8comdata;
			 DTH11_OUT();
			 DHT11_H;
	  // P2_0=1;
	 //����У�� 
	 
	   U8temp=(U8T_data_H_temp+U8T_data_L_temp+U8RH_data_H_temp+U8RH_data_L_temp);
	  if(U8temp==U8checkdata_temp)
	   {
	   	  U8RH_data_H=U8RH_data_H_temp;
	   	  U8RH_data_L=U8RH_data_L_temp;
		  U8T_data_H=U8T_data_H_temp;
	   	  U8T_data_L=U8T_data_L_temp;
	   	  U8checkdata=U8checkdata_temp;
	   }//fi
	   }//fi
#endif
}
	
void Send_DTH11(void)
{
	u8 DHT11_temp[17]={"temp_value:00.0 C"};
	u8 DHT11_humi[17]={"humi_value:00.0RH"};
	str[0]=U8RH_data_H;
	str[1]=U8RH_data_L;
	str[2]=U8T_data_H;
	str[3]=U8T_data_L;
	str[4]=U8checkdata;

	DHT11_temp[11]=(str[2]%100)/10+'0';  // ȡʮλ 
	DHT11_temp[12]=str[2]%10+'0';        // ȡ��λ
	LCD_ShowString(150, 140,DHT11_temp);
	
			
	DHT11_humi[11]=(str[0]%100)/10+'0';  // ȡʮλ 
	DHT11_humi[12]=str[0]%10+'0';        // ȡ��λ
	LCD_ShowString(150, 160,DHT11_humi);		
}

void DHT11_Test(void)
{
		DTH11_RH();
		Send_DTH11();
}

//OUTA4
void GAS_Test(void)
{
	uint8_t gas_read = 0;
	
	gas_read = PCF8574_Single_ReadI2C(PCF8574T_A);
	
	if(!(gas_read & 0x10))
		LCD_ShowString(150, 200, "********GAS IN********");	
  else LCD_ShowString(150, 200, "********GAS NO********");
}

//OUTA5
void HUMAN_Test(void)
{
	uint8_t human_read = 0;
	
	human_read = PCF8574_Single_ReadI2C(PCF8574T_A);
	
	if((human_read & 0x20))
		LCD_ShowString(150, 180, "*******HUMAN IN*******");
  else LCD_ShowString(150, 180, "*******HUMAN NO*******"); 
		
	
}