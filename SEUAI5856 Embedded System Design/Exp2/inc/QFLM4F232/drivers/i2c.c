
********************************************************************************************************/
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_types.h"
#include "hw_gpio.h"
#include "interrupt.h"
#include "gpio.h"

// �궨��
#ifndef  uchar
#define  uchar unsigned char
#endif

#ifndef  uint 	 
#define  uint  unsigned int
#endif

void delay(uchar no)
{
    for(; no>0; no--);
}

// I2C���ŵĶ��塣
#define SDA  GPIO_PIN_3         //ģ��I2C���ݴ���λ
#define SCL  GPIO_PIN_2         //ģ��I2Cʱ�ӿ���λ

int ack;	                //Ӧ���־λack

/*******************************************************************
                     �����ߺ���               
����ԭ��: void  Start_I2c();  
����:     ����I2C����,������I2C��ʼ����
          ��ʼ����Ϊ����SCL���Ǹߵ�ƽʱ,SDA���ɸߵ�ƽ��͵�ƽ�л�   
********************************************************************/
void Start_I2c()
{
  //������ʼ�����������ź�
  HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SDA << 2))) = SDA;   // SDA = 1;
  delay(6);
  //������ʼ������ʱ���ź�
  HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SCL << 2))) = SCL;   // SCL = 1;
  delay(30);    	        //��ʼ��������ʱ�����4.7us,��ʱ
  //������ʼ�ź�
  HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SDA << 2))) = ~SDA;   // SDA = 0;
  delay(30);    	        //��ʼ��������ʱ�����4.7us
  //ǯסI2C���ߣ�׼�����ͻ�������� 
  HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SCL << 2))) = ~SCL;   // SCL = 0;
  delay(12);
}

/*******************************************************************
                       �������ߺ���               
����ԭ��: void  Stop_I2c();  
����:     ����I2C����,������I2C��������.
          ֹͣ����Ϊ����SCL���Ǹߵ�ƽʱ,SDA���ɵ͵�ƽ��ߵ�ƽ�л�
********************************************************************/
void Stop_I2c()
{
  HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SDA << 2))) = ~SDA;  	        //���ͽ��������������ź�
  delay(6);   	        
  HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SCL << 2))) = SCL;  	        //���ͽ���������ʱ���ź�
  delay(30);               //������������ʱ�����4��s
  HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SDA << 2))) = SDA;  	        //����I2C���߽����ź�
  delay(24);
}
/*******************************************************************
                 �ֽ����ݷ��ͺ���               
����ԭ��: void  SendByte(uchar c);
����:     ������c���ͳ�ȥ,�����ǵ�ַ,Ҳ����������,�����ȴ�Ӧ��,����
          ��״̬λ���в���.(��Ӧ����Ӧ��ʹack=0)     
     	  ��������������ack=1; ack=0��ʾ��������Ӧ����𻵡�
********************************************************************/
void  SendByte(uchar c)
{
 uchar BitCnt;
 
 //Ҫ���͵����ݳ���Ϊ8λ,
 for(BitCnt=0;BitCnt<8;BitCnt++)  
   {
     //�жϷ���λ  
     if((c<<BitCnt)&0x80)   
       {
        HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SDA << 2))) = SDA;   
       }
       else  
         {
          HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SDA << 2))) = ~SDA;
         }
        
     delay(6);
     HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SCL << 2))) = SCL;      //��ʱ����Ϊ�ߣ�֪ͨ��������ʼ��������λ
     delay(30);                                                     //��֤ʱ�Ӹߵ�ƽ���ڴ���4��s
     HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SCL << 2))) = ~SCL; 
   }
    
  delay(12);
  HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SDA << 2))) = SDA;         //8λ��������ͷ������ߣ�׼������Ӧ��λ
  delay(12);
  HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SCL << 2))) = SCL;
  delay(18);
  if(!HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SDA << 2))))                  //�ж��Ƿ���յ�Ӧ���ź�
       ack=0;
      else 
       ack=1;

  HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SCL << 2))) = ~SCL;
  delay(12);
}

