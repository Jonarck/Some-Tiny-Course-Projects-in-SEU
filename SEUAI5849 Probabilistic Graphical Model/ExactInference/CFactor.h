/////////////////////////////////////////////////////////////////////////////////////////////
//�ļ�����		CFactor.h
//��  �ܣ�		����������
//�����ߣ�		��־ǿ
//��  �ڣ�		2021��03��12��
//��  �£�		2021��03��13��
//��  �£�		2021��03��29��
//��  �ȣ�		210��
/////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <map>							//mapͷ�ļ�
#include <set>							//setͷ�ļ�
#include <string>						//stringͷ�ļ�
#include <list>							//listͷ�ļ�
#include <vector>						//vectorͷ�ļ�
#include <algorithm>					//algorithm��ͷ�ļ�
#include <numeric>						//numeric��ͷ�ļ�
#include <sstream>						//sstream��ͷ�ļ�
#include <fstream>						//fstream��ͷ�ļ�
#include <time.h>						//ʱ�亯����ͷ�ļ�
#include "math.h"						//��ѧ����ͷ�ļ�
#include "Helper.h"						//��������ͷ�ļ�


//���������ռ�
using namespace std;					//ʹ�ñ�׼�����ռ�


//����������
typedef struct
{
	vector<unsigned int> ValueIDs;								//01 ����ֵID���б����ձ���ID�б��˳������
	double fValue;												//02 ���ӵ�ֵ
}FACTOR_ROW;


//����������
class CFactor
{
	//���캯������������
public:
	CFactor();													//���캯��
	~CFactor();													//��������

	//���з���
public:
	//��ʼ��
	void SetFactorVariableIDs(vector<unsigned int>&);			//�������ӵı���ID�б�
	void SetFactorRow(vector<unsigned int>&, double&);			//���������е�ֵ
	//����
	vector<unsigned int> GetFactorVariableIDs();				//��ȡ����ID�б�
	//����
	void ReduceGivenVariable(unsigned int, unsigned int);		//���ӻ���������������ID���ͱ���ֵID
	void SumOutVariable(unsigned int);								//������͵���������ID
	void Normalize();											//���ӹ�һ��
	//��ѯ
	double Query(vector<unsigned int>&,vector<unsigned int>&);	//���ݸ�������ID���ϡ�����ֵID�ļ��ϣ���ȡ�����ѯ�ĸ���ֵ

