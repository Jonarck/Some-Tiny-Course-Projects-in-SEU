/////////////////////////////////////////////////////////////////////////////////////////////
//文件名：		Helper.h
//功  能：		定义辅助函数
//开发者：		高志强
//日  期：		2021年03月01日
//更  新：		2021年03月29日
//长  度：		60行
/////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <map>								//map头文件
#include <set>								//set头文件
#include <string>							//string头文件
#include <list>								//list头文件
#include <vector>							//vector头文件
#include <algorithm>						//algorithm类头文件
#include <numeric>							//numeric类头文件
#include <sstream>							//sstream类头文件
#include <fstream>							//fstream类头文件
#include <iostream>							//iostream类头文件
#include <time.h>							//时间函数类头文件
#include "math.h"							//数学函数头文件
#include "TinyXML.h"								//TinyXML头文件


//声明命名空间
using namespace std;						//使用标准命名空间


//切分串。例如将输入的长的字符串切分为多个短串（无符号整数）等
void Separate(string&, vector<string>&);

//将CString映射到string
string MapCStringToString(CString&);
//将string转换为wstring
wstring StoWs(const std::string& s);
//将wstring转换为string
string WS2S(const wstring& ws);
//去除空格
void RemoveSpace(wstring& str);

//将字符串转换为双精度数
double TransformStringToDouble(string);
//将字符串转换为整数
unsigned int TransformStringToInt(string);

//获取XML文件中属性的值
string GetAttribute(TiXmlElement *pEle, const char * name);

//检查无符号整数是否在列表中
bool IsINTInVector(unsigned int& nValue, vector<unsigned int>& Values, unsigned int& nPos);
//检查无符号整数是否在集合中
bool IsINTInSet(unsigned int& nNumber, set<unsigned int>& Numbers);
