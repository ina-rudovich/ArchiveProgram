#include "stdafx.h"
#include "compressing.h"

void compressing(fstream &myfile, fstream &huff_file)
{
	vector <uchar> buffer;
	buffer = read_file(myfile);
	map<uchar, int> rates = frequency_of_occurrences(buffer);
	Node* root = new Node;
	root = build_tree(rates, root);
	vector<bool> code;
	map < uchar, vector<bool> >table;
	association_code_byte(root, table, code);
	save_table(rates, huff_file);
	vector<bool> bin_mass = encryption(table, buffer);
	vector<uchar> new_byte_mass = compiling_an_array_of_bytes(bin_mass);

	for (size_t i = 0; i < new_byte_mass.size(); i++)
	{
		huff_file << new_byte_mass[i];
	}

	cout << "Файл успешно сжат." << endl;
}