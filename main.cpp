//Data structure HW4-2
//B10815026 ¹ù¾¥­õ

#include<iostream>
#include<vector>
#include<set>
#include<map>
#include<queue>
#include<unordered_map>
#include<iomanip>
#include<cmath>
#include<algorithm>

using namespace std;

struct Edge {
	Edge(int v1, int v2, double probability, bool enabled) :
		v1(v1),
		v2(v2),
		probability(probability),
		enabled(enabled) {}
	int v1;
	int v2;
	double probability;
	bool enabled;
};

struct NodeData {
	vector<Edge*> edges;
	int treeId;
};

void sortEdges(vector<Edge*>& edges) {
	sort(edges.begin(), edges.end(), [](Edge* e1, Edge* e2) {return e1->probability > e2->probability; });
}

queue<Edge*> generateQueue(vector<Edge*> edges) {
	queue<Edge*> edgeQueue;
	for (auto it = edges.begin(); it != edges.end(); ++it) {
		edgeQueue.push(*it);
	}
	return edgeQueue;
}

double findPath(map<int, NodeData> allNodes, int previousNode, int currentNode, double currentProbability) {
	//check if we reach the destination
	if (currentNode == 2) {
		return currentProbability;
	}

	//check if the current node is the leaf node
	if (allNodes[currentNode].edges.size() == 1) {
		//not found the destination
		return -1;
	}

	double result = -INFINITY;
	//iterate all the enabled edges of the current node
	for (int i = 0; i < allNodes[currentNode].edges.size(); ++i) {
		if (allNodes[currentNode].edges[i]->enabled) {
			Edge* currentEdge = allNodes[currentNode].edges[i];
			int nextNode;
			if (currentEdge->v1 == currentNode) nextNode = currentEdge->v2;
			else nextNode = currentEdge->v1;

			//check if the nextNode is the previous node, prevent circular recursive
			if (previousNode == nextNode) {
				continue;
			}

			//go to next node
			double temp = findPath(allNodes, currentNode, nextNode, currentProbability * currentEdge->probability);
			result = max(result, temp);
		}
	}

	return result;
}

int main() {
	map<int, NodeData> allNodes;
	vector<Edge*> allEdges;
	queue<Edge*> edgeQueue;
	map<int, vector<int>> forest;
	vector<Edge*> MST;

	int edgeNum;
	cin >> edgeNum;
	for (int i = 0; i < edgeNum; ++i) {
		int v1, v2;
		double probability;
		cin >> v1 >> v2 >> probability;
		Edge* newEdge = new Edge(v1,v2,probability,false);
		allEdges.push_back(newEdge);

		allNodes[v1].edges.push_back(newEdge);
		allNodes[v2].edges.push_back(newEdge);
	}

	//initialize the forest
	int treeId = 0;
	for (auto it = allNodes.begin(); it != allNodes.end(); ++it) {
		forest[treeId].push_back(it->first);
		it->second.treeId = treeId;
		++treeId;
	}

	//insert all edges into the queue
	sortEdges(allEdges);
	edgeQueue = generateQueue(allEdges);

	while (!edgeQueue.empty()) {
		Edge* currentEdge = edgeQueue.front();
		edgeQueue.pop();

		//check if this edge connect two differenct tree
		int treeId1 = allNodes[currentEdge->v1].treeId;
		int treeId2 = allNodes[currentEdge->v2].treeId;
		if (treeId1 != treeId2) {
			//make current edge enabled
			currentEdge->enabled = true;
			MST.push_back(currentEdge);

			//merge two tree with different ID
			for (int i = 0; i < forest[treeId2].size(); ++i) {
				int nodeName = forest[treeId2][i];
				allNodes[nodeName].treeId = treeId1;
			}
			//add the nodes in tree2 into tree1 and delete tree2
			forest[treeId1].insert(forest[treeId1].end(), forest[treeId2].begin(), forest[treeId2].end());
			forest.erase(treeId2);
		}
		else {
			//nothing happen
		}
	}

	//find the path from node 1 to node 2
	double result = 1;

	for (int i = 0; i < MST.size(); ++i) {
		result *= MST[i]->probability;
	}

	if (result < 0.05) {
		cout << 0;
	}
	else {
		result = round(result * 10000.0) / 10000.0;
		cout << setprecision(4) << result;
	}
	
}