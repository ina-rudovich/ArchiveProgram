#pragma once

typedef unsigned char uchar;

class Node
{
public:
	int weight;
	uchar byte;
	Node *left, *right;
	bool sheet;

	Node(Node *Left, Node *Right)
	{
		left = Left;
		right = Right;
		weight = Left->weight + Right->weight;
		sheet = false;
	}
	Node() {}
};

struct Compare
{
	bool operator()(Node *left, Node *right) const
	{
		return left->weight < right->weight;
	}
};
