#include "TrieTree.h"

// Node constructor
node::node(char k)
{
	for (int i = 0; i < 26; i++)
	{
		alphabets[i] = NULL;
	}
	key = k;
	Isword = false;
}

// Trie Tree Methods
TrieTree::TrieTree()
{
	root = new node();
	count = 0;
	for (int i = 0; i < 26; i++)
	{
		root->alphabets[i] = NULL;
		root->Isword = false;
		root->key = NULL;
	}
}

// load data from file
void TrieTree::LoadData(string str)
{

	int i = 0;
	int pos = 0;
	node* temp = root;
	while (str[i] != '\0')
	{
		if (isupper(str[i]))
		{
			str[i] = str[i] + 32;
		}
		pos = int(str[i]) - 97;
		if (temp->alphabets[pos] == NULL)
		{
			temp->alphabets[pos] = new node(str[i]);
			for (int j = 0; j < i; j++)
			{
				if (temp->msg == "")
					temp->msg += str[j];
			}
		}
		temp = temp->alphabets[pos];
		i++;
	}
	temp->msg = "";
	temp->msg = str;
	temp->Isword = true;
}

// this function return suggestion array

string* TrieTree::suggest(string str)
{
	node* temp = root;
	int pos = 0;
	int i = 0;
	for (int i = 0; i < 10; i++)
	{
		suggestion[i] = "NULL";
	}

	suggestion[10] = "false";
	while (str[i] != NULL)
	{
		if (islower(str[i]))
		{
			str[i] = str[i] - 32;
		}
		pos = int(str[i]) - 65;
		if (temp != NULL)
			temp = temp->alphabets[pos];
		i++;
	}
	count = 0;
	print(temp);
	return suggestion;
}

// this function check that the node is leaf or not
bool TrieTree::isleaf(node* r)
{
	int i = 0;
	while (i < 26)
	{
		if (r->alphabets[i] != NULL)
			return true;
		i++;
	}
	return false;
}

// display function
void TrieTree::print(node* head)
{
	if (head == NULL)
		return;
	int i = 0;
	bool flag = false;
	while (i < 26)
	{
		if (head->Isword == true && flag == false)
		{
			if (count < 10)
			{
				//cout << head->msg << endl;
				suggestion[count] = head->msg;
				suggestion[10] = "true";
			}
			count++;
			flag = true;
			if (!isleaf(head))
			{
				return;
			}
		}
		if (head->alphabets[i] != NULL)
		{
			print(head->alphabets[i]);
		}
		i++;
	}
}