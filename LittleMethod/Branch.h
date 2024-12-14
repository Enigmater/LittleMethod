#pragma once
#include <vector>
#include <unordered_map>
#include <unordered_set>

using namespace std;

class Branch {
public:
	Branch(vector<vector<int>> cost, bool includeEdge, pair<int, int> edge) {
		this->cost = cost;
		this->includeEdge = includeEdge;
		this->edge = edge;
		leftBr = nullptr;
		rightBr = nullptr;
		parent = nullptr;
	}

	vector<vector<int>> cost;
	int border;
	bool includeEdge;
	pair<int, int> edge;

	Branch* leftBr;
	Branch* rightBr;
	Branch* parent;

	bool allIsNull();

	// Функция для восстановления пути от текущей вершины до корня
	vector<pair<int, int>> getPath();

	vector<pair<int, int>> buildRoute(const vector<pair<int, int>>& edges);

	// Метод для построения пути, начиная с заданного ребра
	vector<pair<int, int>> buildPathFromEdge(const vector<pair<int, int>>& edges, const pair<int, int>& startEdge);

	// Метод для построения пути, заканчивающегося заданным ребром
	vector<pair<int, int>> buildPathToEdge(const vector<pair<int, int>>& edges, const pair<int, int>& endEdge);

	void printCost();
};


