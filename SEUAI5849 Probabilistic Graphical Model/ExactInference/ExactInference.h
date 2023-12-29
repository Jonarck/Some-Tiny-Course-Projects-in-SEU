/////////////////////////////////////////////////////////////////////////////////////////////
//文件名：		ExactInference.h
//功  能：		定义应用程序类
//开发者：		高志强
//日  期：		2021年03月01日
//更  新：		
//长  度：		40行
/////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// 定义应用程序类CExactInferenceApp: 
class CExactInferenceApp : public CWinApp
{
public:
	CExactInferenceApp();			//构造函数

// 重写
public:
	virtual BOOL InitInstance();	//初始化应用程序实例

// 实现

	DECLARE_MESSAGE_MAP()			//开始消息映射

// 公有数据成员
public:
	CString m_sWorkPath;			//工作路径
};

extern CExactInferenceApp theApp;	//唯一的应用程序对象