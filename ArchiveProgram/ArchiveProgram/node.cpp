#include "stdafx.h"
#include "node.h"

Node::Node(Node *Left, Node *Right)
{
	left = Left;
	right = Right;
	weight = Left->weight + Right->weight;
	sheet = false;
}
Node::Node() {}

bool Compare::operator()(Node *left, Node *right) const
	{
		return left->weight < right->weight;
	}