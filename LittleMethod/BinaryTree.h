#pragma once
#include <iostream>
#include <fstream>
#include "Branch.h"

using namespace std;

// Unicode псевдографика
const string
	ch_hor = "\xC4" /*─*/,
	ch_ver = "\xB3" /*│*/,
	ch_ddia = "\xDA" /*┌*/,
	ch_rddia = "\xBF" /*┐*/,
	ch_udia = "\xC0" /*└*/,
	ch_ver_hor = "\xC3\xC4" /*├─*/,
	ch_udia_hor = "\xC0\xC4" /*└─*/,
	ch_ddia_hor = "\xDA\xC4" /*┌─*/,
	ch_ver_spa = "\xB3 " /*│ */;


class BinaryTree
{
public:
	int size;
	Branch* root;
	BinaryTree();
	~BinaryTree();

	static void print(Branch* branch, string const& rpref = "\t", string const& cpref = "\t", string const& lpref = "\t");

	void destroyBranch(Branch* branch);
};

