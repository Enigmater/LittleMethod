#pragma once
#include <vector>

using namespace std;

class LittleMethod {
private:
	vector<vector<int>> cost;
	vector<vector<int>> reduceCost;
	vector<vector<int>> currentFint;

	int currentR;

	int getMinInRow(int row, int excludeIndex = -1);
	int getMinInCol(int col, int excludeIndex = -1);
public:
	LittleMethod(vector<vector<int>> cost);
	void reduceMatrix();  // приведение матрицы
	void calculateFint(); // вычислить штраф
};

