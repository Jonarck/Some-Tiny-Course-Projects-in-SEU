/////////////////////////////////////////////////////////////////////////////////////////////
//文件名：		CFactor.h
//功  能：		定义因子类
//开发者：		高志强
//日  期：		2021年03月12日
//更  新：		2021年03月13日
//更  新：		2021年03月29日
//长  度：		210行
/////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <map>							//map头文件
#include <set>							//set头文件
#include <string>						//string头文件
#include <list>							//list头文件
#include <vector>						//vector头文件
#include <algorithm>					//algorithm类头文件
#include <numeric>						//numeric类头文件
#include <sstream>						//sstream类头文件
#include <fstream>						//fstream类头文件
#include <time.h>						//时间函数类头文件
#include "math.h"						//数学函数头文件
#include "Helper.h"						//辅助函数头文件


//声明命名空间
using namespace std;					//使用标准命名空间


//定义因子行
typedef struct
{
	vector<unsigned int> ValueIDs;								//01 变量值ID的列表，按照变量ID列表的顺序排列
	double fValue;												//02 因子的值
}FACTOR_ROW;


//定义因子类
class CFactor
{
	//构造函数与析构函数
public:
	CFactor();													//构造函数
	~CFactor();													//析构函数

	//公有方法
public:
	//初始化
	void SetFactorVariableIDs(vector<unsigned int>&);			//设置因子的变量ID列表
	void SetFactorRow(vector<unsigned int>&, double&);			//设置因子行的值
	//访问
	vector<unsigned int> GetFactorVariableIDs();				//获取变量ID列表
	//操作
	void ReduceGivenVariable(unsigned int, unsigned int);		//因子化简。消除给定变量ID、和变量值ID
	void SumOutVariable(unsigned int);								//因子求和掉给定变量ID
	void Normalize();											//因子归一化
	//查询
	double Query(vector<unsigned int>&,vector<unsigned int>&);	//根据给定变量ID集合、变量值ID的集合，获取具体查询的概率值

