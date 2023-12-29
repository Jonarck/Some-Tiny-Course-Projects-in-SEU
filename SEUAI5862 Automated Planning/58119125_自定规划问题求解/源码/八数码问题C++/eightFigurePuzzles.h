#pragma once
#include <iostream>
#include <vector>
using namespace std;
const int  puzzleNum = 8;
/*
*��������¼8����Ź�����ÿһ�����Ӷ�Ӧ��λ����Ϣ���ŵ����롣
* xPosition :���� 0 �����һ��
* yPosition: ���� 0 �����һ��
* puzzleId: ���� 0~8
*/
typedef struct {
	int xPosition;
	int yPosition;
	int puzzleId;
} PUZZLE;

/*
*����������һ���ڵ㣬�洢��ǰ�Ź���״̬
*      ��Ŀ��״̬Ϊ����puzzle: {{0,0,0},{0,1,1},{0,2,2},
						   {1,0,3},{1,1,4},{1,2,5},
						   {2,0,6},{2,1,7},{2,2,8},}

				  nextActionList: {[1,0]    �����ƶ�
								   [-1,0]   �����ƶ�
								   [0,1]    �����ƶ�
								   [0.-1]}  �����ƶ�

				  nextAxtionList�Ĵ�С<=4

				  depth�� ��ǰ״̬�������
*/
typedef struct {
	//vector<PUZZLE> puzzles;
	PUZZLE puzzle[9];
	vector<vector<int>> nextActionList;
	vector<vector<int>> precedeActionList;
	int depth;
} PUZZLE_NODE;


/*
* ���룺�ڵ�״̬puzzleNode
* ���  �ո�λ�ã���ά���顣
* �������ҵ� �ո� 0 ���ڵ�λ��,����1��2ά���飬�ֱ��������������;
*/
int* findZeroPosition(PUZZLE_NODE puzzleNode);

/*
*
* ���룺�ڵ�״̬puzzleNode
* �����actionList��ʼ�����puzzleNode
*����������puzzleNode�ĺ�̿ɲ�������״̬������ ��1��0������ո������ƶ�����-1��0������ո������ƶ�����0��1������ո������ƶ�����0��-1������ո������ƶ���
*/
PUZZLE_NODE updatePuzzleNodeActionList(PUZZLE_NODE puzzleNode);

/*
* ���룺�����������飬����[1,0],����ո������ƶ�
* �����puzzleNode��ִ�������붯����õ����µ�����״̬
* ��������������action��actionΪ��ά���飩��puzzleNode,����ִ�иö������µĽڵ�
*/
PUZZLE_NODE moveToPuzzleNode(vector<int> action, PUZZLE_NODE puzzleNode);

/*
* ���룺puzzleNode�ĺ�̶��������Ĵ�С
* ����������������
* ��������������PuzzleNode�������������������������ˡ�
*/
int getRandomNumber(int actionSize);

/*
* ���룺puzzle1:��ǰ״̬ĳһλ���ϵ�״̬��puzzle2��Ŀ��״̬ĳһλ���ϵ�״̬
* �����true:��� false:�����
* �������жϵ�ǰ�ڵ�״̬��Ŀ��ڵ�״̬��ͬһλ��������8����״̬�Ƿ���ͬ��
*/
bool isEqual(PUZZLE puzzle1, PUZZLE puzzle2);

/*
* ���룺��ǰ����ڵ�״̬currentNode��Ŀ������ڵ�״̬objNode
* ����������ڵ�״̬�Ƿ�ƥ�䣬���ƥ�䣬˵���ҵ�Ŀ��״̬������true;
*                             �����ƥ�䣬˵����δ�ҵ�Ŀ��״̬������false;
*��������⵱ǰ�ڵ��Ŀ��ڵ�״̬�Ƿ���ͬ��
*/
bool checkObject(PUZZLE_NODE currentNode, PUZZLE_NODE objNode);

/*
* ���룺���˲���
* ���������Ŀ��״̬����backwardSteps��ĳ�ʼ״̬
* �������������˲��������س�ʼ�ڵ�״̬
*/
PUZZLE_NODE initialPuzzleNode(int backwardSteps);


/*
* ���룺����
* ���������Ŀ��״̬����backwardSteps��ĳ�ʼ״̬
* �������������
*/
void outputAction(vector<int> action, int index);

//�������ɵ�ǰ״̬��Ӧ��Ψһ���֣�����eightFigureFramework��visited�жϵ�ǰ�ڵ�״̬�Ƿ���ʹ���
int visitedNum(PUZZLE_NODE puzzleNode);
