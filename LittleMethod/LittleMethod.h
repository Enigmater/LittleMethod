#pragma once
#include <vector>
#include <algorithm>
#include "BinaryTree.h"

using namespace std;

class LittleMethod {
private:
	vector<vector<int>> cost;
	int n;

	vector<pair<int, int>> path;
	int bestCost;

	int getMinInRow(const vector<vector<int>>& cost, int row, int excludeIndex = -1);
	int getMinInCol(const vector<vector<int>>& cost, int col, int excludeIndex = -1);
public:
	Branch* root;
	LittleMethod(vector<vector<int>> cost);
	int Solve();
	int getTotalCost(vector<pair<int, int>> path);
	int reduceMatrix(vector<vector<int>>& reduceCost);					 // приведение матрицы
	pair<int, pair<int, int>> calculateFint(const vector<vector<int>>& reduceCost); // вычислить штраф (вернуть клектку с максимальным)
	void setLeftMatrix(Branch* left, pair<int, int> maxFint);
	void setRightMatrix(Branch* right, pair<int, int> maxFint);
	Branch* findMinLeaf(const vector<Branch*>& leaves);
	void removeLeaf(vector<Branch*>& leaves, Branch* leaf);
	void printPath(vector<pair<int, int>> path);

};

