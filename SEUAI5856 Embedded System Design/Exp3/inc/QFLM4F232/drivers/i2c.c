
********************************************************************************************************/
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_types.h"
#include "hw_gpio.h"
#include "interrupt.h"
#include "gpio.h"

// 宏定义
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

// I2C引脚的定义。
#define SDA  GPIO_PIN_3         //模拟I2C数据传送位
#define SCL  GPIO_PIN_2         //模拟I2C时钟控制位

int ack;	                //应答标志位ack

/*******************************************************************
                     起动总线函数               
函数原型: void  Start_I2c();  
功能:     启动I2C总线,即发送I2C起始条件
          起始条件为：在SCL线是高电平时,SDA线由高电平向低电平切换   
********************************************************************/
void Start_I2c()
{
  //发送起始条件的数据信号
  HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SDA << 2))) = SDA;   // SDA = 1;
  delay(6);
  //发送起始条件的时钟信号
  HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SCL << 2))) = SCL;   // SCL = 1;
  delay(30);    	        //起始条件建立时间大于4.7us,延时
  //发送起始信号
  HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SDA << 2))) = ~SDA;   // SDA = 0;
  delay(30);    	        //起始条件锁定时间大于4.7us
  //钳住I2C总线，准备发送或接收数据 
  HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SCL << 2))) = ~SCL;   // SCL = 0;
  delay(12);
}

/*******************************************************************
                       结束总线函数               
函数原型: void  Stop_I2c();  
功能:     结束I2C总线,即发送I2C结束条件.
          停止条件为：在SCL线是高电平时,SDA线由低电平向高电平切换
********************************************************************/
void Stop_I2c()
{
  HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SDA << 2))) = ~SDA;  	        //发送结束条件的数据信号
  delay(6);   	        
  HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SCL << 2))) = SCL;  	        //发送结束条件的时钟信号
  delay(30);               //结束条件建立时间大于4μs
  HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SDA << 2))) = SDA;  	        //发送I2C总线结束信号
  delay(24);
}
/*******************************************************************
                 字节数据发送函数               
函数原型: void  SendByte(uchar c);
功能:     将数据c发送出去,可以是地址,也可以是数据,发完后等待应答,并对
          此状态位进行操作.(不应答或非应答都使ack=0)     
     	  发送数据正常，ack=1; ack=0表示被控器无应答或损坏。
********************************************************************/
void  SendByte(uchar c)
{
 uchar BitCnt;
 
 //要传送的数据长度为8位,
 for(BitCnt=0;BitCnt<8;BitCnt++)  
   {
     //判断发送位  
     if((c<<BitCnt)&0x80)   
       {
        HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SDA << 2))) = SDA;   
       }
       else  
         {
          HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SDA << 2))) = ~SDA;
         }
        
     delay(6);
     HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SCL << 2))) = SCL;      //置时钟线为高，通知被控器开始接收数据位
     delay(30);                                                     //保证时钟高电平周期大于4μs
     HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SCL << 2))) = ~SCL; 
   }
    
  delay(12);
  HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SDA << 2))) = SDA;         //8位发送完后释放数据线，准备接收应答位
  delay(12);
  HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SCL << 2))) = SCL;
  delay(18);
  if(!HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SDA << 2))))                  //判断是否接收到应答信号
       ack=0;
      else 
       ack=1;

  HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SCL << 2))) = ~SCL;
  delay(12);
}

/*******************************************************************
                 字节数据接收函数               
函数原型: uchar  RcvByte();
功能:  	  用来接收从器件传来的数据,并判断总线错误(不发应答信号)，
          发完后请用应答函数应答从机。  
********************************************************************/	
uchar  RcvByte()
{
  uchar retc;
  uchar BitCnt;
  
  retc=0; 
  HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SDA << 2))) = SDA;         
  GPIODirModeSet(GPIO_PORTB_BASE, SDA,GPIO_DIR_MODE_IN);             //置数据线为输入方式
  for(BitCnt=0;BitCnt<8;BitCnt++)
    {
     delay(6);           
     HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SCL << 2))) = ~SCL;     //置时钟线为低，准备接收数据位
     delay(30);                                                      //时钟低电平周期大于4.7μ
     HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SCL << 2))) = SCL;      //置时钟线为高使数据线上数据有效
     delay(12);
     retc=retc<<1;

     if(HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SDA << 2))))         //读数据位,接收的数据位放入retc中 
         retc=retc+1;
 
     delay(12);
    }
  GPIODirModeSet(GPIO_PORTB_BASE, SDA,GPIO_DIR_MODE_OUT);            // 置数据线为输出方式
  HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SCL << 2))) = ~SCL;    
  delay(12);
  return(retc);
}

/********************************************************************
                     应答子函数
函数原型:  void Ack_I2c(bit a);
功能:      主控器进行应答信号(可以是应答或非应答信号，由位参数a决定)
********************************************************************/
void Ack_I2c(int a)
{
  if(a==0)
     HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SDA << 2))) = ~SDA;     //在此发出应答或非应答信号 
    else 
      HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SDA << 2))) = SDA;

  delay(18);
  HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SCL << 2))) = SCL;
  delay(30);                                                         //时钟低电平周期大于4μs  
  HWREG(GPIO_PORTB_BASE + (GPIO_O_DATA + (SCL << 2))) = ~SCL;        //清时钟线，钳住I2C总线以便继续接收
  delay(12);
}

/*******************************************************************
                 用户接口函数                                   
*******************************************************************/


