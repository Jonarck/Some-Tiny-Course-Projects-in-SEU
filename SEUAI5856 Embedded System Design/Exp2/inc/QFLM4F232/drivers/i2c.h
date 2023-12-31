
********************************************************************************************************/
//#ifndef  i2c_H
//#define  i2c_H

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef uchar 
#define uchar unsigned char
#endif


/*******************************************************************
                     无子地址发送字节数据函数               

功能:     从启动总线到发送地址，数据，结束总线的全过程,从器件地址sla.
          如果返回1表示操作成功，否则操作有误。
********************************************************************/
extern int ISendByte(uchar sla,uchar c);
 




/*******************************************************************
                     有子地址发送多字节数据函数               

功能:     从启动总线到发送地址，子地址,数据，结束总线的全过程,从器件
          地址sla，子地址suba，发送内容是s指向的内容，发送no个字节。
          如果返回1表示操作成功，否则操作有误。
********************************************************************/
extern int ISendStr(uchar sla,uchar suba,uchar *s,uchar no) ;



/*******************************************************************
                    无子地址发送多字节数据函数   
             
功能:     从启动总线到发送地址，子地址,数据，结束总线的全过程,从器件
          地址sla，发送内容是s指向的内容，发送no个字节。
          如果返回1表示操作成功，否则操作有误。
********************************************************************/
extern int ISendStrExt(uchar sla,uchar *s,uchar no);



/*******************************************************************
                    无子地址读字节数据函数               

功能:     从启动总线到发送地址，读数据，结束总线的全过程,从器件地
          址sla，返回值在c.
          如果返回1表示操作成功，否则操作有误。
********************************************************************/
extern int IRcvByte(uchar sla,uchar *c);





/*******************************************************************
                    有子地址读取多字节数据函数               
   
功能:     从启动总线到发送地址，子地址,读数据，结束总线的全过程,从器件
          地址sla，子地址suba，读出的内容放入s指向的存储区，读no个字节。
          如果返回1表示操作成功，否则操作有误。
********************************************************************/
extern int IRcvStr(uchar sla,uchar suba,uchar *s,uchar no);



/*******************************************************************
                    无子地址读取多字节数据函数               
  
功能:     从启动总线到发送地址,读数据,结束总线的全过程.
          从器件地址sla，读出的内容放入s指向的存储区，
          读no个字节。如果返回1表示操作成功，否则操作有误。
********************************************************************/
extern int IRcvStrExt(uchar sla,uchar *s,uchar no);


#ifdef __cplusplus
}
#endif

#endif    // __I2C_H__
