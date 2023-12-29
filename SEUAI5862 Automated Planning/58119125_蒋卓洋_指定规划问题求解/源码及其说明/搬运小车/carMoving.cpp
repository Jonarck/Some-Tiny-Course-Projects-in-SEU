// carMoving.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <stack>
#include <vector>
#include <map>
#include <queue>
using namespace std;

#define MAX 2

const int  carNum = 4;
/*
*描述：记录每一辆车的编号。
*/
struct CAR {
	int carId;
};

/*
*描述：声明一个节点，存储当前停车栈状态
*      以目标状态为例，
				  carPort: {stack0:[CAR1]
							stack1:[CAR2,CAR3]
							stack2:[CAR0]
							} 

				  nextActionList: {[0,1] 栈0->栈1
								   [0,2] 栈0->栈2
								   [1,0] 栈1->栈0
								   [1.2] 栈1->栈2
								   [2,0] 栈2->栈0
								   [2,1]}栈2->栈1

				  nextAxtionList的大小<=4

				  depth： 当前状态所处深度
*/
struct CAR_NODE {
	vector<stack<CAR>> carPort;
	vector<vector<int>> nextActionList;
	vector<vector<int>> precedeActionList;
	int depth;
};

// 用于记录当前状态是否被访问过。
map<int, int> visited;


// 打印节点状态
void outputNode(CAR_NODE carNode);

// 更新carNode的后继可操作动作状态，
CAR_NODE updateCarNodeActionList(CAR_NODE carNode);

// 移动操作
vector<stack<CAR>> move(vector<stack<CAR>> state, vector<int>& action);
// 给定动作action（action为[pre,post]）和carNode,返回执行该动作后新的carNode
CAR_NODE moveToCarNode(vector<int>& action, CAR_NODE carNode);

// 用于生成当前状态对应的唯一特征数，用于main中visited判断当前节点状态是否访问过。
int visitedNum(CAR_NODE carNode);

// 打印操作列
void outputActions(CAR_NODE beginCarNode, CAR_NODE finalCarNode);

//广度优先搜索
int* binaryFirstSearch(CAR_NODE initialNode, CAR_NODE objNode);


int main()
{
	visited.clear();
	CAR car0, car1, car2, car3;
	car0.carId = 0;
	car1.carId = 1;
	car2.carId = 2;
	car3.carId = 3;
	//设定目标状态
	CAR_NODE objNode;
	stack<CAR> objStack1, objStack2, objStack3;
	objStack1.push(car1);
	objStack2.push(car2); objStack2.push(car3);
	objStack3.push(car0); 
	objNode.carPort.push_back(objStack1);
	objNode.carPort.push_back(objStack2);
	objNode.carPort.push_back(objStack3);
	//设定初始状态
	CAR_NODE initialNode;
	stack<CAR> iniStack1, iniStack2, iniStack3;
	iniStack1.push(car0); iniStack1.push(car1);
	iniStack2.push(car2); iniStack2.push(car3);
	initialNode.carPort.push_back(iniStack1);
	initialNode.carPort.push_back(iniStack2);
	initialNode.carPort.push_back(iniStack3);

	//广度优先搜索
	int* result;
	result = binaryFirstSearch(initialNode, objNode);
		
	return 0;
}

// 打印节点状态
void outputNode(CAR_NODE carNode) {
	vector<stack<CAR>> port = carNode.carPort;
	for (int i = 0; i < carNode.carPort.size(); i++) {
		stack<int> output;
		for (int j = 0; j < carNode.carPort[i].size(); j++) {
			if (carNode.carPort[i].empty()) continue;
			output.push(port[i].top().carId);
			port[i].pop();
		}
		cout << " ------ ------ " << endl;
		if (output.empty()) {
			cout << "|      |      |" << endl;
			cout << " ------ ------ " << endl;
		}
		for (int j = 0; j < output.size(); j++) {
			if (output.size() == 1) {
				cout << "| car" << output.top() << " |      |" << endl;
				cout << " ------ ------ " << endl;
				output.pop();
			}
			else if (output.size() == 2) {
				cout << "| car" << output.top() << " ";
				output.pop();
				cout << "| car" << output.top() << " |" << endl;
				cout << " ------ ------ " << endl;
				output.pop();
			}
		}
	}
	cout << "__________________________________________" << endl;
}

// 更新当前状态下carNode（停车场）的后继可操作动作状态，
CAR_NODE updateCarNodeActionList(CAR_NODE carNode) {
	vector<int> portPost;//终点栈
	vector<int> portPre;//起点栈
	for (int x = 0; x < carNode.carPort.size(); x++) {//找出可以停入和可以驶出的停车栈
		int y = carNode.carPort[x].size();
		if (y != 2) portPost.push_back(x);//终点栈不满
		if (y != 0) portPre.push_back(x);//起点栈不空
	}
	for (int pre = 0; pre < portPre.size(); pre++) {//对可行的起点栈和终点栈进行组合，传入可行操作列表
		for (int post = 0; post < portPost.size(); post++) {
			if (portPre[pre] != portPost[post]) {
				carNode.nextActionList.push_back({ portPre[pre], portPost[post] });
			}
		}
	}
	return carNode;
}

