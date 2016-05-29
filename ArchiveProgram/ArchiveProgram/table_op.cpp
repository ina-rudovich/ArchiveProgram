#include "stdafx.h"
#include "table_op.h"

void save_table(map <uchar, int> table, fstream &myfile)
{
	map < uchar, int>::iterator i;
	myfile << (uchar)(table.size() - 1);

	for (i = table.begin(); i != table.end(); i++)
	{
		myfile.write((char*)&(i->first), 1);
		myfile.write((char*)&(i->second), 4);
	}
}

void get_table(fstream & myfile, map<uchar, int > &table)
{
	uchar table_size;
	myfile.read((char*)&table_size, 1);

	for (int i = 0; i <= table_size; i++)
	{
		uchar temp_byte;
		int temp_value;
		myfile.read((char*)&temp_byte, 1);
		myfile.read((char*)&temp_value, 4);
		table[temp_byte] = temp_value;
	}
}