#pragma once

#include "stdafx.h"
#include <stdio.h>
#include <vector>
#include <map>
#include<fstream>

using namespace std;

typedef unsigned char uchar;

void get_table(fstream & myfile, map<uchar, int > &table);
void save_table(map <uchar, int> table, fstream &myfile);