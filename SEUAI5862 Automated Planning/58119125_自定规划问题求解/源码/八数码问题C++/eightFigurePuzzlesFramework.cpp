#include <iostream>
#include <vector>
#include <queue>
#include<map>
#include <time.h>
#include "eightFigurePuzzles.h"
#include <windows.h>
using namespace std;

//���ڼ�¼��ǰ״̬�Ƿ񱻷��ʹ���
map<int, int> visited;

//�����������������������ȡ�
#define MAX_DEPTH 20

//openList��closeList����A*������
vector<PUZZLE_NODE> closeList;
vector<PUZZLE_NODE> openList;


//�����������
int* binaryFirstSearch(PUZZLE_NODE initialNode, PUZZLE_NODE objPuzzleNode)
{
	//result[0] 1:correct;0:wrong
//result[1] ���� steps 
	int result[2] = { 0,0 };

	/*
		���ڸ�λ����ɹ������������
	*/


	if (checkObject(initialNode, objPuzzleNode)) {
		result[0] = 1;
	}
	else {
		result[0] = 0;
	}

	return result;

}

//�����������
int* depthFirstSearch(PUZZLE_NODE initialNode, PUZZLE_NODE objPuzzleNode) {

	//result[0] 1:correct;0:wrong
	//result[1] ���� steps 
	int result[2] = { 0,0 };
	/*
		���ڸ�λ�������������������������޶�Ϊ25��
	*/


	if (checkObject(initialNode, objPuzzleNode) && initialNode.depth < MAX_DEPTH) {
		result[0] = 1;
	}
	else {
		result[0] = 0;
	}

	return result;
}


//����ʽ����1
int* heuristicSearchInformedByIncorrectNum(PUZZLE_NODE initialNode, PUZZLE_NODE objPuzzleNode) {
	//result[0] 1:correct;0:wrong
	//result[1] ���� steps 
	int result[2] = { 0,0 };

	/*
		���ڸ�λ���������ʽ����������ʽ����ʹ�ò���ȷλ�õ����������
	*/


	if (checkObject(initialNode, objPuzzleNode)) {
		result[0] = 1;
	}
	else {
		result[0] = 0;
	}

	return result;
}

//����ʽ����2
int* heuristicSearchInformedByManhattonDis(PUZZLE_NODE initialNode, PUZZLE_NODE objPuzzleNode) {
	//result[0] 1:correct;0:wrong
	//result[1] ���� steps 
	int result[2] = { 0,0 };
	/*
		���ڸ�λ���������ʽ����������ʽ�������õ�Ŀ��λ�õ������پ��롣
	*/


	if (checkObject(initialNode, objPuzzleNode)) {
		result[0] = 1;
	}
	else {
		result[0] = 0;
	}

	return result;
}

