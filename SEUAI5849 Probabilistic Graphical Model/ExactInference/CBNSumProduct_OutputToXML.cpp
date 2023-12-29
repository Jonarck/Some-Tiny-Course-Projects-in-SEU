/////////////////////////////////////////////////////////////////////////////////////////////
//文件名：		CBNSumProduct_OutputToXML.cpp
//功  能：		输出查询结果到XML文件
//开发者：		高志强
//日  期：		2021年03月10日
//更  新：		2021年03月24日
//长  度：		70行
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"									//MFC标准头文件
#include "ExactInference.h"							//应用程序类头文件
#include "CBNSumProduct.h"							//贝叶斯网络精确推理类头文件，采用和积变量消除法
#include "tinyxml.h"								//TinyXML头文件
#include "Helper.h"									//辅助函数头文件


//声明外部变量
extern CExactInferenceApp theApp;


//名  称：		OutputJointToXML()
//功  能：		输出查询结果到XML文件
//参  数：		无
//返回值：		无
void CBNSumProduct::OutputToXML()
{
	//生成XML文档
	TiXmlDocument doc;
	//设置文档属性
	TiXmlDeclaration* pDecl = new TiXmlDeclaration("1.0", "GBK", "");
	//连接描述信息到文档
	doc.LinkEndChild(pDecl);

	//创建根元素
	TiXmlElement *pRootElement = new TiXmlElement("Probilities");
	//连接根元素
	doc.LinkEndChild(pRootElement);

	
	//遍历所有查询结果
	for (unsigned int i = 0; i < m_QueryResults.size(); i++)
	{
		//创建概率元素
		TiXmlElement* pProbability = new TiXmlElement("Probability");

		//设置概率取值
		char sTemp[MAX_PATH];
		sprintf_s(sTemp, "%.6f", m_QueryResults[i]);
		pProbability->SetAttribute("PROBABILITY_VALUE", sTemp);
		

		//连接概率到概率列表
		pRootElement->LinkEndChild(pProbability);
	}

	//获取当前工作路径
	CString sFileName = theApp.m_sWorkPath;
	sFileName = sFileName + _T("\\Data");
	sFileName = sFileName + _T("\\Output.xml");

	//保存XML文件
	doc.SaveFile((MapCStringToString(sFileName)).c_str());

	//释放内存
	doc.Clear();

	//自动打开文件
	ShellExecute(NULL, _T("open"), sFileName, NULL, NULL, SW_SHOWNORMAL);
}