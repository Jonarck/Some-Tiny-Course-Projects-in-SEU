/////////////////////////////////////////////////////////////////////////////////////////////
//文件名：		CBNSumProduct_SumProductEliminateVar.cpp
//功  能：		和积消除变量
//开发者：		高志强
//日  期：		2021年03月14日
//更  新：		2021年03月24日
//长  度：		80行
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"									//MFC标准头文件
#include "CBNSumProduct.h"							//贝叶斯网络精确推理类头文件，采用和积变量消除法
#include "Helper.h"									//辅助函数头文件


//名  称：		Sum_Product_Eliminate_Var()
//功  能：		和积消除变量
//参  数：		unsigned int,vector<CFactor>&
//返回值：		无
void CBNSumProduct::Sum_Product_Eliminate_Var(unsigned int nVariableID,vector<CFactor>& OldFactors)
{
	//定义因子列表
	vector<CFactor> NewFactors;
	
	/////////////////////////////////////////////////////////////////////////////
	//步骤1：构建因子列表，并从因子列表中删除和给定变量值不一致的行
	vector<CFactor>::iterator it = OldFactors.begin();
	while (it != OldFactors.end())
	{
		//定义给定变量位置
		unsigned int nPos;
		//定义并初始化因子的变量ID列表
		vector<unsigned int> VariableIDs = it->GetFactorVariableIDs();
		//检查因子的变量中是否包含给定变量。如果有的话，则通过参数返回其位置
		if (IsINTInVector(nVariableID, VariableIDs, nPos))
		{
			//添加到因子列表
			NewFactors.push_back(*it);

			//删除、并更新指针
			it = OldFactors.erase(it);
		}
		else
		{
			//更新指针
			it++;
		}
	}

	//检查因子列表的大小
	if (NewFactors.size() == 0)
	{
		//没有因子包含给定变量、直接返回
		return;
	}
	else
	{
		//定义新的乘积因子PSAI
		CFactor psai = NewFactors[0];

		
		///////////////////////////////////////////////////////////////////////////
		//步骤2：因子积
		for (unsigned int i = 1; i < NewFactors.size(); i++)
		{
			//更新因子积
			psai = psai*NewFactors[i];
		}


		//////////////////////////////////////////////////////////////////////////
		//步骤3：求和掉给定变量
		psai.SumOutVariable(nVariableID);


		//步骤4：将因子积添加到因子列表
		OldFactors.push_back(psai);
	}
}

