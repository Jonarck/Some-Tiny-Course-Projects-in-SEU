/////////////////////////////////////////////////////////////////////////////////////////////
//文件名：		ExactInferenceDlg_User.cpp
//功  能：		响应用户命令
//开发者：		高志强
//日  期：		2021年02月20日
//更  新：		
//长  度：		70行
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"					//MFC头文件
#include "ExactInference.h"			//应用程序类头文件
#include "ExactInferenceDlg.h"		//主对话框类头文件
#include "CBNSumProduct.h"			//贝叶斯网络精确推理类头文件。采用和积变量消除法


//名  称：		OnBnClickedButtonBnInit()
//功  能：		响应初始化命令
//参  数：		无
//返回值：		无
void CExactInferenceDlg::OnBnClickedButtonBnInit()
{
	//定义贝叶斯网络对象
	CBNSumProduct theBN;
	
	//初始化
	theBN.Init();
	//提示
	AfxMessageBox(_T("完成读取贝叶斯网络的结构和参数，并开始推理...。请按确定键继续"));
	
	//查询
	theBN.Query();

}

