#include "stdafx.h"
#include "decruption.h"

/*ѕолучение булевого массива дл€ "восстановлени€ файла"*/
vector<bool> decryption(vector<uchar> byte_mass)
{
	vector<bool> bit_mass;
	uchar counter;
	if ((counter = byte_mass.front()) == 0) /*если все байты "полные"*/
	{
		for (size_t i = 1; i < byte_mass.size(); i++)
		{
			vector<bool> temp_bool_mass;
			uchar temp_byte = byte_mass[i];

			for (int j = 1; j < 9; j++)
			{
				bool temp = ((temp_byte % 2) != 0);
				temp_bool_mass.push_back(temp);
				temp_byte /= 2;
			}

			for (int j = 7; j >= 0; j--)
			{
				bit_mass.push_back(temp_bool_mass[j]);
			}
		}
	}
	else     /*если последний "неполный"*/
	{
		for (size_t i = 1; i < byte_mass.size() - 1; i++)
		{
			vector<bool> temp_bool_mass;
			uchar temp_byte = byte_mass[i];

			for (int j = 1; j < 9; j++)
			{
				bool temp = ((temp_byte % 2) != 0);
				temp_bool_mass.push_back(temp);
				temp_byte /= 2;
			}


			for (int j = 7; j >= 0; j--)
			{
				bit_mass.push_back(temp_bool_mass[j]);

			}
		}

		vector<bool> temp_bool_mass;
		uchar temp_byte = byte_mass.back();
		byte_mass.pop_back();

		for (int j = 0; j < counter; j++)
		{
			bool temp = ((temp_byte % 2) == 1);
			temp_bool_mass.push_back(temp);
			temp_byte /= 2;
		}

		for (int j = counter - 1; j >= 0; j--)
		{
			bit_mass.push_back(temp_bool_mass[j]);
		}
	}
	return bit_mass;
}

/*ѕолучение первоначальной последовательности байт (обход дерева)*/
vector<uchar> byte_mass_recovery(Node* tree_root, vector<bool> bin_mass)
{
	vector<uchar> byte_mass;
	size_t i = 0;

	while (i < bin_mass.size())
	{
		Node* root = tree_root;
		while (!(root->sheet))
		{
			if (bin_mass[i] == 0)
				root = root->left;
			else
				root = root->right;
			i++;
		}
		byte_mass.push_back(root->byte);
	}

	return byte_mass;
}