/////////////////////////////////////////////////////////////////////////////////////////////
//文件名：		CBNSumProduct_Query_Helper.cpp
//功  能：		查询的辅助函数
//开发者：		高志强
//日  期：		2021年03月14日
//更  新：		2021年03月24日
//长  度：		70行
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"									//MFC标准头文件
#include "CBNSumProduct.h"							//贝叶斯网络精确推理类头文件，采用和积变量消除法
#include "Helper.h"									//辅助函数头文件


//名  称：		Query_Helper()
//功  能：		查询的辅助函数
//参  数：		QUERY&,vector<CFactor>
//返回值：		无
void CBNSumProduct::Query_Helper(QUERY& query, vector<CFactor> Factors)
{
	//////////////////////////////////////////////////////////////////////////////
	//步骤1：根据给定变量、规约因子列表
	//遍历所有给定变量ID
	for (unsigned int i = 0; i < query.GivenVariables.size(); i++)
	{
		//遍历所有因子
		for (unsigned int j = 0; j <Factors.size(); j++)
		{
			//规约因子：
			Factors[j].ReduceGivenVariable(query.GivenVariables[i].nNodeID, query.GivenVariables[i].nValueID);
		}
	}


	/////////////////////////////////////////////////////////////////////////////
	//步骤2：计算边缘概率
	Query_Marginal(query,Factors);
}