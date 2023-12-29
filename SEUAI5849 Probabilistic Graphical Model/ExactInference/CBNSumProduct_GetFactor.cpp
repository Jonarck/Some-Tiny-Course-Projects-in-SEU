/////////////////////////////////////////////////////////////////////////////////////////////
//文件名：		CBNSumProduct_GetFactor.cpp
//功  能：		获取因子值
//开发者：		高志强
//日  期：		2021年03月09日
//更  新：		2021年03月24日
//长  度：		100行
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"									//MFC标准头文件
#include "CBNSumProduct.h"							//贝叶斯网络精确推理类头文件，采用和积变量消除法


//名  称：		GetFactor()
//功  能：		计算因子的值
//参  数：		unsigned int,unsigned int,vector<QUERY_VARIABLE>&
//返回值：		无
double CBNSumProduct::GetFactor(unsigned int nNodeID,						//节点ID
								unsigned int nValueID,						//节点值ID
								vector<GROUNDING_VARIABLE>& QueryVariables)	//查询变量列表
{
	//获取节点
	BN_NODE bn_node = m_Nodes[nNodeID];

	//定义、并初始化获取父节点ID列表
	vector<unsigned int> NodeIDs = bn_node.ParentIDs;

	//定义值ID列表
	vector<unsigned int> ValueIDs;

	//遍历父节点ID，获取其值ID
	for (unsigned int i = 0; i < NodeIDs.size(); i++)
	{
		//获取新的节点ID
		unsigned int nNewNodeID = NodeIDs[i];
		//定义新的值ID
		unsigned int nNewValueID = GetValueID(nNewNodeID, QueryVariables);

		//添加到值ID表
		ValueIDs.push_back(nNewValueID);
	}

	//添加当前节点的值ID
	ValueIDs.push_back(nValueID);


	//在当前节点的CPT中查找
	vector<CPT_ROW> CPTRows = m_Nodes[nNodeID].CPTRowIDs;
	//遍历所有行
	for (unsigned int i = 0; i < CPTRows.size(); i++)
	{
		//检查该行是否存在
		if (CPTRows[i].ValueIDs == ValueIDs)
		{
			//定义、并获取概率
			double fProb = m_Nodes[nNodeID].CPTRowValues[i];

			//返回概率
			return fProb;
		}
	}


	//提示异常
	AfxMessageBox(_T("GetFactor()异常"));
	
	//缺省、返回1.0
	return 1.0f;
}

//名  称：		GetValueID()
//功  能：		获取值ID
//参  数：		unsigned int,vector<QUERY_VARIABLE>&
//返回值：		unsigned int
unsigned int CBNSumProduct::GetValueID(unsigned int nNodeID,						//节点ID
										vector<GROUNDING_VARIABLE>& QueryVariables)	//查询变量列表
{
	//遍历所有查询变量
	for (unsigned int i = 0; i < QueryVariables.size(); i++)
	{
		//检查是否找到给定的节点ID
		if (nNodeID == QueryVariables[i].nNodeID)
		{
			//如果找到，则返回相应值ID
			return QueryVariables[i].nValueID;
		}
	}

	//提示异常
	AfxMessageBox(_T("GetValueID()异常"));
	//缺省返回0
	return 0;
}