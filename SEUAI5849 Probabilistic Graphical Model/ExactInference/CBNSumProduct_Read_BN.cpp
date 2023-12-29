/////////////////////////////////////////////////////////////////////////////////////////////
//文件名：		CBNSumProduct_Read_BN.cpp
//功  能：		读取贝叶斯网络结构和参数
//开发者：		高志强
//日  期：		2021年03月01日
//更  新：		2021年03月06日
//更  新：		2021年03月08日
//更  新：		2021年03月23日
//长  度：		170行
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"									//MFC标准头文件
#include "ExactInference.h"							//应用程序类头文件
#include "CBNSumProduct.h"							//贝叶斯网络精确推理类头文件，采用和积变量消除法
#include "Helper.h"									//辅助函数头文件


//声明外部变量
extern CExactInferenceApp theApp;


//名  称：		Read_BN()
//功  能：		读取贝叶斯网络结构和参数
//参  数：		无
//返回值：		无
void CBNSumProduct::Read_BN()
{
	//获取当前工作路径
	CString sFileName = CString(theApp.m_sWorkPath);
	sFileName = sFileName + _T("\\Data");
	sFileName = sFileName + _T("\\BayesianNetwork.xml");

	
	//搜索文件，如果文件不存在，则直接返回
	CFileFind findWenJian;								//文件名称
	BOOL bWenJian = findWenJian.FindFile(sFileName);	//搜索文件是否存在
	//如果没有发现文件，则直接返回
	if (!bWenJian)
	{
		//提示
		AfxMessageBox(_T("贝叶斯网络结构和参数文件BayesianNetwork.xml不存在"));
		
		//返回
		return;
	}

	
	//打开文件
	TiXmlDocument aDoc(MapCStringToString(sFileName).c_str());

	//检查打开文件是否成功
	if (!aDoc.LoadFile())
	{
		//提示
		AfxMessageBox(_T("打开BayesianNetwork_Part.xml失败:"));

		//退出
		return exit(0);
	}

	//获取根结点
	TiXmlElement *pRoot = aDoc.RootElement();


	////////////////////////////////////////////////////////////////////////////////////////////////
	//步骤1：获取节点表
	TiXmlElement* pNodes = pRoot->FirstChildElement();
	if (pNodes != NULL)
	{
		//获取节点
		TiXmlElement* pNode = pNodes->FirstChildElement();
		//遍历所有节点
		while (pNode != NULL)
		{
			//获取节点属性
			BN_NODE bn_node;
			//01 节点ID
			string sTemp = GetAttribute(pNode, "ID");
			bn_node.nID = TransformStringToInt(sTemp);
			//02 节点名称
			bn_node.sName = GetAttribute(pNode, "NAME");			
			//03 节点名称简写
			bn_node.sAbbreviation = GetAttribute(pNode, "ABBREVIATION");
			//04 节点值的个数
			sTemp = GetAttribute(pNode, "NUMBER_OF_VALUES");
			bn_node.nNumberOfValues=TransformStringToInt(sTemp);
			//05 父节点个数
			sTemp = GetAttribute(pNode, "NUMBER_OF_PARENTS");
			bn_node.nNumberOfParents = TransformStringToInt(sTemp);

			//06 父节点列表
			if (bn_node.nNumberOfParents != 0)
			{
				sTemp = GetAttribute(pNode, "PARENT_IDS");
				vector<string> IDs;
				//获取双亲ID列表
				Separate(sTemp, IDs);
				//转化为无符号整数
				for (unsigned int i = 0; i < IDs.size(); i++)
				{
					//获取整数
					unsigned int nID = TransformStringToInt(IDs[i]);

					//加入父节点ID列表
					bn_node.ParentIDs.push_back(nID);
				}
			}
			
			//07 CPT中的概率值
			sTemp = GetAttribute(pNode, "CPT");
			vector<string> Probabilities;
			//切分
			Separate(sTemp, Probabilities);
			//转化为双精度数
			for (unsigned int i = 0; i < Probabilities.size(); i++)
			{
				//获取双精度数
				double fProb = TransformStringToDouble(Probabilities[i]);

				//加入CPT每行的值
				bn_node.CPTRowValues.push_back(fProb);
			}

			//添加到节点表
			m_Nodes.push_back(bn_node);


			//更新节点
			pNode = pNode->NextSiblingElement();
		}
	}


	///////////////////////////////////////////////////////////////////////////////////////////////
	//步骤2：获取边表
	TiXmlElement* pEdges = pNodes->NextSiblingElement();
	if (pEdges != NULL)
	{
		TiXmlElement* pEdge = pEdges->FirstChildElement();

		//检查指针是否为空
		while (pEdge != NULL)
		{
			//获取边属性
			BN_EDGE bn_edge;
			//01 边ID
			string sTemp = GetAttribute(pEdge, "ID");
			bn_edge.nID = TransformStringToInt(sTemp);
			//02 边的起点ID
			sTemp = GetAttribute(pEdge, "START_NODE_ID");
			bn_edge.nStartNodeID = TransformStringToInt(sTemp);
			//03 边的终点ID
			sTemp = GetAttribute(pEdge, "END_NODE_ID");
			bn_edge.nEndNodeID = TransformStringToInt(sTemp);

			//添加到边表
			m_Edges.push_back(bn_edge);


			//更新边指针
			pEdge = pEdge->NextSiblingElement();
		}
	}
	
	
	//释放文件所占内存
	aDoc.Clear();
}