// 移动操作
vector<stack<CAR>> move(vector<stack<CAR>> state, vector<int>& action) {
	state[action[1]].push(state[action[0]].top());
	action.push_back(state[action[0]].top().carId);//将本次移动到小车传入action中
	state[action[0]].pop();
	return state;
}

// 给定动作action（action为[pre,post]）和carNode,返回执行该动作后新的carNode
CAR_NODE moveToCarNode(vector<int>& action, CAR_NODE carNode) {
	CAR_NODE nextCarNode;
	nextCarNode.carPort = move(carNode.carPort, action);
	return nextCarNode;
}


// 打印操作列
void outputActions(CAR_NODE beginCarNode, CAR_NODE finalCarNode) {
	for (int i = 0; i < finalCarNode.precedeActionList.size(); i++) {

		vector<int>action = finalCarNode.precedeActionList[i];
		cout << endl;
		cout << "第" << i + 1 << "步移动操作：" << endl;
		cout << "将小车" << action[2] << "从停车道" << action[0] + 1 << "移动到停车道" << action[1] + 1 << endl;
		cout << endl;
		cout << "第" << i + 1 << "步移动结果：" << endl;
		beginCarNode = moveToCarNode(action, beginCarNode);
		outputNode(beginCarNode);
	}
}

// 判断两个节点状态是否相同，对应栈中元素数量相同+对应栈顶元素相同
bool checkObject(CAR_NODE carNode1, CAR_NODE carNode2) {
	for (int i = 0; i < carNode1.carPort.size(); i++) {
		if (carNode1.carPort[i].size() == carNode2.carPort[i].size()) {
			if (carNode1.carPort[i].empty()) {
				continue;
			}
			if (carNode1.carPort[i].top().carId == carNode2.carPort[i].top().carId) {
				continue;
			}
			return false;
		}
		else {
			return false;
		}
	}
	return true;
}


// 用于生成当前状态对应的唯一特征数，用于main中visited判断当前节点状态是否访问过。
int visitedNum(CAR_NODE carNode) {
	int numFeatureValue = 0;//栈内数量特征数（x位数）
	for (int i = 0; i < carNode.carPort.size(); i++) {
		numFeatureValue += carNode.carPort[i].size() * pow(10, i);
	}
	int topFeatureValue = 0;//栈顶元素特征数（x位数）
	for (int i = 0; i < carNode.carPort.size(); i++) {
		if (carNode.carPort[i].empty()) {
			topFeatureValue += 0;
		}
		else {
			topFeatureValue += (carNode.carPort[i].top().carId + 1) * pow(10, i);
		}

	}
	return numFeatureValue * pow(10, carNode.carPort.size()) + topFeatureValue;//返回状态特征数
}

//广度优先搜索
int* binaryFirstSearch(CAR_NODE initialNode, CAR_NODE objNode)
{
	//result[0] 1:correct;0:wrong
	//result[1] 步数 steps 
	int result[2] = { 0,0 };

	cout << "初始节点状态：" << endl;
	outputNode(initialNode);

	CAR_NODE carNode = initialNode;
	//广度优先队列
	queue<CAR_NODE> carNodeQueue;
	carNode.depth = 0;
	int depth = 0;
	carNodeQueue.push(carNode);

	while (carNodeQueue.size()) {//队列不为空时
		//队首节点作为当前节点
		CAR_NODE currentCarNode = carNodeQueue.front();

		if (checkObject(currentCarNode, objNode)) {//与目标结果相同，则返回
			cout << endl;
			cout << "移动操作过程:" << endl;
			outputActions(initialNode, currentCarNode);
			cout << endl;
			cout << "找到正确结果:" << endl;
			outputNode(currentCarNode);
			result[0] = 1;
			result[1] = currentCarNode.depth;
			return result;
		}
		else {//未到达目标

			//节点状态标记为已访问
			visited[visitedNum(currentCarNode)] = 1;

			//计算并更新当前节点的下一步状态集
			if (currentCarNode.nextActionList.size() == 0) {
				currentCarNode = updateCarNodeActionList(currentCarNode);
			}
			//当前节点
			carNodeQueue.pop();

			//对当前节点依次尝试可行动作，依次到达下一个节点
			for (int i = 0; i < currentCarNode.nextActionList.size(); i++) {
				//生成动作执行后到达的邻接节点并存储
				CAR_NODE nextCarNode = moveToCarNode(currentCarNode.nextActionList[i], currentCarNode);

				//邻接节点的前置前点列表复制
				if (!currentCarNode.precedeActionList.empty()) {
					for (int actionIndex = 0; actionIndex < currentCarNode.precedeActionList.size(); actionIndex++)
					{
						nextCarNode.precedeActionList.push_back(currentCarNode.precedeActionList[actionIndex]);
					}
				}
				//下一个节点的前置前点列表更新（加入当前节点）
				nextCarNode.precedeActionList.push_back(currentCarNode.nextActionList[i]);

				//若本次遍历到的邻接节点已访问过，则进行下一次循环，遍历下一个邻接节点
				if (visited[visitedNum(nextCarNode)] == 1) {
					continue;
				}

				//若本次遍历到的邻接节点未曾访问过，则将该邻接节点加入广度优先搜索队列等待访问，该邻接节点的搜索深度加1
				nextCarNode.depth = currentCarNode.depth + 1;
				carNodeQueue.push(nextCarNode);

			}
		}

	}
	cout << "本问题无解";
	return result;
}