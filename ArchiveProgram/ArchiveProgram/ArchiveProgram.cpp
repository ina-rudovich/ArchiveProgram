// ArchiveProgram.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include<fstream>
#include<iostream>
#include<string>
#include <Windows.h>
#include<stdio.h>
#include <vector>
#include <map>
#include <list>
#include <clocale>

using namespace std;

typedef unsigned char uchar;

class Node
{
public:
	int weight;
	uchar byte;
	Node *left, *right;
	bool sheet;

	Node(Node *Left, Node *Right)
	{
		left = Left;
		right = Right;
		weight = Left->weight + Right->weight;
		sheet = false;
	}
	Node() {}
};

struct Compare
{
	bool operator()(Node *left, Node *right) const
	{
		return left->weight < right->weight;
	}
};

streamoff file_size(fstream &myfiles);
vector<uchar> read_file(fstream &myfile);
map<uchar, int> frequency_of_occurrences(vector<uchar> &buffer);
Node* build_tree(map<uchar, int> &rates, Node* root);
void Association_code_byte(Node* root, map<uchar, vector<bool> > &table, vector <bool> &code);
vector<uchar> compiling_an_array_of_bytes(vector<bool> bin_mass);
vector<bool> encryption(map<uchar, vector<bool> > &table, vector<uchar> &buffer);

vector<bool> decryption(vector<uchar> byte_mass);
vector<uchar> byte_mass_recovery(Node* root, vector<bool> bin_mass);

void decompressing(fstream &myfile, fstream &huff_file);
void compressing(fstream &huff_file, fstream &myfile);

int option(int argc, char* argv[]);

void get_table(fstream & myfile, map<uchar, int > &table);
void save_table(map <uchar, int> table, fstream &myfile);

int main(int argc, char* argv[])
{
	setlocale(LC_CTYPE, "rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
    return option(argc, argv);
}

/*Размер файла*/
streamoff file_size(fstream &myfile)
{
	streamoff size = 0;
	myfile.seekg(0, ios::end);
	size = myfile.tellg();
	myfile.seekg(0, ios::beg);
	return size;
}

/*Запись байтов файла в массив*/
vector<uchar> read_file(fstream &myfile)
{
	streamoff size = file_size(myfile);
	vector <uchar> v;

	for (int i = 0; i < size; i++)
	{
		v.push_back(myfile.get());
	}

	return v;
}

/*Подсчет частот*/
map<uchar, int> frequency_of_occurrences(vector<uchar> &buffer)
{
	map<uchar, int> rates;

	for (size_t i = 0; i < buffer.size(); i++)
	{
		rates[buffer[i]]++;
	}

	return rates;
}

/*Построение дерева*/
Node* build_tree(map<uchar, int> &rates, Node* root)
{
	list <Node*> nodes;

	map<uchar, int>::iterator i;
	for (i = rates.begin(); i != rates.end(); i++)
	{
		Node *p = new Node;
		p->byte = i->first;
		p->weight = i->second;
		p->left = NULL;
		p->right = NULL;
		p->sheet = true;
		nodes.push_back(p);
	}

	while (nodes.size() != 1)
	{
		nodes.sort(Compare());

		Node *left_son = nodes.front();
		nodes.pop_front();
		Node *right_son = nodes.front();
		nodes.pop_front();

		Node *parent = new Node(left_son, right_son);
		nodes.push_front(parent);
	}

	root = nodes.front();
	nodes.pop_front();
	return root;
}

/*Получение нового двоичного кода для байта*/
void Association_code_byte(Node* root, map<uchar, vector<bool> > &table, vector <bool> &code)
{
	if (root->left != NULL)
	{
		code.push_back(0);
		Association_code_byte(root->left, table, code);
		code.pop_back();
	}

	if (root->right != NULL)
	{
		code.push_back(1);
		Association_code_byte(root->right, table, code);
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
		for (size_t j = 0; j < (table[buffer[i]]).size();j++)
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
			temp_byte += bin_mass[k]*((uchar) pow(2, i));
			k++;
		}

		new_byte_mass.push_back(temp_byte);		
	}

	bool t = (k == bin_mass.size());

		if (!t)
		{
			new_byte_mass[0] = (uchar)bin_mass.size() % 8;
			uchar temp_byte = 0;

			for (uchar i = new_byte_mass[0]; i >0 ;i--)
			{
				temp_byte += bin_mass[k] * ((uchar) pow(2, i-1));
				k++;			
			}

			new_byte_mass.push_back(temp_byte);
		}

	return new_byte_mass;
}