/*******************************************************************
                 向无子地址器件发送字节数据函数               
函数原型: int  ISendByte(uchar sla,uchar c);  
功能:     从启动总线到发送地址，数据，结束总线的全过程,从器件地址sla.
          如果返回1表示操作成功，否则操作有误。
注意：    使用前必须已结束总线。
********************************************************************/
int ISendByte(uchar sla,uchar c)
{
  Start_I2c();          //启动总线
  SendByte(sla);        //发送器件地址
  if(ack==0)
     return(0);

  SendByte(c);          //发送数据
  if(ack==0)            
     return(0);

  Stop_I2c();           //结束总线 
  return(1);
}

/*******************************************************************
                    向有子地址器件发送多字节数据函数               
函数原型: int  ISendStr(uchar sla,uchar suba,uchar *s,uchar no);  
功能:     从启动总线到发送地址，子地址,数据，结束总线的全过程,从器件
          地址sla，子地址suba，发送内容是s指向的内容，发送no个字节。
          如果返回1表示操作成功，否则操作有误。
注意：    使用前必须已结束总线。
********************************************************************/
int ISendStr(uchar sla,uchar suba,uchar *s,uchar no)
{
  uchar i;

  Start_I2c();          //启动总线
  SendByte(sla);        //发送器件地址
  if(ack==0)           
    return(0);

  SendByte(suba);       //发送器件子地址
  if(ack==0)
    return(0);

  for(i=0;i<no;i++)
    {   
     SendByte(*s);      //发送数据
     if(ack==0)
       return(0);
     s++;
    } 
  Stop_I2c();           //结束总线
  return(1);
}
/*******************************************************************
                    向无子地址器件发送多字节数据函数               
函数原型: int  ISendStrExt(uchar sla,uchar *s,uchar no);  
功能:     从启动总线到发送地址，子地址,数据，结束总线的全过程,从器件
          地址sla，发送内容是s指向的内容，发送no个字节。
          如果返回1表示操作成功，否则操作有误。
注意：    使用前必须已结束总线。
********************************************************************/
int ISendStrExt(uchar sla,uchar *s,uchar no)
{
  uchar i;

  Start_I2c();          //启动总线
  SendByte(sla);        //发送器件地址
  if(ack==0)
    return(0);

  for(i=0;i<no;i++)
    {   
     SendByte(*s);      //发送数据
     if(ack==0)
       return(0);
     s++;
    } 
  Stop_I2c();           //结束总线 
  return(1);
}

/*******************************************************************
                    向无子地址器件读取一字节数据函数               
函数原型: int  IRcvByte(uchar sla,ucahr *c);  
功能:     从启动总线到发送地址，读数据，结束总线的全过程,从器件地
          址sla，返回值在c.
          如果返回1表示操作成功，否则操作有误。
注意：    使用前必须已结束总线。
********************************************************************/
int IRcvByte(uchar sla,uchar *c)
{
  Start_I2c();          //启动总线
  SendByte(sla+1);      //发送器件地址
  if(ack==0)
     return(0);

  *c=RcvByte();         //接收数据
  Ack_I2c(1);           //接收完，发送非应答位，结束总线
  Stop_I2c();           //结束总线 
  return(1);
}

/*******************************************************************
                    向有子地址器件读取多字节数据函数               
函数原型: int  ISendStr(uchar sla,uchar suba,uchar *s,uchar no);  
功能:     从启动总线到发送地址，子地址,读数据，结束总线的全过程,从器件
          地址sla，子地址suba，读出的内容放入s指向的存储区，读no个字节。
           如果返回1表示操作成功，否则操作有误。
注意：    使用前必须已结束总线。
********************************************************************/
int IRcvStr(uchar sla,uchar suba,uchar *s,uchar no)
{
  uchar i;

  Start_I2c();          //启动总线
  SendByte(sla);        //发送器件地址
  if(ack==0)
     return(0);

  SendByte(suba);       //发送器件子地址
  if(ack==0)
     return(0);



  Start_I2c();		//重新启动总线
  SendByte(sla+1);
  if(ack==0)
     return(0);




  for(i=0;i<no-1;i++)   //先接收前(no-1)字节
   {   
     *s=RcvByte();      //接收数据
     Ack_I2c(0);        //还未接收完，发送应答位  
     s++;
   } 
   *s=RcvByte();        //接收第no字节
   Ack_I2c(1);          //接收完，发送非应答位
   Stop_I2c();          //结束总线 
   return(1);
}
/*******************************************************************
                    向无子地址器件读取多字节数据函数               
函数原型: int  ISendStrExt(uchar sla,uchar *s,uchar no);  
功能:     从启动总线到发送地址,读数据,结束总线的全过程.
          从器件地址sla，读出的内容放入s指向的存储区，
          读no个字节。如果返回1表示操作成功，否则操作有误。
注意：    使用前必须已结束总线。
********************************************************************/
int IRcvStrExt(uchar sla,uchar *s,uchar no)
{
  uchar i;

  Start_I2c();
  SendByte(sla+1);	//R/W选择位，为1时为读， 为0 时为写
  if(ack==0)return(0);

  for(i=0;i<no-1;i++)   //先接收前（no-1)个字节
   {   
     *s=RcvByte();      //接收数据
     Ack_I2c(0);        //未读取完，发送应答位  
     s++;
   } 
   *s=RcvByte();        //接收第no字节
   Ack_I2c(1);          //接收完，发送非应答位
   Stop_I2c();          //结束总线 
   return(1);
}
//*******************************************************************