//�����������
int* binaryFirstSearchDemo(PUZZLE_NODE initialNode, PUZZLE_NODE objPuzzleNode)
{
	//result[0] 1:correct;0:wrong
	//result[1] ���� steps 
	int result[2] = { 0,0 };

	cout << "��ʼ�ڵ�״̬��" << endl;
	for (int i = 0; i < 3; i++) {
		cout << " " << initialNode.puzzle[i * 3 + 0].puzzleId << "  " << initialNode.puzzle[i * 3 + 1].puzzleId <<
			"  " << initialNode.puzzle[i * 3 + 2].puzzleId << endl;
	}
	cout << endl;
	/*
		���ڸ�λ����ɹ����������������
	*/
	PUZZLE_NODE puzzleNode = initialNode;
	queue<PUZZLE_NODE> puzzleNodeQueue;
	puzzleNode.depth = 0;
	int depth = 0;
	puzzleNodeQueue.push(puzzleNode);
	while (puzzleNodeQueue.size()) {
		PUZZLE_NODE currentPuzzleNode = puzzleNodeQueue.front();
		if (checkObject(currentPuzzleNode, objPuzzleNode)) {

			for (int i = 0; i < currentPuzzleNode.precedeActionList.size(); i++) {
				outputAction(currentPuzzleNode.precedeActionList[i], i + 1);
			}
			cout << "�ҵ���ȷ���:" << endl;
			for (int i = 0; i < 3; i++) {
				cout << " " << currentPuzzleNode.puzzle[i * 3 + 0].puzzleId << "  " << currentPuzzleNode.puzzle[i * 3 + 1].puzzleId <<
					"  " << currentPuzzleNode.puzzle[i * 3 + 2].puzzleId << endl;
			}
			cout << endl;

			result[0] = 1;
			result[1] = currentPuzzleNode.depth;
			return result;
		}
		else {
			visited[visitedNum(currentPuzzleNode)] = 1;
			if (currentPuzzleNode.nextActionList.size() == 0) {
				currentPuzzleNode = updatePuzzleNodeActionList(currentPuzzleNode);
			}
			puzzleNodeQueue.pop();
			for (int i = 0; i < currentPuzzleNode.nextActionList.size(); i++) {
				PUZZLE_NODE nextPuzzleNode = moveToPuzzleNode(currentPuzzleNode.nextActionList[i], currentPuzzleNode);
				if (!currentPuzzleNode.precedeActionList.empty()) {
					for (int actionIndex = 0; actionIndex < currentPuzzleNode.precedeActionList.size(); actionIndex++)
					{
						nextPuzzleNode.precedeActionList.push_back(currentPuzzleNode.precedeActionList[actionIndex]);
					}
				}
				nextPuzzleNode.precedeActionList.push_back(currentPuzzleNode.nextActionList[i]);
				if (visited[visitedNum(nextPuzzleNode)] == 1) {
					continue;
				}
				nextPuzzleNode.depth = currentPuzzleNode.depth + 1;
				puzzleNodeQueue.push(nextPuzzleNode);
			}
		}

	}
	return result;
}

int main() {
	PUZZLE_NODE objPuzzleNode;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			objPuzzleNode.puzzle[i * 3 + j].puzzleId = i * 3 + j;
			objPuzzleNode.puzzle[i * 3 + j].xPosition = i;
			objPuzzleNode.puzzle[i * 3 + j].yPosition = j;
		}
	}
	objPuzzleNode = updatePuzzleNodeActionList(objPuzzleNode);

	int setup = 0;
	while (setup != -1) {


		visited.clear();
		cout << "-------------------------------------------------" << endl;
		cout << "�������������:" << endl;
		cout<< " -1:�˳�; " << endl;
		cout << "0:�����������ʾ��;" << endl;
		cout << "1:�����������; " << endl;
		cout << "2:�����������; " << endl;
		cout << "3:����ʽ����1; " << endl;
		cout<<  "4:����ʽ����2):" << endl; 
		
		cin >> setup;
		int backwardSteps;
		cout << "��������ڵ���5С�ڵ���20�Ļ��˲���" << endl;
		cin >> backwardSteps;
		while (backwardSteps < 5 || backwardSteps >20) {
			cout << "���������������ڵ���5С�ڵ���20�Ļ��˲���" << endl;
			cin >> backwardSteps;
		}


		PUZZLE_NODE initialNode = initialPuzzleNode(backwardSteps);


		
		int* result;
		if (setup == 1) {
			result = binaryFirstSearch(initialNode, objPuzzleNode);
		}
		else if (setup == 2) {
			result = depthFirstSearch(initialNode, objPuzzleNode);
		}
		else if (setup == 3) {
			result = heuristicSearchInformedByIncorrectNum(initialNode, objPuzzleNode);
		}
		else if (setup == 4) {
			result = heuristicSearchInformedByManhattonDis(initialNode, objPuzzleNode);
		}
		else if (setup == 0) {
			cout << "�����������ʾ��:" << endl;
			result = binaryFirstSearchDemo(initialNode, objPuzzleNode);
		}
		else {
			cout << "����������������������" << endl;
			cout << endl;
			return 0;
		}

		if (result[0] == 1) {
			cout << "���Ϊcorrect,����Ϊ" << result[1] << endl;
		}
		else {
			cout << "���Ϊwrong" << endl;
		}
		cout << "-------------------------------------------------" << endl;

		cout << endl;
	}
	return 0;

}
