#include <iostream>
#include <vector>
#include <queue>
#include<map>
#include <time.h>
#include "eightFigurePuzzles.h"
#include <windows.h>
using namespace std;

//用于记录当前状态是否被访问过。
map<int, int> visited;

//深度有限搜索，用于限制深度。
#define MAX_DEPTH 20

//openList与closeList用于A*搜索。
vector<PUZZLE_NODE> closeList;
vector<PUZZLE_NODE> openList;


//广度优先搜索
int* binaryFirstSearch(PUZZLE_NODE initialNode, PUZZLE_NODE objPuzzleNode)
{
	//result[0] 1:correct;0:wrong
//result[1] 步数 steps 
	int result[2] = { 0,0 };

	/*
		请在该位置完成广度优先搜索。
	*/


	if (checkObject(initialNode, objPuzzleNode)) {
		result[0] = 1;
	}
	else {
		result[0] = 0;
	}

	return result;

}

//深度有限搜索
int* depthFirstSearch(PUZZLE_NODE initialNode, PUZZLE_NODE objPuzzleNode) {

	//result[0] 1:correct;0:wrong
	//result[1] 步数 steps 
	int result[2] = { 0,0 };
	/*
		请在该位置完成深度有限搜索，最大深度限度为25。
	*/


	if (checkObject(initialNode, objPuzzleNode) && initialNode.depth < MAX_DEPTH) {
		result[0] = 1;
	}
	else {
		result[0] = 0;
	}

	return result;
}


//启发式搜索1
int* heuristicSearchInformedByIncorrectNum(PUZZLE_NODE initialNode, PUZZLE_NODE objPuzzleNode) {
	//result[0] 1:correct;0:wrong
	//result[1] 步数 steps 
	int result[2] = { 0,0 };

	/*
		请在该位置完成启发式搜索，启发式函数使用不正确位置的数码个数。
	*/


	if (checkObject(initialNode, objPuzzleNode)) {
		result[0] = 1;
	}
	else {
		result[0] = 0;
	}

	return result;
}

//启发式搜素2
int* heuristicSearchInformedByManhattonDis(PUZZLE_NODE initialNode, PUZZLE_NODE objPuzzleNode) {
	//result[0] 1:correct;0:wrong
	//result[1] 步数 steps 
	int result[2] = { 0,0 };
	/*
		请在该位置完成启发式搜索，启发式函数采用到目标位置的曼哈顿距离。
	*/


	if (checkObject(initialNode, objPuzzleNode)) {
		result[0] = 1;
	}
	else {
		result[0] = 0;
	}

	return result;
}

//广度优先搜索
int* binaryFirstSearchDemo(PUZZLE_NODE initialNode, PUZZLE_NODE objPuzzleNode)
{
	//result[0] 1:correct;0:wrong
	//result[1] 步数 steps 
	int result[2] = { 0,0 };

	cout << "初始节点状态：" << endl;
	for (int i = 0; i < 3; i++) {
		cout << " " << initialNode.puzzle[i * 3 + 0].puzzleId << "  " << initialNode.puzzle[i * 3 + 1].puzzleId <<
			"  " << initialNode.puzzle[i * 3 + 2].puzzleId << endl;
	}
	cout << endl;
	/*
		请在该位置完成广度优先搜索函数。
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
			cout << "找到正确结果:" << endl;
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
		cout << "请输入调试设置:" << endl;
		cout<< " -1:退出; " << endl;
		cout << "0:广度优先搜索示例;" << endl;
		cout << "1:广度优先搜索; " << endl;
		cout << "2:深度有限搜索; " << endl;
		cout << "3:启发式搜索1; " << endl;
		cout<<  "4:启发式搜索2):" << endl; 
		
		cin >> setup;
		int backwardSteps;
		cout << "请输入大于等于5小于等于20的回退步数" << endl;
		cin >> backwardSteps;
		while (backwardSteps < 5 || backwardSteps >20) {
			cout << "输入错误，请输入大于等于5小于等于20的回退步数" << endl;
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
			cout << "广度优先搜索示例:" << endl;
			result = binaryFirstSearchDemo(initialNode, objPuzzleNode);
		}
		else {
			cout << "输入设置有误，请重新运行" << endl;
			cout << endl;
			return 0;
		}

		if (result[0] == 1) {
			cout << "结果为correct,步数为" << result[1] << endl;
		}
		else {
			cout << "结果为wrong" << endl;
		}
		cout << "-------------------------------------------------" << endl;

		cout << endl;
	}
	return 0;

}
