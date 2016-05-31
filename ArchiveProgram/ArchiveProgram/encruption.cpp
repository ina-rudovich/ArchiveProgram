#include "stdafx.h"
#include "encruption.h"

/*Получение нового двоичного кода для байта*/
void association_code_byte(Node* root, map<uchar, vector<bool> > &table, vector <bool> &code)
{
	if (root->left != NULL)
	{
		code.push_back(0);
		association_code_byte(root->left, table, code);
		code.pop_back();
	}

	if (root->right != NULL)
	{
		code.push_back(1);
		association_code_byte(root->right, table, code);
		code.pop_back();
	}

	if (root->sheet)
	{
		table[root->byte] = code;
	}
}

/*получение булевого массива для сжатия*/
vector<bool> encryption(map<uchar, vector<bool> > &table, vector<uchar> &buffer)
{
	vector<bool> bin_mass;

	for (size_t i = 0; i < buffer.size(); i++)
	{
		for (size_t j = 0; j < (table[buffer[i]]).size(); j++)
			bin_mass.push_back((table[buffer[i]])[j]);
	}

	return bin_mass;
}

/*преобразование булевого массива в массив байт*/
vector<uchar> compiling_an_array_of_bytes(vector<bool> bin_mass)
{
	vector<uchar> new_byte_mass;
	new_byte_mass.push_back(0);

	size_t k = 0;
	size_t mass_bytes = bin_mass.size() - bin_mass.size() % 8;

	for (k; k < mass_bytes;)
	{
		uchar temp_byte = 0;

		for (int i = 7; i >= 0; i--)
		{
			temp_byte += bin_mass[k] * ((uchar)pow(2, i));
			k++;
		}

		new_byte_mass.push_back(temp_byte);
	}

	bool t = (k == bin_mass.size());

	if (!t)
	{
		new_byte_mass[0] = (uchar)bin_mass.size() % 8;
		uchar temp_byte = 0;

		for (uchar i = new_byte_mass[0]; i >0; i--)
		{
			temp_byte += bin_mass[k] * ((uchar)pow(2, i - 1));
			k++;
		}

		new_byte_mass.push_back(temp_byte);
	}

	return new_byte_mass;
}
