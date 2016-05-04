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


using namespace std;

typedef unsigned char uchar_byte;
typedef char uchar;

class Node
{
public:
	int weight;
	uchar byte;
	Node *left, *right;

	Node(Node *Left, Node *Right)
	{
		left = Left;
		right = Right;
		weight = Left->weight + Right->weight;
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

streamoff file_size(fstream &myfile);
fstream open_file(void);
vector<uchar> read_file(fstream &myfile);
void write_file(vector<uchar> & buffer);
map<uchar, int> frequency_of_occurrences(vector<uchar> buffer);
Node* build_tree(map<uchar, int> rates, Node* root);
void print_tree(Node *root);
void Association_code_byte(Node* root, map<uchar, vector<bool> > &table, vector <bool> &code);
vector<bool> encryption(map<uchar, vector<bool> > table, vector<uchar> buffer);
vector<uchar_byte> compiling_an_array_of_bytes(vector<bool> bin_mass);
vector<bool> decryption(vector<uchar_byte> byte_mass);
vector<uchar> byte_mass_recovery(Node* root, vector<bool> bin_mass);

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	fstream myfile;

	//myfile = open_file();
	/*myfile.open("D:\\ЛАБЫ\\ArchiveProgram\\ArchiveProgram\\ArchiveProgram\\test.txt", ios::binary | ios::in);
	vector <uchar> buffer;
	buffer = read_file(myfile);
	myfile.close();*/
	
	vector<uchar> buffer;
	string str = "it is string for tests!!!!!!!";

	for (unsigned int i = 0; i < str.size(); i++)
	{
		buffer.push_back(str[i]);
	}

	map<uchar, int> rates = frequency_of_occurrences(buffer);

	Node* root = new Node;
	root = build_tree(rates, root);
	//print_tree(root);

	vector<bool> code;
	map < uchar, vector<bool> >table;
	Association_code_byte(root, table, code);

	vector<bool> bin_mass = encryption(table, buffer);

	vector<uchar_byte> new_byte_mass = compiling_an_array_of_bytes(bin_mass);

	vector<bool> bin_mass_from_decryption = decryption(new_byte_mass);

	vector<uchar> decipher_mass = byte_mass_recovery(root, bin_mass_from_decryption);

	for (size_t i = 0; i < decipher_mass.size(); i++)
	{
		cout << decipher_mass[i];
	}
	cout << endl;

	//write_file(buffer);
	//system("cls");
	system("pause");
    return 0;
}

/*Открытия файла для сжатия*/
fstream open_file(void)
{
	string str;
	fstream myfile;
	for (;;)
	{
		cout << "Введите полный путь к файлу" << endl;
		getline(cin, str);
		myfile.open(str, ios::binary | ios::in);
		if (!myfile.is_open())
		{
			cout << "Неверные данные" << endl;
		}
		else
		{
			cout << "Файл открыт" << endl;
			system("pause");
			return myfile;
		}
	}
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

/*Запись байтов из массива в файл*/
void write_file(vector<uchar> & buffer)
{
	string str;
	/*cout << "Введите имя для сжатого файла" << endl;
	cin >> str;*/
	str = "Compressed_Huffman.hfmn";
	fstream myfile(str, ios::binary | ios::out);

	for (size_t i = 0; i < buffer.size(); i++) {
		myfile << buffer[i];
	}
	cout << "Файл Записан" << endl;
	myfile.close();
	system("pause");
}

/*Подсчет частот*/
map<uchar, int> frequency_of_occurrences(vector<uchar> buffer)
{
	map<uchar, int> rates;

	for (size_t i = 0; i < buffer.size(); i++) {
		rates[buffer[i]]++;
	}

	return rates;
}

/*Построение дерева*/
Node* build_tree(map<uchar, int> rates, Node* root)
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
	if (root->byte)
	{
		table[root->byte] = code;		
	}
}

/*вывод дерева*/
void print_tree(Node *root)
{
	if (root != NULL)
	{
		if(root->byte)
		cout << "weight - " << root->weight << " : byte - "<< root->byte << endl;
		else
			cout << "weight - " << root->weight << endl;

		print_tree(root->left);
		print_tree(root->right);
	}
	return;
}

/*получение булевого массива для сжатия*/
vector<bool> encryption(map<uchar, vector<bool> > table, vector<uchar> buffer)
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
vector<uchar_byte> compiling_an_array_of_bytes(vector<bool> bin_mass)
{
	vector<uchar_byte> new_byte_mass;
	new_byte_mass.push_back(0);

	while ((bin_mass.size() / 8))
	{
		uchar_byte temp_byte = 0;
		for (int i = 7; i >= 0; i--)
		{
			temp_byte += bin_mass.front()*((uchar_byte) pow(2, i));
			bin_mass.erase(bin_mass.begin(), bin_mass.begin()+1);
		}
		new_byte_mass.push_back(temp_byte);
	}

	bool t = bin_mass.empty();
		if (!t)
		{
			new_byte_mass[0] = (uchar_byte)bin_mass.size();
			uchar_byte temp_byte = 0;
			uchar_byte i = 0;
			while (bin_mass.size() != 0)
			{
				temp_byte += bin_mass.back() * ((uchar_byte) pow(2, i));
				bin_mass.pop_back();
				i++;
			}
			new_byte_mass.push_back(temp_byte);
		}

	return new_byte_mass;
}

/*Получение булевого массива для "восстановления файла"*/
vector<bool> decryption(vector<uchar_byte> byte_mass)
{
	vector<bool> bit_mass;
	uchar_byte counter;
	if ((counter = byte_mass.front() ) == 0) /*если все байты "полные"*/
	{
		byte_mass.erase(byte_mass.begin(), byte_mass.begin() + 1 );
		for (size_t i = 0; i < byte_mass.size();)
		{
			vector<bool> temp_bool_mass;
			uchar_byte temp_byte = byte_mass.front();

			for (int j = 1; j < 9;j++)
			{
				bool temp = ((temp_byte % 2) != 0);
				temp_bool_mass.push_back(temp);
				temp_byte /= 2;
			}
			byte_mass.erase(byte_mass.begin(), byte_mass.begin() + 1);

			for (int j = 1; j < 9; j++)
			{
				bit_mass.push_back(temp_bool_mass.back());
				temp_bool_mass.pop_back();
			}
		}
	}
	else     /*если последний "неполный"*/
	{
		byte_mass.erase(byte_mass.begin(), byte_mass.begin() + 1);
		vector<bool> temp_bool_mass;
		for (size_t i = 1; i < byte_mass.size();)
		{

			uchar_byte temp_byte = byte_mass.front();

			for (int j = 1; j < 9; j++)
			{
				bool temp = ((temp_byte % 2) != 0);
				temp_bool_mass.push_back(temp);
				temp_byte /= 2;
			}
			byte_mass.erase(byte_mass.begin(), byte_mass.begin() + 1);

			for (int j = 1; j < 9; j++)
			{
				bit_mass.push_back(temp_bool_mass.back());
				temp_bool_mass.pop_back();
			}
		}

		uchar_byte temp_byte = byte_mass.front();
		byte_mass.pop_back();
		for (int j = 0; j < counter; j++)
		{
			bool temp = ((temp_byte % 2) != 0);
			temp_bool_mass.push_back(temp);
			temp_byte /= 2;
		}

		for (int j = 0; j < counter; j++)
		{
			bit_mass.push_back(temp_bool_mass.back());
			temp_bool_mass.pop_back();
		}
	}
	return bit_mass;
}

/*Получение первоначальной последовательности байт (обход дерева)*/
vector<uchar> byte_mass_recovery(Node* tree_root, vector<bool> bin_mass)
{
	vector<uchar> byte_mass;
	while (bin_mass.size() != 0)
	{
		Node* root = tree_root;
		while (!(root->byte))
		{
			if (bin_mass.front() == 0)
				root = root->left;
			else
				root = root->right;
			bin_mass.erase(bin_mass.begin(), bin_mass.begin() + 1);
		}
		byte_mass.push_back(root->byte);
	}
	return byte_mass;
}