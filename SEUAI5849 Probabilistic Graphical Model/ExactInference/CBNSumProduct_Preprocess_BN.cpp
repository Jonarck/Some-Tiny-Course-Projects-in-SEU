/////////////////////////////////////////////////////////////////////////////////////////////
//文件名：		CBNSumProduct_Preprocess_BN.cpp
//功  能：		预处理贝叶斯网络结构与参数
//开发者：		高志强
//日  期：		2021年03月09日
//更  新：		2021年03月23日
//长  度：		90行
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"									//MFC标准头文件
#include "CBNSumProduct.h"							//贝叶斯网络精确推理类头文件，采用和积变量消除法


//名  称：		Preprocess_BN()
//功  能：		预处理贝叶斯网络结构与参数
//参  数：		无
//返回值：		无
void CBNSumProduct::Preprocess_BN()
{
	//遍历所有节点，整理条件概率分布CPT
	for (unsigned int i = 0; i < m_Nodes.size(); i++)
	{
		//构造节点的父节点ID和取值的排列
		vector<unsigned int> NodeIDs = m_Nodes[i].ParentIDs;
		//添加当前节点ID，以便形成因子
		NodeIDs.push_back(m_Nodes[i].nID);

		//定义父节点取值
		unsigned int nRow = 0;									//从CPT表的第0行开始
		vector<unsigned int> ValueIDs;							//值ID的表。和给定的节点ID顺序相同
		Arrange(m_Nodes[i].nID, ValueIDs, NodeIDs, 0, nRow);	//0表示变量值的起始位置。
	}
}

//名  称：		Arrange()
//功  能：		对父节点的ID及其取值进行排列
//参  数：		unsigned int,vector<unsigned int>,vector<unsigned int>,unsigned int,unsigned int&
//返回值：		无
void CBNSumProduct::Arrange(unsigned int nNodeID,		//当前节点ID
						vector<unsigned int> ValueIDs,  //变量值ID的排列
						vector<unsigned int> NodeIDs,	//节点ID排列
						unsigned int nPos,				//当前节点ID位置，从第0列开始
						unsigned int& nRow)				//当前行号
{
	//检查递归是否结束
	if (nPos == NodeIDs.size())
	{
		//获取节点值表
		CPT_ROW cpt_row;
		cpt_row.ValueIDs = ValueIDs;
		cpt_row.fProb = m_Nodes[nNodeID].CPTRowValues[nRow];

		//添加到节点的CPT
		m_Nodes[nNodeID].CPTRowIDs.push_back(cpt_row);

		//更新行号
		nRow++;

		//返回
		return;
	}

	//取首节点
	unsigned int nCurrID = NodeIDs[nPos];

	//获取其值的个数
	unsigned int nNumberOfValues = m_Nodes[nCurrID].nNumberOfValues;
	//遍历所有节点值
	for (unsigned int i = 0; i < nNumberOfValues; i++)
	{
		//定义新的值ID列表
		vector<unsigned int> NewValueIDs = ValueIDs;
		//添加值
		NewValueIDs.push_back(i);
		
		//定义新的位置
		unsigned int nNewPos = nPos + 1;

		//递归调用
		Arrange(nNodeID, NewValueIDs, NodeIDs, nNewPos, nRow);
	}
}