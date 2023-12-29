/////////////////////////////////////////////////////////////////////////////////////////////
//文件名：		CBNSumProduct_Query.cpp
//功  能：		查询
//开发者：		高志强
//日  期：		2021年03月24日
//更  新：		
//长  度：		70行
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"									//MFC标准头文件
#include "CBNSumProduct.h"							//贝叶斯网络精确推理类头文件，采用和积变量消除法
#include "Helper.h"									//辅助函数头文件


//名  称：		Query()
//功  能：		查询
//参  数：		无
//返回值：		无
void CBNSumProduct::Query()
{
	//遍历所有查询进行推理
	for (unsigned int i = 0; i < m_Queries.size(); i++)
	{
		//Preprocess_Query(m_Queries[i]);
		
		//调用查询的辅助函数
		Query_Helper(m_Queries[i], m_Factors);
	}

	//输出查询结果
	OutputToXML();
}