#pragma once
#include "stdafx.h"
#include "node.h"
#include <stdio.h>
#include <vector>
#include <map>

using namespace std;

void association_code_byte(Node* root, map<uchar, vector<bool> > &table, vector <bool> &code);
vector<uchar> compiling_an_array_of_bytes(vector<bool> bin_mass);
vector<bool> encryption(map<uchar, vector<bool> > &table, vector<uchar> &buffer);
