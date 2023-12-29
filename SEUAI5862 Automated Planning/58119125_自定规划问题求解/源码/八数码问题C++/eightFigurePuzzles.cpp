#include <time.h>
#include "eightFigurePuzzles.h"
#include <windows.h>

//�ҵ� �ո� 0 ���ڵ�λ��
int* findZeroPosition(PUZZLE_NODE puzzleNode) {
	int res[2] = { 0,0 };
	for (int i = 0; i < puzzleNum + 1; i++) {
		if (puzzleNode.puzzle[i].puzzleId == 0) {
			res[0] = puzzleNode.puzzle[i].xPosition;
			res[1] = puzzleNode.puzzle[i].yPosition;
			return res;
		}
	}
	return res;
}

//����puzzleNode�ĺ�̿ɲ�������״̬������ ��1��0������ո������ƶ�����-1��0������ո������ƶ�����0��1������ո������ƶ�����0��-1������ո������ƶ���
PUZZLE_NODE updatePuzzleNodeActionList(PUZZLE_NODE puzzleNode) {
	int* xyPosition = findZeroPosition(puzzleNode);
	int x = xyPosition[0];
	int y = xyPosition[1];
	if (x >= 1) {
		vector<int> actionUp;
		actionUp.push_back(1);
		actionUp.push_back(0);
		puzzleNode.nextActionList.push_back(actionUp);
	}
	if (x <= 1) {
		vector<int> actionDown;
		actionDown.push_back(-1);
		actionDown.push_back(0);
		puzzleNode.nextActionList.push_back(actionDown);
	}
	if (y >= 1) {
		vector<int> actionLeft;
		actionLeft.push_back(0);
		actionLeft.push_back(1);
		puzzleNode.nextActionList.push_back(actionLeft);
	}
	if (y <= 1) {
		vector<int> actionRight;
		actionRight.push_back(0);
		actionRight.push_back(-1);
		puzzleNode.nextActionList.push_back(actionRight);
	}
	return puzzleNode;
}

void outputAction(vector<int> action, int index) {
	/*cout << action[0] << " " << action[1] << endl;*/
	if (action[0] == 1 && action[1] == 0) {
		cout << "���� " << index << ":�����ƶ�" << endl;
		cout << endl;
	}
	else if (action[0] == -1 && action[1] == 0) {
		cout << "���� " << index << "�����ƶ�" << endl;
		cout << endl;
	}
	else if (action[0] == 0 && action[1] == 1) {
		cout << "���� " << index << "�����ƶ�" << endl;
		cout << endl;
	}
	else {
		cout << "���� " << index << "�����ƶ�" << endl;
		cout << endl;
	}
}

// ��������action��actionΪ��ά���飩��puzzleNode,����ִ�иö������µĽڵ�
PUZZLE_NODE moveToPuzzleNode(vector<int> action, PUZZLE_NODE puzzleNode) {
	//cout << action[0] << " " << action[1] << endl;
	//if (action[0] == 1 && action[1] == 0) {
	//	cout << "�����ƶ�" << endl;
	//}
	//else if (action[0] == -1 && action[1] == 0) {
	//	cout << "�����ƶ�" << endl;
	//}
	//else if (action[0] == 0 && action[1] == 1) {
	//	cout << "�����ƶ�" << endl;
	//}
	//else {
	//	cout << "�����ƶ�" << endl;
	//}

	int* xyPosition = findZeroPosition(puzzleNode);
	int x = xyPosition[0];
	int y = xyPosition[1];
	PUZZLE_NODE nextPuzzleNode;
	for (int xPos = 0; xPos < 3; xPos++) {
		for (int yPos = 0; yPos < 3; yPos++) {
			nextPuzzleNode.puzzle[xPos * 3 + yPos].xPosition = xPos;
			nextPuzzleNode.puzzle[xPos * 3 + yPos].yPosition = yPos;
			if (xPos == x && yPos == y) {
				nextPuzzleNode.puzzle[xPos * 3 + yPos].puzzleId = puzzleNode.puzzle[((x - action[0]) * 3 + (y - action[1]))].puzzleId;
			}
			else if (xPos == (x - action[0]) && yPos == (y - action[1])) {
				nextPuzzleNode.puzzle[xPos * 3 + yPos].puzzleId = puzzleNode.puzzle[(x * 3 + y)].puzzleId;
			}
			else {
				nextPuzzleNode.puzzle[xPos * 3 + yPos].puzzleId = puzzleNode.puzzle[xPos * 3 + yPos].puzzleId;
			}

		}
	}
	return nextPuzzleNode;
}

// ��������PuzzleNode�������������
int getRandomNumber(int actionSize)
{
	int RandomNumber;
	srand((unsigned)time(0));//time()��ϵͳʱ���ʼ���֡�Ϊrand()���ɲ�ͬ��������ӡ�
	RandomNumber = rand() % actionSize;//����1~100�����
	return RandomNumber;
}


//�������˲��������س�ʼ״̬
PUZZLE_NODE initialPuzzleNode(int backwordSteps) {
	PUZZLE_NODE objNode;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			objNode.puzzle[i * 3 + j].puzzleId = i * 3 + j;
			objNode.puzzle[i * 3 + j].xPosition = i;
			objNode.puzzle[i * 3 + j].yPosition = j;
		}
	}
	PUZZLE_NODE initialPuzzleNode = updatePuzzleNodeActionList(objNode);


	for (int i = 0; i < backwordSteps; i++) {
		PUZZLE_NODE precedePuzzleNode = initialPuzzleNode;
		Sleep(100);
		int action = getRandomNumber(initialPuzzleNode.nextActionList.size());
		initialPuzzleNode = moveToPuzzleNode(initialPuzzleNode.nextActionList[action], initialPuzzleNode);
		initialPuzzleNode = updatePuzzleNodeActionList(initialPuzzleNode);
	}
	initialPuzzleNode = updatePuzzleNodeActionList(initialPuzzleNode);
	return initialPuzzleNode;
}

//�ж�����8����״̬�Ƿ���ͬ
bool isEqual(PUZZLE puzzle1, PUZZLE puzzle2) {
	if (puzzle1.xPosition == puzzle2.xPosition && puzzle1.yPosition == puzzle2.yPosition
		&& puzzle1.puzzleId == puzzle2.puzzleId)
		return true;
	else
		return false;
}

//��⵱ǰ�ڵ��Ŀ��ڵ�״̬�Ƿ���ͬ
bool checkObject(PUZZLE_NODE currentNode, PUZZLE_NODE objNode) {
	for (int i = 0; i < puzzleNum + 1; i++) {
		if (!isEqual(currentNode.puzzle[i], objNode.puzzle[i]))
			return false;
	}
	return true;
}

//�жϵ�ǰ�ڵ�״̬�Ƿ񱻷��ʹ���
int visitedNum(PUZZLE_NODE puzzleNode) {
	int mapValue = 0;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			mapValue = mapValue * 10 + puzzleNode.puzzle[i * 3 + j].puzzleId;
		}
	}
	return mapValue;
}