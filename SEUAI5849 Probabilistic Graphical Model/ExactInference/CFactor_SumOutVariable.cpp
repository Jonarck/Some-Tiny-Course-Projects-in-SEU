/////////////////////////////////////////////////////////////////////////////////////////////
//文件名：		CFactor_SumOutVariable.cpp
//功  能：		根据给定变量，对因子求和
//开发者：		高志强
//日  期：		2021年03月13日
//更  新：		2021年03月23日
//更  新：		2021年03月29日
//长  度：		150行
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"									//MFC标准头文件
#include "CFactor.h"								//因子类头文件
#include "Helper.h"									//辅助函数头文件


//名  称：		SumOutVariable()
//功  能：		根据给定变量，对因子求和
//参  数：		unsigned int
//返回值：		无
void CFactor::SumOutVariable(unsigned int nVariableID)
{	
	
	///////////////////////////////////////////////
	//函数2：求和掉变量，需要同学完成
	// 开始时间：21-5-26 20:00
	// 完成时间：21-5-26 00:18
	// debug完成：21-5-27 9:00
	// 完成者：58119125蒋卓洋
	///////////////////////////////////////////////
	// 分析：在一个因子中，给定该因子中需求和掉的变量，对该因子求和，要有以下操作：
	//  1.找到非被求和变量值ID相同的若干行
	// 	2.将这一组中的若干行的概率值相加，赋给其中一行，删去剩下的行。对每一组非被求和变量值ID相同的行重复执行这一步。
	// 	3.删去每一行中被求和变量的变量值ID对应元素。
	// 	4.删去变量ID中被求和变量对应元素
	// 	难以实现，换一种思路：
	// 	1.将因子行提取出来，按照被求和变量值ID分组
	// 	2.将每组中每行的被求和变量值ID对应的元素消去
	// 	3.将每组中对应的非被求和变量值ID（变量值ID数组剩下的元素的取值）相同的行的概率值相加，合成一行
	// 	4.将第三部中得到的行构成求和后的因子
	// 	5.将因子的变量ID数组中被求和变量的ID对应元素删去
	// 	编译失败，重新换方法：
	//	**利用因子行二维表的Vector的高级方法解决之前算法思路上的难点
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//1.遍历变量ID数组，删除被求和变量同时找到被求和变量对应的数组下标
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	unsigned int Index_nVariableID = 0;//定义并初始化被求和变量在变量ID数组中的下标
	//用迭代器+erase()方法实现删除指定元素
	for (vector<unsigned int>::iterator it = m_VariableIDs.begin(); it != m_VariableIDs.end();) {	// 遍历该因子的所有变量ID
		if (*it == nVariableID) {
			it = m_VariableIDs.erase(it);// 删除因子中被求和变量ID
			break;
		}
		else {
			it++;
			Index_nVariableID++;//累加以记录被求和变量对应的数组下标
		}

	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//2.遍历因子行，消除被求和变量值所在的列
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < m_FactorRows.size(); i++) {
		m_FactorRows[i].ValueIDs.erase(m_FactorRows[i].ValueIDs.begin() + Index_nVariableID);
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//3.遍历因子行，合并消除被求和变量后变量值ID相同的行
	// 	   3.1将两因子行的概率值相加赋给前因子行
	// 	   3.2删去后因子行
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	for (vector<FACTOR_ROW>::iterator it1 = m_FactorRows.begin(); it1 != m_FactorRows.end(); it1++) {//遍历前因子行
		for (vector<FACTOR_ROW>::iterator it2 = it1 + 1; it2 != m_FactorRows.end();) {//从前因子行后一位开始遍历后因子行
			if (it1->ValueIDs == it2->ValueIDs) {//如果找到该it所指的因子行的变量值相同的因子行it2
				it1->fValue += it2->fValue;	//将it2所指因子行的因子值加到it所指的因子行中
				it2 = m_FactorRows.erase(it2); //删除it2所指的因子行，迭代器会指向下行，不需要移动
			}
			else {
				it2++;//若不删除行，需迭代器移动
			}
		}
	}

	/*
//1.遍历变量ID数组，找到被求和变量对应的数组下标和非被求和变量对应数组下标
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int Index_nVariableID = 0;//定义并初始化被求和变量在变量ID数组中的下标
for (unsigned int i = 0; i < m_VariableIDs.size(); i++) {
	if (m_VariableIDs[i] == nVariableID) {
		Index_nVariableID = i;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//2.遍历因子行，按照被求和变量的变量值ID分组
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
vector<vector<FACTOR_ROW>>Sumed_FactorRowClass;//分组表，每一个元素是一组被求和变量的变量值ID相同的因子行

for (unsigned int i = 0; i < m_FactorRows.size(); i++) {//遍历因子行,获得行分组，针对被求和变量变量值ID的个数分两种情况讨论
	if (nVariableID == 2) {//变量2（G）有三个取值0,1,2，分三组
		vector<vector<FACTOR_ROW>>Sumed_FactorRowClass3(3);
		Sumed_FactorRowClass = Sumed_FactorRowClass3;//分组表确定初始化：大小3
		if (m_FactorRows[i].ValueIDs[Index_nVariableID] == 0) {
			Sumed_FactorRowClass[0].push_back(m_FactorRows[i]);
		}
		else if (m_FactorRows[i].ValueIDs[Index_nVariableID] == 1) {
			Sumed_FactorRowClass[1].push_back(m_FactorRows[i]);
		}
		else if (m_FactorRows[i].ValueIDs[Index_nVariableID] == 2) {
			Sumed_FactorRowClass[2].push_back(m_FactorRows[i]);
		}
	}
	else {//其他变量有两个取值0,1，分两组
		vector<vector<FACTOR_ROW>>Sumed_FactorRowClass2(2);
		Sumed_FactorRowClass = Sumed_FactorRowClass2;//分组表确定初始化：大小2
		if (m_FactorRows[i].ValueIDs[Index_nVariableID] == 0) {
			Sumed_FactorRowClass[0].push_back(m_FactorRows[i]);
		}
		else if (m_FactorRows[i].ValueIDs[Index_nVariableID] == 1) {
			Sumed_FactorRowClass[1].push_back(m_FactorRows[i]);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//3.处理分好的组，将每组中每行的被求和变量值ID对应的元素消去
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
for (unsigned int i = 0; i < Sumed_FactorRowClass.size(); i++) {//遍历每一个组
	for (unsigned int j = 0; j < Sumed_FactorRowClass[i].size(); j++) {//每一个组中遍历每一行
		FACTOR_ROW Sumed_FactorRow;//定义消去被求和变量后的新行
		Sumed_FactorRow.fValue = Sumed_FactorRowClass[i][j].fValue;//新行的概率值不变，赋值
		for (unsigned int k = 0; k < m_VariableIDs.size(); k++) {//遍历此老行的变量值ID元素把老行的非被求和变量值ID元素加入至新行变量值ID数组
			if (Sumed_FactorRowClass[i][j].ValueIDs[k] != nVariableID) {//若不为被求和变量的ID取值元素，则赋值给新行
				Sumed_FactorRow.ValueIDs.push_back(Sumed_FactorRowClass[i][j].ValueIDs[k]);
			}
		}
		Sumed_FactorRowClass[i][j] = Sumed_FactorRow;//用新行替代老行
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//4.处理消去被求和变量后的组，将所有组中相同的行合并，按组数不同分类讨论
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
vector<FACTOR_ROW> m_Sumed_FactorRow;//定义合并后的因子行的列表
if (Sumed_FactorRowClass.size() == 2) {//若有两组
	for (unsigned int i = 0; i < Sumed_FactorRowClass[0].size(); i++) {
		for (unsigned int j = 0; j < Sumed_FactorRowClass[1].size(); j++) {
			if (Sumed_FactorRowClass[0][i].ValueIDs == Sumed_FactorRowClass[1][j].ValueIDs) {//若两组中某两行变量值ID数组完全相同则合并
				m_Sumed_FactorRow[i].ValueIDs
					= Sumed_FactorRowClass[0][i].ValueIDs;//合并结果行的变量值ID数组即为相同行的变量值ID数组
				m_Sumed_FactorRow[i].fValue
					= Sumed_FactorRowClass[0][i].fValue
					+ Sumed_FactorRowClass[1][j].fValue;//合并结果行的概率即为两相同行的概率求和
			}
		}
	}
}
else if (Sumed_FactorRowClass.size() == 3) {//若有三组
	for (unsigned int i = 0; i < Sumed_FactorRowClass[0].size(); i++) {
		for (unsigned int j = 0; j < Sumed_FactorRowClass[1].size(); j++) {
			for (unsigned int k = 0; k < Sumed_FactorRowClass[2].size(); k++) {
				if (Sumed_FactorRowClass[0][i].ValueIDs == Sumed_FactorRowClass[1][j].ValueIDs && Sumed_FactorRowClass[0][i].ValueIDs == Sumed_FactorRowClass[2][k].ValueIDs) {//若三组中某三行变量值ID数组完全相同则合并
					m_Sumed_FactorRow[i].ValueIDs
						= Sumed_FactorRowClass[0][i].ValueIDs;//合并结果行的变量值ID数组即为相同行的变量值ID数组
					m_Sumed_FactorRow[i].fValue
						= Sumed_FactorRowClass[0][i].fValue
						+ Sumed_FactorRowClass[1][j].fValue
						+ Sumed_FactorRowClass[2][k].fValue;//合并结果行的概率即为三相同行的概率求和
				}
			}
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//5.更新求和后因子的因子行列表
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
m_FactorRows = m_Sumed_FactorRow;//更新因子行为合并后的因子行
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//6.删去被求和变量的变量ID
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
vector<unsigned int>m_Sumed_VariableIDs;//定义求和后因子的变量ID数组
for (unsigned int i = 0; i < m_VariableIDs.size(); i++) {
	if (m_VariableIDs[i] != nVariableID) {
		m_Sumed_VariableIDs.push_back(m_VariableIDs[i]);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//7.更新求和后因子的变量ID数组
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
m_VariableIDs = m_Sumed_VariableIDs;//更新求和后因子的变量ID数组为删去被求和变量的变量ID的数组
*/

}

//名  称：		MergeIntoFirstRow()
//功  能：		累积因子值到首行
//参  数：		FACTOR_ROW&
//返回值：		无
void CFactor::MergeIntoFirstRow(FACTOR_ROW& first_row)
{
	//遍历所有因子行
	vector<FACTOR_ROW>::iterator it = m_FactorRows.begin();

	//删除首行
	it = m_FactorRows.erase(it);

	//检查是否结束
	while (it != m_FactorRows.end())
	{
		//检查变量值ID的列表是否相等
		if (first_row.ValueIDs == it->ValueIDs)
		{
			//累加因子值
			first_row.fValue += it->fValue;

			//删除、并更新迭代器
			it = m_FactorRows.erase(it);
		}
		else
		{
			//更新迭代器
			it++;
		}
	}
}