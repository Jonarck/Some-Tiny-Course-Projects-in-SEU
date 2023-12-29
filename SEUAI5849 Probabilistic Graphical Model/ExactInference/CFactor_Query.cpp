/////////////////////////////////////////////////////////////////////////////////////////////
//文件名：		CFactor_Query.cpp
//功  能：		查询因子中的特定概率。例如因子中包括A、B、C三个变量，只想获取A、B、C特定取值的概率等。需要给定变量ID列表和变量值的ID列表
//开发者：		高志强
//日  期：		2021年03月15日
//更  新：		2021年03月23日
//更  新：		2021年03月29日
//长  度：		80行
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"									//MFC标准头文件
#include "CFactor.h"								//因子类头文件
#include "Helper.h"									//辅助函数头文件


//名  称：		Query()
//功  能：		查询特定值
//参  数：		vector<unsigned int>&,vector<unsigned int>&
//返回值：		double
double CFactor::Query(vector<unsigned int>& VariableIDs,vector<unsigned int>& ValueIDs)
{
	//定义位置集合
	vector<unsigned int> Positions;
	
	//遍历查询的变量ID
	for (unsigned int i = 0; i < VariableIDs.size(); i++)
	{
		//遍历因子的变量ID
		for (unsigned int j = 0; j < m_VariableIDs.size(); j++)
		{
			//检查变量ID是否相同
			if (VariableIDs[i] == m_VariableIDs[j])
			{
				//添加到位置集合
				Positions.push_back(j);
			}
		}
	}

	
	//初始化返回的概率值
	double fProb = 0.0f;
	
	//遍历因子行，求和
	for (unsigned int i = 0; i < m_FactorRows.size(); i++)
	{
		//定义因子行
		FACTOR_ROW factor_row = m_FactorRows[i];
		
		//检查位置和值是否正确
		bool bMatch = true;
		
		//遍历所有位置
		for (unsigned int j = 0; j < Positions.size(); j++)
		{
			//获取位置
			unsigned int nPos=Positions[j];		//获取位置
			unsigned int nValue = ValueIDs[j];	//获取变量值的ID
			//如果不相等，则更新匹配结果
			if (factor_row.ValueIDs[nPos] != nValue)
			{
				//更新匹配结果。这里似乎可以终止循环
				bMatch = false;
			}
		}

		//检查该行是否匹配。如果匹配的话，则需要累加概率值
		if (bMatch == true)
		{
			//累计概率值
			fProb += factor_row.fValue;
		}
	}

	//返回概率
	return fProb;
}

//名  称：		GetFactorVariableIDs()
//功  能：		获取变量ID列表
//参  数：		无
//返回值：		unsigned int
vector<unsigned int> CFactor::GetFactorVariableIDs()
{
	//返回变量ID列表
	return m_VariableIDs;
}