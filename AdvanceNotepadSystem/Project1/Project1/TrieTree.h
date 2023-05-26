#pragma once
#ifndef TrieTree_h
#define TrieTree_h
#include <iostream>
using namespace std;
struct node
{
	node* alphabets[26];
	char key;
	bool Isword;
	string msg;
	node(char k = NULL);
};

class TrieTree
{
	node* root;
	// suggestion array of string to store suggestion
	string suggestion[11];
	int count;

public:
	TrieTree();

	node* GetRoot()
	{
		return root;
	}
	// load data from file
	void LoadData(string str);
	// this function return array of suggestion
	string* suggest(string str);
	// check the node is laef or not
	bool isleaf(node* r);

private:
	// display function of tree
	void print(node* head = NULL);
};

#endif // !TrieTree_h