	//�������ӻ�
	CFactor operator*(const CFactor& second)
	{
		
		//�����µġ������ص�����
		CFactor factor;
		
		/////////////////////////////////////////////
		//����1�����ӻ����أ���Ҫ��ͬѧ���
		// ��ʼʱ�䣺21-5-24 17:23
		// ���ʱ�䣺21-5-24 20:15
		// ����ߣ�58119125��׿��
		////////////////////////////////////////////
		//ǰ�ò��裺ͨ�����Ե���BN��ʼ�������۲��������ȡֵ�����۲�Ԥ��������ȷ�����ڵ㡱���ߡ��ͳ�ʼ�����ӡ��ṹ�Ͷ��巽ʽ��
		// ����������������������ɣ�����������IDֵ�ı���ID��Ӧ�гɵ������У�������ȡֵID������������еĸ��ʣ�
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//1.���������ӽ���
		// ������������˵Ĺ���ʵ�����ǽ������ӵķֲ��µ����ϸ��ʷֲ�������������Ƿ�����ͬ������Ϊ���࣬����������ͬ��������Ҫ����ͬ�����ϲ�
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//1.1������������ͬ����IDӳ���ϵ��������˺ϲ�ʱʹ�ã�

		map<unsigned int, unsigned int>First_to_Second;
		
		//1.2�����������������ӣ������Ӻͱ������ӣ��ı���IDȡֵ������ӳ���ϵ��
		for (unsigned int i = 0; i < m_VariableIDs.size(); i++) {//�����ӱ���
			unsigned int First_VariableID = m_VariableIDs[i];//ÿ�α�����¼�ôα����ı���IDȡֵ
			for (unsigned int j = 0; j < second.m_VariableIDs.size(); j++) {// �������ӱ���
				unsigned int Second_VariableID = second.m_VariableIDs[j];//ÿ�α�����¼�ôα����ı���IDȡֵ

				if (First_VariableID == Second_VariableID) {//����ӳ���ϵ������
					First_to_Second.insert(make_pair(i, j));//��ͬ����ID�ӳ����ӣ��е�λ�ã����������ӣ��е�λ�ã��Ķ�Ӧ��ϵ��ӳ�䣩
				}
			}
		}

		//1.3ʹ��ӳ���ϵ��map�����������кͳ�������ͬ�ı���ID���ڵ�ַ������ȡ��Ϊһ�����ϣ����ں����˷��ϲ�����
		set<unsigned int>Intersection_in_Second;//����ü���
		
		for (map<unsigned int, unsigned int>::iterator it = First_to_Second.begin(); it != First_to_Second.end(); it++) {//����ӳ���ϵmap��ȡ�������кͳ�������ͬ�ı���ID���ڵ�ַ
			Intersection_in_Second.insert(it->second);
		}
		
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//2.���ӳ˷�ʵ��
		// �����������������һ�ǳ������뱻�����Ӽ�����ͬ�ı���IDֵ������ͬ����IDֵ�ϲ����ڶ�Ӧ�ϲ������������������ж�Ӧ����
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//2.1����һ���������뱻�����Ӽ�����ͬ�ı���ID��ֱ�ӹ���������
		if (First_to_Second.size() == 0) {
			for (unsigned int i = 0; i < m_FactorRows.size(); i++) {//���������ӵ�������
				for (unsigned int j = 0; j < second.m_FactorRows.size(); j++) {//�����������ӵ�������
					FACTOR_ROW Product_FactorRow;//�������ӳ˻���������
					Product_FactorRow.ValueIDs = m_FactorRows[i].ValueIDs;//�Ƚ������ӵı���ֵID����˻����ӵı���ֵID
					for (unsigned int m = 0; m < second.m_FactorRows[j].ValueIDs.size(); m++) {//�ٱ����������ӵı���ֵID�����ǽ����ı���ֵID����˻����ӵı���ֵID
						Product_FactorRow.ValueIDs.push_back(second.m_FactorRows[j].ValueIDs[m]);//����ǽ����ı������ӱ���ֵID						
					}
					Product_FactorRow.fValue = m_FactorRows[i].fValue * second.m_FactorRows[j].fValue;//����˻����������еĸ���ֵ
					factor.m_FactorRows.push_back(Product_FactorRow);//�˻�������������ӵ����μ�������������������
				}
			}
			factor.m_VariableIDs = m_VariableIDs;//�Ƚ������ӵı���ID���������ӱ���ID
			for (unsigned int i = 0; i < second.m_VariableIDs.size(); i++) {//�ٱ����������ӵı���ID�����ǽ����ı���ID���������ӵı���ID
				factor.m_VariableIDs.push_back(second.m_VariableIDs[i]);//����ǽ����ı������ӱ���ID						
			}
		}
		//2.2���Ͷ����������뱻�����Ӽ�����ͬ�ı���IDֵ��������������ϲ���ͬ����
		else {
			for (unsigned int i = 0; i < m_FactorRows.size(); i++) {//���������ӵ�������
				for (unsigned int j = 0; j < second.m_FactorRows.size(); j++) {//�����������ӵ�������
					if (IsConsistentValueID(m_FactorRows[i], second.m_FactorRows[j], First_to_Second)) {//�������������м���ʱ
						FACTOR_ROW Product_FactorRow;//�������ӳ˻���������
						Product_FactorRow.ValueIDs = m_FactorRows[i].ValueIDs;//�Ƚ������ӵı���ֵID����˻����ӵı���ֵID
						for (unsigned int m = 0; m < second.m_FactorRows[j].ValueIDs.size(); m++) {//�ٱ����������ӵı���ֵID�����ǽ����ı���ֵID����˻����ӵı���ֵID
							if (!IsINTInSet(m, Intersection_in_Second)) {
								Product_FactorRow.ValueIDs.push_back(second.m_FactorRows[j].ValueIDs[m]);//����ǽ����ı������ӱ���ֵID
							}
						}
						Product_FactorRow.fValue = m_FactorRows[i].fValue * second.m_FactorRows[j].fValue;//����˻����������еĸ���ֵ
						factor.m_FactorRows.push_back(Product_FactorRow);//�˻�������������ӵ����μ�������������������
					}
				}
			}
			factor.m_VariableIDs = m_VariableIDs;//�Ƚ������ӵı���ID���������ӱ���ID
			for (unsigned int i = 0; i < second.m_VariableIDs.size(); i++) {//�ٱ����������ӵı���ID�����ǽ����ı���ID���������ӵı���ID
				if (!IsINTInSet(i, Intersection_in_Second)) {
					factor.m_VariableIDs.push_back(second.m_VariableIDs[i]);//����ǽ����ı������ӱ���ID	
				}
			}
		}
		
		//��������
		return factor;
	
	};		
	
	//˽�з���
private:
	//�����ӻ�ʱ�ж������Ƿ���Ժϲ�
	bool IsConsistentValueID(const FACTOR_ROW&, const FACTOR_ROW&, map<unsigned int, unsigned int>&);	//��������Ƿ����	
	//�����ʱ�����ӵ�ֵ�ϲ�������
	void MergeIntoFirstRow(FACTOR_ROW&);
	
	//˽������
private:	
	vector<unsigned int> m_VariableIDs;					//���ӱ���ID�б�
	vector<FACTOR_ROW> m_FactorRows;					//�����е��б� => ������������
};