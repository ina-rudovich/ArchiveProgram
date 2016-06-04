#include "stdafx.h"
#include "tree_op.h"

/*counting amount of times that bytes appeared in file*/
map<uchar, int> frequency_of_occurrences(vector<uchar> &buffer)
{
	map<uchar, int> rates;

	for (size_t i = 0; i < buffer.size(); i++)
	{
		rates[buffer[i]]++;
	}

	return rates;
}

/*create tree*/
Node* build_tree(map<uchar, int> &rates, Node* root)
{
	list <Node*> nodes;

	map<uchar, int>::iterator i;
	for (i = rates.begin(); i != rates.end(); i++)
	{
		Node *p = new Node;
		p->byte = i->first;
		p->weight = i->second;
		p->left = NULL;
		p->right = NULL;
		p->sheet = true;
		nodes.push_back(p);
	}

	while (nodes.size() != 1)
	{
		nodes.sort(Compare());

		Node *left_son = nodes.front();
		nodes.pop_front();
		Node *right_son = nodes.front();
		nodes.pop_front();

		Node *parent = new Node(left_son, right_son);
		nodes.push_front(parent);
	}

	root = nodes.front();
	nodes.pop_front();
	return root;
}
