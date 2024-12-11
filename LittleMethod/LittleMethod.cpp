#include "LittleMethod.h"
#include <algorithm>

int LittleMethod::getMinInRow(int row, int excludeIndex)
{
	int minCol = 0;
	for (int j = 0; j < reduceCost[0].size(); j++)
		if (j != excludeIndex && reduceCost[row][j] < reduceCost[row][minCol]) minCol = j;
	return reduceCost[row][minCol];
}

int LittleMethod::getMinInCol(int col, int excludeIndex)
{
	int minRow = 0;
	for (int i = 0; i < reduceCost.size(); i++)
		if (i != excludeIndex && reduceCost[i][col] < reduceCost[minRow][col]) minRow = i;
	return reduceCost[minRow][col];
}

LittleMethod::LittleMethod(vector<vector<int>> cost)
{
	this->cost = cost;
	reduceCost = cost;
}


void LittleMethod::reduceMatrix()
{
	currentR = 0;
	// вычитаем минимальный элемент по строкам
	for (auto& row : reduceCost) {
		auto minIter = min_element(row.begin(), row.end());
		currentR += *minIter;
		for (auto& el : row) el -= *minIter;
	}

	// вычитаем минимальный элемент по столбцам
	for (int j = 0; j < reduceCost[0].size(); j++) {\
		// найдем минимальный в столбце
		int minEl = getMinInCol(j);
		currentR += minEl;
		// вычтем его из каждого элемента столбца
		for (int i = 0; i < reduceCost.size(); i++)
			reduceCost[i][j] -= minEl;
 	}
}

void LittleMethod::calculateFint()
{
	currentFint = vector<vector<int>>(cost.size(), vector<int>(cost[0].size(), 0));
	for (int i = 0; i < reduceCost.size(); i++) {
		for (int j = 0; j < reduceCost[0].size(); j++) {
			if (reduceCost[i][j]) continue;
			int minRowEl = getMinInRow(i, j);
			int minColEl = getMinInCol(j, i);
			currentFint[i][j] = minRowEl + minColEl;
		}
	}
}
