#include "stdafx.h"
#include "decompressing.h"

/*action for decompressing*/
void decompressing(fstream &huff_file, fstream &myfile)
{
	map< uchar, int> _rates;
	get_table(huff_file, _rates);
	Node *_root = new Node;
	_root = build_tree(_rates, _root);
	vector<uchar> byte_mass;
	streamoff size = file_size(huff_file);
	huff_file.seekg(_rates.size() * 5 + 1, ios::beg);

	for (int i = _rates.size() * 5 + 1; i < size; i++)
	{
		byte_mass.push_back(huff_file.get());
	}

	vector<bool> bin_mass_from_decryption = decryption(byte_mass);
	vector<uchar> decipher_mass = byte_mass_recovery(_root, bin_mass_from_decryption);

	for (size_t i = 0; i < decipher_mass.size(); i++)
	{
		myfile << decipher_mass[i];
	}
}