/*******************************************************************
                 �ֽ����ݽ��պ���               
����ԭ��: uchar  RcvByte();
����:  	  �������մ���������������,���ж����ߴ���(����Ӧ���ź�)��
          ���������Ӧ����Ӧ��ӻ���  
********************************************************************/	
uchar  RcvByte()
{
  uchar retc;
  uchar BitCnt;
  
  retc=0; 
  HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SDA << 2))) = SDA;         
  GPIODirModeSet(GPIO_PORTB_BASE, SDA,GPIO_DIR_MODE_IN);             //��������Ϊ���뷽ʽ
  for(BitCnt=0;BitCnt<8;BitCnt++)
    {
     delay(6);           
     HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SCL << 2))) = ~SCL;     //��ʱ����Ϊ�ͣ�׼����������λ
     delay(30);                                                      //ʱ�ӵ͵�ƽ���ڴ���4.7��
     HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SCL << 2))) = SCL;      //��ʱ����Ϊ��ʹ��������������Ч
     delay(12);
     retc=retc<<1;

     if(HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SDA << 2))))         //������λ,���յ�����λ����retc�� 
         retc=retc+1;
 
     delay(12);
    }
  GPIODirModeSet(GPIO_PORTB_BASE, SDA,GPIO_DIR_MODE_OUT);            // ��������Ϊ�����ʽ
  HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SCL << 2))) = ~SCL;    
  delay(12);
  return(retc);
}

/********************************************************************
                     Ӧ���Ӻ���
����ԭ��:  void Ack_I2c(bit a);
����:      ����������Ӧ���ź�(������Ӧ����Ӧ���źţ���λ����a����)
********************************************************************/
void Ack_I2c(int a)
{
  if(a==0)
     HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SDA << 2))) = ~SDA;     //�ڴ˷���Ӧ����Ӧ���ź� 
    else 
      HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SDA << 2))) = SDA;

  delay(18);
  HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SCL << 2))) = SCL;
  delay(30);                                                         //ʱ�ӵ͵�ƽ���ڴ���4��s  
  HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SCL << 2))) = ~SCL;        //��ʱ���ߣ�ǯסI2C�����Ա��������
  delay(12);
}

/*******************************************************************
                 �û��ӿں���                                   
*******************************************************************/


/*******************************************************************
                 �����ӵ�ַ���������ֽ����ݺ���               
����ԭ��: int  ISendByte(uchar sla,uchar c);  
����:     ���������ߵ����͵�ַ�����ݣ��������ߵ�ȫ����,��������ַsla.
          �������1��ʾ�����ɹ��������������
ע�⣺    ʹ��ǰ�����ѽ������ߡ�
********************************************************************/
int ISendByte(uchar sla,uchar c)
{
  Start_I2c();          //��������
  SendByte(sla);        //����������ַ
  if(ack==0)
     return(0);

  SendByte(c);          //��������
  if(ack==0)            
     return(0);

  Stop_I2c();           //�������� 
  return(1);
}

/*******************************************************************
                    �����ӵ�ַ�������Ͷ��ֽ����ݺ���               
����ԭ��: int  ISendStr(uchar sla,uchar suba,uchar *s,uchar no);  
����:     ���������ߵ����͵�ַ���ӵ�ַ,���ݣ��������ߵ�ȫ����,������
          ��ַsla���ӵ�ַsuba������������sָ������ݣ�����no���ֽڡ�
          �������1��ʾ�����ɹ��������������
ע�⣺    ʹ��ǰ�����ѽ������ߡ�
********************************************************************/
int ISendStr(uchar sla,uchar suba,uchar *s,uchar no)
{
  uchar i;

  Start_I2c();          //��������
  SendByte(sla);        //����������ַ
  if(ack==0)           
    return(0);

  SendByte(suba);       //���������ӵ�ַ
  if(ack==0)
    return(0);

  for(i=0;i<no;i++)
    {   
     SendByte(*s);      //��������
     if(ack==0)
       return(0);
     s++;
    } 
  Stop_I2c();           //��������
  return(1);
}
/*******************************************************************
                    �����ӵ�ַ�������Ͷ��ֽ����ݺ���               
����ԭ��: int  ISendStrExt(uchar sla,uchar *s,uchar no);  
����:     ���������ߵ����͵�ַ���ӵ�ַ,���ݣ��������ߵ�ȫ����,������
          ��ַsla������������sָ������ݣ�����no���ֽڡ�
          �������1��ʾ�����ɹ��������������
ע�⣺    ʹ��ǰ�����ѽ������ߡ�
********************************************************************/
int ISendStrExt(uchar sla,uchar *s,uchar no)
{
  uchar i;

  Start_I2c();          //��������
  SendByte(sla);        //����������ַ
  if(ack==0)
    return(0);

  for(i=0;i<no;i++)
    {   
     SendByte(*s);      //��������
     if(ack==0)
       return(0);
     s++;
    } 
  Stop_I2c();           //�������� 
  return(1);
}

