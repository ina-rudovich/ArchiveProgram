#pragma once
#include "stdafx.h"
#include<stdio.h>
#include<fstream>
#include<iostream>
#include <vector>

using namespace std;
typedef unsigned char uchar;

streamoff file_size(fstream &myfiles);
vector<uchar> read_file(fstream &myfile);