#define _CRT_SECURE_NO_WARNINGS
#include <conio.h>
#include <iostream>
#include <vector>

#include "LittleMethod.h"

using namespace std;

vector<vector<int>> cost = {
    {INT_MAX, 21, 15, 27, 26, 24},
    {15, INT_MAX, 12, 8, 17, 22},
    {9, 18, INT_MAX, 23, 0, 24},
    {5, 10, 26, INT_MAX, 7, 20},
    {23, 17, 15, 27, INT_MAX, 18},
    {24, 27, 11, 3, 26, INT_MAX}
};

vector<vector<int>> cost1 = {
    {INT_MAX, 27, 43, 16, 30, 26},
    {7, INT_MAX, 16, 1, 30, 25},
    {20, 13, INT_MAX, 35, 5, 0},
    {21, 16, 25, INT_MAX, 18, 18},
    {12, 46, 27, 48, INT_MAX, 5},
    {23, 5, 5, 9, 5, INT_MAX}
};


int main()
{
    LittleMethod* s = new LittleMethod(cost);
    int result = s->Solve();
    cout << "\033[31m" << "Exclude edge - red\n";
    cout << "\033[32m" << "Include edge - green\n" << "\033[0m";
    BinaryTree::print(s->root);
    cout << "\033[35m" << "RESULT optimal cost: " << "\033[0m" << result;

    return 0;
}