	//重载因子积
	CFactor operator*(const CFactor& second)
	{
		
		//定义新的、待返回的因子
		CFactor factor;
		
		/////////////////////////////////////////////
		//函数1：因子积重载：需要由同学完成
		// 开始时间：21-5-24 17:23
		// 完成时间：21-5-24 20:15
		// 完成者：58119125蒋卓洋
		////////////////////////////////////////////
		//前置步骤：通过调试调用BN初始化函数观察各个变量取值，并观察预处理函数，确定“节点”“边”和初始“因子”结构和定义方式。
		// 分析：；因子由两部分组成：所含变量的ID值的变量ID对应行成的因子行（含变量取值ID罗列与该因子行的概率）
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//1.求两个因子交集
		// 分析：因子相乘的过程实际上是将两因子的分布新的联合概率分布，需根据因子是否含有相同变量分为两类，且若含有相同变量，需要间相同变量合并
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//1.1定义两因子相同变量ID映射关系，便于相乘合并时使用：

		map<unsigned int, unsigned int>First_to_Second;
		
		//1.2遍历做积的两个因子（乘因子和被乘因子）的变量ID取值，建立映射关系：
		for (unsigned int i = 0; i < m_VariableIDs.size(); i++) {//乘因子遍历
			unsigned int First_VariableID = m_VariableIDs[i];//每次遍历记录该次遍历的变量ID取值
			for (unsigned int j = 0; j < second.m_VariableIDs.size(); j++) {// 被乘因子遍历
				unsigned int Second_VariableID = second.m_VariableIDs[j];//每次遍历记录该次遍历的变量ID取值

				if (First_VariableID == Second_VariableID) {//建立映射关系的条件
					First_to_Second.insert(make_pair(i, j));//相同变量ID从乘因子（中的位置）到被乘因子（中的位置）的对应关系（映射）
				}
			}
		}

		//1.3使用映射关系的map将被乘因子中和乘因子相同的变量ID所在地址单独提取成为一个集合，便于后续乘法合并操作
		set<unsigned int>Intersection_in_Second;//定义该集合
		
		for (map<unsigned int, unsigned int>::iterator it = First_to_Second.begin(); it != First_to_Second.end(); it++) {//遍历映射关系map提取乘因子中和乘因子相同的变量ID所在地址
			Intersection_in_Second.insert(it->second);
		}
		
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//2.因子乘法实现
		// 分析：分两种情况：一是乘因子与被乘因子间有相同的变量ID值，则将相同变量ID值合并，在对应合并后的因子行求该因子行对应概率
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//2.1类型一：乘因子与被乘因子间无相同的变量ID，直接构造因子行
		if (First_to_Second.size() == 0) {
			for (unsigned int i = 0; i < m_FactorRows.size(); i++) {//遍历乘因子的因子行
				for (unsigned int j = 0; j < second.m_FactorRows.size(); j++) {//遍历被乘因子的因子行
					FACTOR_ROW Product_FactorRow;//定义因子乘积的因子行
					Product_FactorRow.ValueIDs = m_FactorRows[i].ValueIDs;//先将乘因子的变量值ID放入乘积因子的变量值ID
					for (unsigned int m = 0; m < second.m_FactorRows[j].ValueIDs.size(); m++) {//再遍历被乘因子的变量值ID将不是交集的变量值ID放入乘积因子的变量值ID
						Product_FactorRow.ValueIDs.push_back(second.m_FactorRows[j].ValueIDs[m]);//放入非交集的被乘因子变量值ID						
					}
					Product_FactorRow.fValue = m_FactorRows[i].fValue * second.m_FactorRows[j].fValue;//计算乘积因子因子行的概率值
					factor.m_FactorRows.push_back(Product_FactorRow);//乘积因子因子行添加到本次计算所得因子因子行中
				}
			}
			factor.m_VariableIDs = m_VariableIDs;//先将乘因子的变量ID放入结果因子变量ID
			for (unsigned int i = 0; i < second.m_VariableIDs.size(); i++) {//再遍历被乘因子的变量ID将不是交集的变量ID放入结果因子的变量ID
				factor.m_VariableIDs.push_back(second.m_VariableIDs[i]);//放入非交集的被乘因子变量ID						
			}
		}
		//2.2类型二：乘因子与被乘因子间有相同的变量ID值，构造因子行需合并相同变量
		else {
			for (unsigned int i = 0; i < m_FactorRows.size(); i++) {//遍历乘因子的因子行
				for (unsigned int j = 0; j < second.m_FactorRows.size(); j++) {//遍历被乘因子的因子行
					if (IsConsistentValueID(m_FactorRows[i], second.m_FactorRows[j], First_to_Second)) {//当两因子因子行兼容时
						FACTOR_ROW Product_FactorRow;//定义因子乘积的因子行
						Product_FactorRow.ValueIDs = m_FactorRows[i].ValueIDs;//先将乘因子的变量值ID放入乘积因子的变量值ID
						for (unsigned int m = 0; m < second.m_FactorRows[j].ValueIDs.size(); m++) {//再遍历被乘因子的变量值ID将不是交集的变量值ID放入乘积因子的变量值ID
							if (!IsINTInSet(m, Intersection_in_Second)) {
								Product_FactorRow.ValueIDs.push_back(second.m_FactorRows[j].ValueIDs[m]);//放入非交集的被乘因子变量值ID
							}
						}
						Product_FactorRow.fValue = m_FactorRows[i].fValue * second.m_FactorRows[j].fValue;//计算乘积因子因子行的概率值
						factor.m_FactorRows.push_back(Product_FactorRow);//乘积因子因子行添加到本次计算所得因子因子行中
					}
				}
			}
			factor.m_VariableIDs = m_VariableIDs;//先将乘因子的变量ID放入结果因子变量ID
			for (unsigned int i = 0; i < second.m_VariableIDs.size(); i++) {//再遍历被乘因子的变量ID将不是交集的变量ID放入结果因子的变量ID
				if (!IsINTInSet(i, Intersection_in_Second)) {
					factor.m_VariableIDs.push_back(second.m_VariableIDs[i]);//放入非交集的被乘因子变量ID	
				}
			}
		}
		
		//返回因子
		return factor;
	
	};		
	
	//私有方法
private:
	//在因子积时判断两行是否可以合并
	bool IsConsistentValueID(const FACTOR_ROW&, const FACTOR_ROW&, map<unsigned int, unsigned int>&);	//检查两行是否兼容	
	//在求和时将因子的值合并到首行
	void MergeIntoFirstRow(FACTOR_ROW&);
	
	//私有数据
private:	
	vector<unsigned int> m_VariableIDs;					//因子变量ID列表
	vector<FACTOR_ROW> m_FactorRows;					//因子行的列表 => 用于描述因子
};