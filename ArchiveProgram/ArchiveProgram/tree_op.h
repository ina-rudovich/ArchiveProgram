#pragma once
#include "stdafx.h"
#include<stdio.h>
#include <vector>
#include <map>
#include <list>
#include "node.h"
using namespace std;
typedef unsigned char uchar;

map<uchar, int> frequency_of_occurrences(vector<uchar> &buffer);
Node* build_tree(map<uchar, int> &rates, Node* root);