/*******************************************************************
                    �����ӵ�ַ������ȡһ�ֽ����ݺ���               
����ԭ��: int  IRcvByte(uchar sla,ucahr *c);  
����:     ���������ߵ����͵�ַ�������ݣ��������ߵ�ȫ����,��������
          ַsla������ֵ��c.
          �������1��ʾ�����ɹ��������������
ע�⣺    ʹ��ǰ�����ѽ������ߡ�
********************************************************************/
int IRcvByte(uchar sla,uchar *c)
{
  Start_I2c();          //��������
  SendByte(sla+1);      //����������ַ
  if(ack==0)
     return(0);

  *c=RcvByte();         //��������
  Ack_I2c(1);           //�����꣬���ͷ�Ӧ��λ����������
  Stop_I2c();           //�������� 
  return(1);
}

/*******************************************************************
                    �����ӵ�ַ������ȡ���ֽ����ݺ���               
����ԭ��: int  ISendStr(uchar sla,uchar suba,uchar *s,uchar no);  
����:     ���������ߵ����͵�ַ���ӵ�ַ,�����ݣ��������ߵ�ȫ����,������
          ��ַsla���ӵ�ַsuba�����������ݷ���sָ��Ĵ洢������no���ֽڡ�
           �������1��ʾ�����ɹ��������������
ע�⣺    ʹ��ǰ�����ѽ������ߡ�
********************************************************************/
int IRcvStr(uchar sla,uchar suba,uchar *s,uchar no)
{
  uchar i;

  Start_I2c();          //��������
  SendByte(sla);        //����������ַ
  if(ack==0)
     return(0);

  SendByte(suba);       //���������ӵ�ַ
  if(ack==0)
     return(0);



  Start_I2c();		//������������
  SendByte(sla+1);
  if(ack==0)
     return(0);




  for(i=0;i<no-1;i++)   //�Ƚ���ǰ(no-1)�ֽ�
   {   
     *s=RcvByte();      //��������
     Ack_I2c(0);        //��δ�����꣬����Ӧ��λ  
     s++;
   } 
   *s=RcvByte();        //���յ�no�ֽ�
   Ack_I2c(1);          //�����꣬���ͷ�Ӧ��λ
   Stop_I2c();          //�������� 
   return(1);
}
/*******************************************************************
                    �����ӵ�ַ������ȡ���ֽ����ݺ���               
����ԭ��: int  ISendStrExt(uchar sla,uchar *s,uchar no);  
����:     ���������ߵ����͵�ַ,������,�������ߵ�ȫ����.
          ��������ַsla�����������ݷ���sָ��Ĵ洢����
          ��no���ֽڡ��������1��ʾ�����ɹ��������������
ע�⣺    ʹ��ǰ�����ѽ������ߡ�
********************************************************************/
int IRcvStrExt(uchar sla,uchar *s,uchar no)
{
  uchar i;

  Start_I2c();
  SendByte(sla+1);	//R/Wѡ��λ��Ϊ1ʱΪ���� Ϊ0 ʱΪд
  if(ack==0)return(0);

  for(i=0;i<no-1;i++)   //�Ƚ���ǰ��no-1)���ֽ�
   {   
     *s=RcvByte();      //��������
     Ack_I2c(0);        //δ��ȡ�꣬����Ӧ��λ  
     s++;
   } 
   *s=RcvByte();        //���յ�no�ֽ�
   Ack_I2c(1);          //�����꣬���ͷ�Ӧ��λ
   Stop_I2c();          //�������� 
   return(1);
}
//*******************************************************************

