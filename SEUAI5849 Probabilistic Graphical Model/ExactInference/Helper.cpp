 /////////////////////////////////////////////////////////////////////////////////////////////
//文件名：		Helper.cpp
//功  能：		实现辅助函数
//开发者：		高志强
//日  期：		2021年03月01日
//更  新：		2021年03月29日
//长  度：		250行
/////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"						//MFC头文件
#include "Helper.h"						//辅助函数头文件
#include<comutil.h>						//常用功能函数头文件	
#pragma comment(lib, "comsuppw.lib")	//常用功能函数库文件


//名  称：		Separate()
//功  能：		切分串
//参  数：		string&,vector<string>&
//返回值：		无
void Separate(string& sDiagnosises, vector<string>& DiagNames)
{
	//定义宽字符串
	wstring str = StoWs(sDiagnosises);

	//定义切分的开始和结束位置
	int nSepPosition = 0;

	//定义分割位置
	vector<int> SepPositions;
	//添加第1个元素
	SepPositions.push_back(nSepPosition);

	//遍历所有字符
	unsigned int i = nSepPosition;
	while (i<str.size())
	{
		//检查是否为分隔符
		WCHAR ch = str[i];

		if (ch == ',' || ch == ';' ||
			ch == L'，' || ch == L'；' ||
			ch == L'、')
		{
			//获取分割位置
			nSepPosition = i;

			//添加到分割位置列表
			SepPositions.push_back(nSepPosition);

			//更新位置
			i++;
		}
		else
		{
			//更新位置
			i++;
		}
	}

	//添加最后一个元素分隔符的位置
	//注意：这里发现一个错误，应该是宽字符串str的长度，而不应该是字符串sDiagnosis的长度
	SepPositions.push_back(str.size());

	//遍历所有分割位置。注意：不能用最后一个分割位置
	//修改：2014年05月01日。最后一个字符可能是分隔符
	for (unsigned int i = 0; i<SepPositions.size() - 1; i++)
	{
		//获取起始位置
		int nStartPos = SepPositions[i];
		//如果是第二个分段起，则向下移一个字符位置
		if (i>0)
		{
			nStartPos = SepPositions[i] + 1;
		}

		//获取结束位置
		int nEndPos = SepPositions[i + 1];

		//检查起始位置和结束位置是否合理
		if (nStartPos<(int)str.size()
			&& nEndPos <= (int)str.size()
			&& nStartPos != nEndPos)
		{
			//获取当前子字符串
			wstring sDiagName = str.substr(nStartPos, nEndPos - nStartPos);

			//裁剪左右空格
			RemoveSpace(sDiagName);

			//转换为字符串
			string sTemp = WS2S(sDiagName);

			//添加到诊断名称列表
			DiagNames.push_back(sTemp);
		}
	}
}

//名  称：		MapCStringToString()
//功  能：		将CString映射到string
//参  数：		CString&
//返回值：		string
string MapCStringToString(CString& sGivenCString)
{
	CStringA sTemp(sGivenCString.GetBuffer(0));
	sGivenCString.ReleaseBuffer();

	string sReturnString = sTemp.GetBuffer(0);
	sTemp.ReleaseBuffer();

	//返回结果
	return sReturnString;
}

//名  称：		StoWs()
//功  能：		将string映射到wstring
//参  数：		string&
//返回值：		wstring
wstring StoWs(const string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

//名  称：		WS2S()
//功  能：		将wstring转换为string
//参  数：		wstring&
//返回值：		string
string WS2S(const wstring& ws)
{
	_bstr_t t = ws.c_str();

	char* pchar = (char*)t;

	//获取字符串
	string result = pchar;

	//返回结果
	return result;
}

//名  称：		RemoveSpace()
//功  能：		去除空格
//参  数：		string&
//返回值：		无
void RemoveSpace(wstring& str)
{
	//获取宽字符
	wstring buff(str);

	//定义空格符
	WCHAR space = ' ';

	//裁剪左右空格
	str.assign(buff.begin() + buff.find_first_not_of(space), buff.begin() + buff.find_last_not_of(space) + 1);
}

//名  称：		TransformCStringToDouble()
//功  能：		将string转换为double
//参  数：		CString
//返回值：		dobule
double TransformStringToDouble(string sProbability)
{
	//将string抓换为double
	double fProb = atof(sProbability.c_str());

	//返回结果
	return fProb;
}

//名  称：		TransformStringToInt()
//功  能：		将string转换为unsigned int
//参  数：		string
//返回值：		unsigned int
unsigned int TransformStringToInt(string sNumber)
{
	//将string抓换为unsigned int
	unsigned int nNumber = (unsigned int)atoi(sNumber.c_str());

	//返回结果
	return nNumber;
}

// 名  称：		GetAttribute()
// 功  能：		获取属性值
// 参  数：		TiXmlElement*,const char*
// 返回值：		string
string GetAttribute(TiXmlElement *pEle, const char * name)
{
	const char * s = pEle->Attribute(name);
	if (s) return s;	// 如果const char *不为空，返回字符串
	else return "";		// 如果是空指针，返回空字符串
}

//名  称：		IsINTInVector()
//功  能：		检查无号整型变量是否在列表中
//参  数：		unsigned int&,vector<unsigned int>&,unsigned int&
//返回值：		bool
bool IsINTInVector(unsigned int& nValue, vector<unsigned int>& Values, unsigned int& nPos)
{
	//遍历所有值
	for (unsigned int i = 0; i < Values.size(); i++)
	{
		//检查是否相等
		if (nValue == Values[i])
		{
			//更新返回的位置
			nPos = i;

			//返回真
			return true;
		}
	}

	//缺省返回假
	return false;
}

//名  称：		IsINTInSet()
//功  能：		检查无符号整数是否在集合内
//参  数：		unsigned int&,set<unsigned int>&
//返回值：		bool
bool IsINTInSet(unsigned int& nNumber, set<unsigned int>& Numbers)
{
	//在集合中查找
	set<unsigned int>::iterator it = Numbers.find(nNumber);
	if (it != Numbers.end())
	{
		//找到
		return true;
	}
	else
	{
		//未找到
		return false;
	}
}