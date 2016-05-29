#pragma once
#include "stdafx.h"
#include "node.h"
#include <stdio.h>
#include <vector>

using namespace std;

vector<bool> decryption(vector<uchar> byte_mass);
vector<uchar> byte_mass_recovery(Node* root, vector<bool> bin_mass);