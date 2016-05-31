#pragma once

typedef unsigned char uchar;

class Node
{
public:
	int weight;
	uchar byte;
	Node *left, *right;
	bool sheet;

	Node(Node *Left, Node *Right);
	Node();
};

struct Compare
{
	bool operator()(Node *left, Node *right) const;
};
