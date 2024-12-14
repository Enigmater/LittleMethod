#include "LittleMethod.h"
#include <algorithm>
#include <queue>

const bool PRINT_REDUCE = false;
const bool PRINT_REDUCE_R = true;
const bool PRINT_COST = true;
const bool PRINT_TREE_FOR_STEP = true;
const bool PRINT_PATH = true;
const bool PRINT_FINT = true;

int LittleMethod::getMinInRow(const vector<vector<int>>& cost, int row, int excludeIndex)
{
	int minCol = 0;
	while (minCol == excludeIndex) minCol++;
	for (int j = 0; j < n; j++)
		if (j != excludeIndex && cost[row][j] < cost[row][minCol]) minCol = j;
	return cost[row][minCol];
}

int LittleMethod::getMinInCol(const vector<vector<int>>& cost, int col, int excludeIndex)
{
	int minRow = 0;
	while (minRow == excludeIndex) minRow++;
	for (int i = 0; i < n; i++)
		if (i != excludeIndex && cost[i][col] < cost[minRow][col]) minRow = i;
	return cost[minRow][col];
}

LittleMethod::LittleMethod(vector<vector<int>> cost)
{
	this->cost = cost;
	n = cost.size();
	bestCost = INT_MAX;
}

int LittleMethod::Solve()
{
	// TODO: реализовать метод ветвей и границ для решения задачи коммивояжера 
	// 0. инициализация root (делим на поддеревья)
	// 1. Если маршрут построен не до конца (checkComplete), то выбираем поддерево (из всех листовых вершин) с наименьшей границей (border)
	// 2. Если поддерево левый потомок, то матрица преобразуется setLeftMatrix, иначе - setRightMatrix
	// если левое, то редуцируем (reduceMatrix), иначе сразу приступаем к высчитыванию штрафов (calculateFint) и далее делим на левое и правое дерево (примерно как в initRoot)
	// повторяем, пока в матрице не останется 1 возможное ребро (шаг №1 - checkComplete)
	
	root = new Branch(cost, false, {});

	if (PRINT_COST) {
		cout << "Matrix cost:\n";
		root->printCost();
	}

	root->border = reduceMatrix(root->cost);
	
	// Начинаем с корня дерева и продолжаем разбиение
	vector<Branch*> leaves;  // Массив для хранения всех листьев
	leaves.push_back(root);  // Добавляем корень в массив листьев

	while (true) {
		if (PRINT_TREE_FOR_STEP) {
			cout << "\033[33m" << "TREE:\n" << "\033[0m";
			BinaryTree::print(root);
		}

		// Находим лист с минимальной границей
		Branch* minBranch = findMinLeaf(leaves);

		// Пытаемся построить  маршрут
		auto path = minBranch->getPath();


		if (PRINT_PATH) {
			cout << "\033[33m" << "Current PATH:\n\t" << "\033[0m";
			printPath(path);
		}

		if (PRINT_COST) {
			cout << "\nReduced matrix (r = " << minBranch->border;
			if (minBranch->parent) {
				cout << ", ";
				if (minBranch->includeEdge)cout << "\033[32m" << "INCLUDE";
				else cout << "\033[31m" << "EXCLUDE";
				cout << "\033[0m" << " edge: (" << minBranch->edge.first + 1<< ", " << minBranch->edge.second + 1 << ")";
			}
			cout << ")\n";
			minBranch->printCost();
		}

		auto route = minBranch->buildRoute(path);
		if (route.size() == n) {
			cout << "\033[33m" << "ROUTE example:\n\t" << "\033[0m";
			printPath(route);
			return getTotalCost(route);
		}
		
		// Выбираем максимальный штраф - {X, {A, B}}, где X - штраф, A B - ячейка
 		auto maxFint = calculateFint(minBranch->cost); 

		if (PRINT_FINT) {
			cout << "Max FINT = " << maxFint.first << " in cell (" << maxFint.second.first + 1 << ", " << maxFint.second.second + 1 << ")\n";
		}

		// Разбиваем лист
		if (!minBranch->leftBr) {
 			Branch* leftBranch = new Branch(minBranch->cost, false, maxFint.second);
			minBranch->leftBr = leftBranch;
			leftBranch->parent = minBranch;

			setLeftMatrix(leftBranch, maxFint.second);
			if (PRINT_REDUCE) cout << "Reduce for left branch:\n";
			reduceMatrix(leftBranch->cost);
			leftBranch->border = minBranch->border + maxFint.first;

			if (PRINT_REDUCE_R) cout << "LEFT branch r = " << minBranch->border << " + " << maxFint.first << " = " << leftBranch->border << "\n";

			leaves.push_back(leftBranch);
		}
		if (!minBranch->rightBr) {
			Branch* rightBranch = new Branch(minBranch->cost, true, maxFint.second);
			minBranch->rightBr = rightBranch;
			rightBranch->parent = minBranch;

			setRightMatrix(rightBranch, maxFint.second);
			if (PRINT_REDUCE) cout << "\nReduce for right branch:\n";
			int r = reduceMatrix(rightBranch->cost);
			rightBranch->border = r + minBranch->border;

			if (PRINT_REDUCE_R) cout << "RIGHT branch r = " << minBranch->border << " + " << r << " = " << rightBranch->border << "\n";

			leaves.push_back(rightBranch);
		}

		// Удаляем текущий лист из массива
		removeLeaf(leaves, minBranch); 
	}
}

