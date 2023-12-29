/////////////////////////////////////////////////////////////////////////////////////////////
//文件名：		CBNSumProduct_Preprocess_Factor.cpp
//功  能：		预处理因子
//开发者：		高志强
//日  期：		2021年03月13日
//更  新：		2021年03月23日
//长  度：		70行
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"									//MFC标准头文件
#include "CBNSumProduct.h"							//贝叶斯网络精确推理类头文件，采用和积变量消除法
#include "Helper.h"									//辅助函数头文件


//名  称：		Preprocess_Factor()
//功  能：		预处理因子
//参  数：		无
//返回值：		无
void CBNSumProduct::Preprocess_Factor()
{
	//遍历BN中的所有节点
	for (unsigned int i = 0; i < m_Nodes.size(); i++)
	{
		//定义因子
		CFactor factor;

		//////////////////////////////////////////////////////////////
		//步骤1：初始化因子。设置因子的变量ID的列表
		vector<unsigned int>NodeIDs= m_Nodes[i].ParentIDs;	//父节点ID列表
		//添加本身节点的ID
		NodeIDs.push_back(m_Nodes[i].nID);
		//初始化因子
		factor.SetFactorVariableIDs(NodeIDs);


		//////////////////////////////////////////////////////////////
		//步骤2：初始化因子。设置因子值的ID表
		for (unsigned int j = 0; j < m_Nodes[i].CPTRowIDs.size(); j++)
		{
			//获取值ID的表
			vector<unsigned int> ValueIDs=m_Nodes[i].CPTRowIDs[j].ValueIDs;
			//获取因子值
			double fValue = m_Nodes[i].CPTRowIDs[j].fProb;

			//设置因子的行
			factor.SetFactorRow(ValueIDs, fValue);
		}
		
		//添加到因子集合
		m_Factors.push_back(factor);
	}


	//////////////////////////////////////////////////////////////////////////////
	//可以注释掉，主要用于测试因子运算。包括因子积、因子化简、因子求和掉部分变量等	
	//因子积
	//CFactor new_factor = m_Factors[0] * m_Factors[2];

	//因子化简
	//new_factor.ReduceGivenVariable(0, 0);

	//因子和
	//new_factor.SumVariable(0);
	///////////////////////////////////////////////////////////////////////////////
}