/*Получение булевого массива для "восстановления файла"*/
vector<bool> decryption(vector<uchar> byte_mass)
{
	vector<bool> bit_mass;
	uchar counter;
	if ((counter = byte_mass.front() ) == 0) /*если все байты "полные"*/
	{
		for (size_t i = 1; i < byte_mass.size();i++)
		{
			vector<bool> temp_bool_mass;
			uchar temp_byte = byte_mass[i];

			for (int j = 1; j < 9;j++)
			{
				bool temp = ((temp_byte % 2) != 0);
				temp_bool_mass.push_back(temp);
				temp_byte /= 2;
			}
			
			for (int j = 7; j >=0; j--)
			{
				bit_mass.push_back(temp_bool_mass[j]);
			}
		}
	}
	else     /*если последний "неполный"*/
	{	
		for (size_t i = 1; i < byte_mass.size()-1;i++)
		{
			vector<bool> temp_bool_mass;
			uchar temp_byte = byte_mass[i];

			for (int j = 1; j < 9; j++)
			{
				bool temp = ((temp_byte % 2) != 0);
				temp_bool_mass.push_back(temp);
				temp_byte /= 2;
			}


			for (int j = 7; j >=0; j--)
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

		for (int j = counter - 1;j>=0; j--)
		{
			bit_mass.push_back(temp_bool_mass[j]);
		}
	}
	return bit_mass;
}

/*Получение первоначальной последовательности байт (обход дерева)*/
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

void compressing(fstream &myfile, fstream &huff_file)
{
	vector <uchar> buffer;
	buffer = read_file(myfile);
	map<uchar, int> rates = frequency_of_occurrences(buffer);
	Node* root = new Node;
	root = build_tree(rates, root);
	vector<bool> code;
	map < uchar, vector<bool> >table;
	Association_code_byte(root, table, code);
	save_table(rates, huff_file);
	vector<bool> bin_mass = encryption(table, buffer);
	vector<uchar> new_byte_mass = compiling_an_array_of_bytes(bin_mass);

	for (size_t i = 0; i < new_byte_mass.size(); i++) 
	{
		huff_file << new_byte_mass[i];
	}

	cout << "Файл успешно сжат." << endl;
}

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

	cout << "Файл успешно распакован." << endl;
}

int option(int argc, char* argv[])
{
	if (argc != 5)
	{
		cout << "Wrong count of argument" << endl;
		return -1;
	}

	string i_name;
	string o_name;

	if (((string)argv[1] == "-i") && ((string)argv[3] == "-o"))
	{
		i_name = argv[2];
		o_name = argv[4];
	}
	else
	{
		if (((string)argv[3] == "-i") && ((string)argv[1] == "-o"))
		{
			i_name = argv[2];
			o_name = argv[4];
		}
		else
			cout << "Wrong arguments" << endl;
			return -1;
	}

	fstream i_file(i_name, ios::binary | ios::in);
	fstream o_file(o_name, ios::binary | ios::app);

	if (!i_file.is_open() || !o_file.is_open())
	{
		cout << "Error opening file" << endl;
		return - 1;
	}
		
	string file_extention = "";
	if (i_name.find_last_of(".") != string::npos)
		file_extention = i_name.substr(i_name.find_last_of("."), i_name.size() - 1);

	if (file_extention.compare(".hfmn") == 0)
	{
		cout << "Этот файл будет распакован." << endl;
		decompressing(i_file,o_file);
	}
	else
	{
		cout << "Этот файл будет сжат." << endl;
		compressing(i_file,o_file);
	}

	i_file.close();
	o_file.close();
	return 0;
}