/////////////////////////////////////////////////////////////////////////////////////////////
//文件名：		CBNSumProduct.h
//功  能：		定义贝叶斯网络的精确推理类。采用和积变量消除法
//开发者：		高志强
//日  期：		2021年02月28日
//更  新：		2021年03月08日
//更  新：		2021年03月23日
//更  新：		2021年03月29日
//长  度：		130行
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
#include "CFactor.h"					//因子类头文件


//声明命名空间
using namespace std;					//使用标准命名空间


//定义类型
//实例化变量类型
typedef struct
{
	unsigned int nNodeID;								//01 变量ID
	unsigned int nValueID;								//02 变量的值ID
}GROUNDING_VARIABLE;

//条件概率分布表的行
typedef struct
{
	vector<unsigned int> ValueIDs;						//01 节点值ID的列表，包括节点的双亲、节点自己。节点ID的列表和PARENT_ID相同。最后添加节点自己
	double fProb;										//02 概率。相当于条件概率分布表的一行中最后的概率
}CPT_ROW;

//贝叶斯网络节点
typedef struct
{
	unsigned int nID;									//01 节点ID，如0
	string sName;										//02 节点名称，如Difficulty
	string sAbbreviation;								//03 节点简称，如D
	unsigned int nNumberOfValues;						//04 节点值的个数。节点取值用0、1、2代替，以学生例子中的Grade为例，取值不能是1、2、3。
	unsigned int nNumberOfParents;						//05 父节点个数，
	vector<unsigned int> ParentIDs;						//06 父节点ID列表。遵循该顺序给定条件概率分布表
	vector<double> CPTRowValues;						//07 条件概率分布表中每行的值
	vector<CPT_ROW> CPTRowIDs;							//08 条件概率分布表中每行的***变量ID***序列
}BN_NODE;

//贝叶斯网络的边
typedef struct
{
	unsigned int nID;									//01 边的ID。保留、未用
	unsigned int nStartNodeID;							//02 边的起点ID
	unsigned int nEndNodeID;							//03 边的终点ID
}BN_EDGE;

//查询
typedef struct
{
	vector<GROUNDING_VARIABLE> QueryVariables;								//查询的节点及其取值
	vector<GROUNDING_VARIABLE> GivenVariables;								//给定的节点及其取值
	vector<unsigned int> EliminateVariables;								//删除节点的顺序
}QUERY;


//定义贝叶斯网络的精确推理类CBNSumProduct。采用和积变量消除法
class CBNSumProduct
{
	//构造函数与析构函数
public:
	CBNSumProduct();									//构造函数
	~CBNSumProduct();									//析构函数

	//公有方法
public:
	//初始化
	void Init();										//初始化
	//查询
	void Query();										//查询
	
	//私有方法
private:
	//读取
	void Read_BN();										//读取贝叶斯网络结构与参数
	void Read_Query();									//读取查询变量、给定变量、变量消除顺序
	//预处理
	void Preprocess();									//预处理
	void Preprocess_BN();								//预处理贝叶斯网络结构与参数
	void Preprocess_Factor();							//预处理因子
	//查询
	void Query_Helper(QUERY&, vector<CFactor>);			//查询的辅助函数
	void Query_Marginal(QUERY&, vector<CFactor>&);		//查询边缘概率分布
	
	void Arrange(unsigned int,vector<unsigned int>, vector<unsigned int>,unsigned int,unsigned int&);				//排列父节点ID及其取值
	double GetProbFromCPT(vector<CPT_ROW>& cpts,vector<unsigned int>&);												//从CPT中查找概率
	double GetFactor(unsigned int nNodeID, unsigned int nNodeValue, vector<GROUNDING_VARIABLE>& QueryVariables);	//获取因子
	unsigned int GetValueID(unsigned int, vector<GROUNDING_VARIABLE>&);												//获取值的ID

	void OutputToXML();																								//输出概率分布到XML文件

	void Sum_Product_Eliminate_Var(unsigned int,vector<CFactor>&);													//和积变量消除
	
	//数据成员
private:
	vector<BN_NODE> m_Nodes;							//BN的节点表
	vector<BN_EDGE> m_Edges;							//BN的边表
	vector<CFactor> m_Factors;							//因子列表
	vector<QUERY> m_Queries;							//查询列表
	vector<double> m_QueryResults;						//查询结果列表
};