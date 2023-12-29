/////////////////////////////////////////////////////////////////////////////////////////////
//文件名：		CBNSumProduct_Read_Query.cpp
//功  能：		读取查询任务。包括查询节点、给定节点、节点删除顺序
//开发者：		高志强
//日  期：		2021年03月06日
//更  新：		2021年03月08日
//更  新：		2021年03月23日
//长  度：		180行
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"									//MFC标准头文件
#include "ExactInference.h"							//应用程序类头文件
#include "CBNSumProduct.h"							//贝叶斯网络精确推理类头文件，采用和积变量消除法
#include "Helper.h"									//辅助函数头文件


//声明外部变量
extern CExactInferenceApp theApp;


//名  称：		Read_Query()
//功  能：		读取查询任务。包括查询节点、给定节点、删除节点顺序
//参  数：		无
//返回值：		无
void CBNSumProduct::Read_Query()
{
	//获取当前工作路径
	CString sFileName = CString(theApp.m_sWorkPath);
	sFileName = sFileName + _T("\\Data");
	sFileName = sFileName + _T("\\BayesianNetwork_Query.xml");


	//搜索文件，如果文件不存在，则直接返回
	CFileFind findWenJian;								//文件名称
	BOOL bWenJian = findWenJian.FindFile(sFileName);	//搜索文件是否存在
	//如果没有发现文件，则直接返回
	if (!bWenJian)
	{
		//直接返回
		return;
	}


	//打开文件
	TiXmlDocument aDoc(MapCStringToString(sFileName).c_str());

	//检查打开文件是否成功
	if (!aDoc.LoadFile())
	{
		//提示
		AfxMessageBox(_T("打开BayesianNetwork_Query.xml失败:"));

		//退出
		return exit(0);
	}

	//获取根结点
	TiXmlElement *pRoot = aDoc.RootElement();
	TiXmlElement *pQuery = pRoot->FirstChildElement();
	while (pQuery != NULL)
	{
		//定义查询
		QUERY query;

		////////////////////////////////////////////////////////////////////////////////////////////
		//步骤1：获取边缘指针
		TiXmlElement* pMarginal = pQuery->FirstChildElement();
		//遍历所有查询变量
		if (pMarginal != NULL)
		{
			//获取查询变量指针
			TiXmlElement* pVariable = pMarginal->FirstChildElement();
			//遍历查询变量指针
			while (pVariable != NULL)
			{
				//定义实例化变量
				GROUNDING_VARIABLE query_variable;

				//获取查询变量ID
				string sTemp = GetAttribute(pVariable, "ID");
				query_variable.nNodeID = TransformStringToInt(sTemp);

				//获取查询变量值ID
				sTemp = GetAttribute(pVariable, "VALUE");

				//设置是否给定值
				query_variable.nValueID = TransformStringToInt(sTemp);//获取节点值的ID

				//添加到查询变量表
				query.QueryVariables.push_back(query_variable);

				//更新查询变量指针
				pVariable = pVariable->NextSiblingElement();
			}
		}


		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//步骤2：获取给定变量指针
		TiXmlElement* pGiven = pMarginal->NextSiblingElement();
		if (pGiven != NULL)
		{
			//获取给定变量指针
			TiXmlElement* pVariable = pGiven->FirstChildElement();

			//检查给定变量指针是否为空
			while (pVariable != NULL)
			{
				//定义实例化变量
				GROUNDING_VARIABLE given_variable;

				//获取给定变量ID
				string sTemp = GetAttribute(pVariable, "ID");
				given_variable.nNodeID = TransformStringToInt(sTemp);

				//获取给定变量值的ID
				sTemp = GetAttribute(pVariable, "VALUE");

				//设置是否给定值
				//获取给定变量值ID
				given_variable.nValueID = TransformStringToInt(sTemp);

				//添加到给定变量值表
				query.GivenVariables.push_back(given_variable);

				//更新给定变量指针
				pVariable = pVariable->NextSiblingElement();
			}
		}


		////////////////////////////////////////////////////////////////////////////////////////////////////
		//步骤3：获取删除变量顺序
		TiXmlElement* pEliminate = pGiven->NextSiblingElement();
		if (pEliminate != NULL)
		{
			//获取删除变量ID列表
			string sTemp = GetAttribute(pEliminate, "IDs");

			//检查是否为空
			if (sTemp != "")
			{
				//转换为字符列表
				vector<string>IDs;
				Separate(sTemp, IDs);

				//转换为整数
				for (unsigned int i = 0; i < IDs.size(); i++)
				{
					//获取删除节点ID
					unsigned int nID = TransformStringToInt(IDs[i]);

					//添加到删除节点集合
					query.EliminateVariables.push_back(nID);
				}
			}
		}

		//添加到查询列表
		m_Queries.push_back(query);

		//更新查询指针
		pQuery = pQuery->NextSiblingElement();
	}//结束查询节点

	
	//释放文件所占内存
	aDoc.Clear();
}