int LittleMethod::getTotalCost(vector<pair<int, int>> path)
{
	int totalCost = 0;
	for (auto& e : path) 
		totalCost += cost[e.first][e.second];  // Суммируем стоимость каждого ребра
	return totalCost;
}

int LittleMethod::reduceMatrix(vector<vector<int>>& reduceCost)
{
	int currentR = 0;
	// вычитаем минимальный элемент по строкам
	int rowId = 1;
	for (auto& row : reduceCost) {
		auto minIter = min_element(row.begin(), row.end());
		int min = *minIter;

		if (PRINT_REDUCE) {
			cout << "Min el in row #" << rowId++ << ": ";
			if (min == INT_MAX) cout << "M\n";
			else cout << min << "\n";
		}

		if (min == INT_MAX) continue;
		currentR += min;
		for (auto& el : row) if (el != INT_MAX) el -= min;
	}

	// вычитаем минимальный элемент по столбцам
	for (int j = 0; j < reduceCost[0].size(); j++) {
		// найдем минимальный в столбце
		int minEl = getMinInCol(reduceCost, j);

		if (PRINT_REDUCE) {
			cout << "Min el in col #" << j + 1 << ": ";
			if (minEl == INT_MAX) cout << "M\n";
			else cout << minEl << "\n";
		}

		if (minEl == INT_MAX) continue;
		currentR += minEl;
		// вычтем его из каждого элемента столбца
		for (int i = 0; i < reduceCost.size(); i++) if (reduceCost[i][j] != INT_MAX) reduceCost[i][j] -= minEl;
 	}

	return currentR;
}

pair<int, pair<int, int>> LittleMethod::calculateFint(const vector<vector<int>>& reduceCost)
{
	pair<int, int> v = { 0, 0 };
	int max = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (reduceCost[i][j]) continue;
			int minRowEl = getMinInRow(reduceCost, i, j);
			int minColEl = getMinInCol(reduceCost, j, i);
			int temp = minRowEl + minColEl;

			if (temp > max) {
				max = temp;
				v = { i, j };
			}
		}
	}
	return { max, v };
}

void LittleMethod::setLeftMatrix(Branch* left, pair<int, int> maxFint)
{
	left->cost[maxFint.first][maxFint.second] = INT_MAX;
}

void LittleMethod::setRightMatrix(Branch* right, pair<int, int> maxFint)
  {
	for (int j = 0; j < n; j++) right->cost[maxFint.first][j] = INT_MAX;
	for (int i = 0; i < n; i++) right->cost[i][maxFint.second] = INT_MAX;
	right->cost[maxFint.second][maxFint.first] = INT_MAX;

	auto path = right->getPath();
	auto pathFrom = right->buildPathFromEdge(path, maxFint);
	auto pathEnd = right->buildPathToEdge(path, maxFint);
	if (pathFrom.size() > 0) {
		int from = pathFrom[pathFrom.size() - 1].second;
		int to = pathFrom[0].first;
		right->cost[from][to] = INT_MAX;
	}
	else if (pathEnd.size() > 0) {
		int from = pathEnd[pathEnd.size() - 1].second;
		int to = pathEnd[0].first;
		right->cost[from][to] = INT_MAX;
	}
}

Branch* LittleMethod::findMinLeaf(const vector<Branch*>& leaves)
{
	Branch* minBranch = nullptr;
	int minBorder = INT_MAX;

	// Ищем ветвь с минимальной границей среди всех листьев
	for (auto& branch : leaves) {
		if (branch->border < minBorder) {
			minBorder = branch->border;
			minBranch = branch;
		}
	}

	return minBranch;
}

void LittleMethod::removeLeaf(vector<Branch*>& leaves, Branch* leaf)
{
	// Находим и удаляем лист из массива листьев
	auto it = find(leaves.begin(), leaves.end(), leaf);
	if (it != leaves.end()) leaves.erase(it);
}

void LittleMethod::printPath(vector<pair<int, int>> path)
{
	for (const auto& point : path) 
		cout << "(" << point.first + 1 << ", " << point.second + 1 << ") ";
	cout << "\n";
}
