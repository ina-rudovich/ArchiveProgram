// ArchiveProgram.cpp: ���������� ����� ����� ��� ����������� ����������.
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
#include <ctime>

using namespace std;

typedef unsigned char uchar_byte;
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
		return left->weight <= right->weight;
	}
};

streamoff file_size(fstream &myfile);
fstream open_file(void);
vector<uchar> read_file(fstream &myfile);
void write_file(vector<uchar> & buffer);
map<uchar, int> frequency_of_occurrences(vector<uchar> &buffer);
Node* build_tree(map<uchar, int> &rates, Node* root);
void print_tree(Node *root);
void Association_code_byte(Node* root, map<uchar, vector<bool> > &table, vector <bool> &code);
vector<bool> encryption(map<uchar, vector<bool> > &table, vector<uchar> &buffer);
vector<uchar_byte> compiling_an_array_of_bytes(vector<bool> bin_mass);
vector<bool> decryption(vector<uchar_byte> byte_mass);
vector<uchar> byte_mass_recovery(Node* root, vector<bool> bin_mass);
void add_base_info_for_decode(map<uchar, vector<bool> > table, fstream &myfile, string str);
void get_base_info_for_decode(fstream & myfile, map<uchar, vector<bool> > &table, string &file_name);
Node* make_tree_for_decode(map<uchar, vector<bool> > &table);


int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	fstream myfile;
	//myfile = open_file();
	myfile.open("..//..//..//tests//cube.obj", ios::binary | ios::in);
	vector <uchar> buffer;
	buffer = read_file(myfile);
	myfile.close();
	cout << buffer.size() << endl;
	cout << "clock1 --- " << clock()/ CLK_TCK  << endl;

	/*for (int i = 0; i < buffer.size(); i++)
		cout << (int)buffer[i] << " ";*/
	
	/*vector<uchar> buffer;
	string str = "it is my stringggggg!!!!!!!";

	for (unsigned int i = 0; i < str.size(); i++)
	{
		buffer.push_back(str[i]);
	}*/

	map<uchar, int> rates = frequency_of_occurrences(buffer);

	Node* root = new Node;
	root = build_tree(rates, root);
	//print_tree(root);

	vector<bool> code;
	map < uchar, vector<bool> >table;
	Association_code_byte(root, table, code);
	
	fstream huff_file("..//..//..//tests//tree.hfmn", ios::binary | ios::in | ios :: app);
	add_base_info_for_decode(table, huff_file, "test.txt");
	
	map < uchar, vector<bool> > t;
	string name;
	get_base_info_for_decode(huff_file, t, name);
	huff_file.close();
	Node *tree = make_tree_for_decode(t);
	//print_tree(tree);

	vector<bool> bin_mass = encryption(table, buffer);

	vector<uchar_byte> new_byte_mass = compiling_an_array_of_bytes(bin_mass);

	write_file(new_byte_mass);
	cout << "clock7 --- " << clock()  << endl;

	vector<bool> bin_mass_from_decryption = decryption(new_byte_mass);

	vector<uchar> decipher_mass = byte_mass_recovery(tree, bin_mass_from_decryption);

	write_file(decipher_mass);
	//system("cls");
	cout << "clock --- " << clock()/ CLK_TCK << endl;;
	system("pause");
    return 0;
}

/*�������� ����� ��� ������*/
fstream open_file(void)
{
	string str;
	fstream myfile;
	for (;;)
	{
		cout << "������� ������ ���� � �����" << endl;
		getline(cin, str);
		myfile.open(str, ios::binary | ios::in);
		if (!myfile.is_open())
		{
			cout << "�������� ������" << endl;
		}
		else
		{
			cout << "���� ������" << endl;
			system("pause");
			return myfile;
		}
	}
}

/*������ �����*/
streamoff file_size(fstream &myfile)
{
	streamoff size = 0;
	myfile.seekg(0, ios::end);
	size = myfile.tellg();
	myfile.seekg(0, ios::beg);
	return size;
}

/*������ ������ ����� � ������*/
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

/*������ ������ �� ������� � ����*/
void write_file(vector<uchar> & buffer)
{
	string str;
	cout << "������� ��� ��� ������� �����" << endl;
	cin >> str;
	//str = "Compressed_Huffman.hfmn";
	fstream myfile(str, ios::binary | ios::out);

	for (size_t i = 0; i < buffer.size(); i++) {
		myfile << buffer[i];
	}
	cout << "���� �������" << endl;
	myfile.close();
	system("pause");
}

/*������� ������*/
map<uchar, int> frequency_of_occurrences(vector<uchar> &buffer)
{
	map<uchar, int> rates;

	for (size_t i = 0; i < buffer.size(); i++) {
		rates[buffer[i]]++;
	}

	return rates;
}

/*���������� ������*/
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

/*��������� ������ ��������� ���� ��� �����*/
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

/*����� ������*/
void print_tree(Node *root)
{
	if (root != NULL)
	{
		if(root->sheet)
		cout <<  " : byte - "<< root->byte << endl;
		else
			cout << "***" << endl;

		print_tree(root->left);
		print_tree(root->right);
	}
	return;
}

/*��������� �������� ������� ��� ������*/
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

/*�������������� �������� ������� � ������ ����*/
vector<uchar_byte> compiling_an_array_of_bytes(vector<bool> bin_mass)
{
	vector<uchar_byte> new_byte_mass;
	new_byte_mass.push_back(0);

	size_t k = 0;
	size_t mass_bytes = bin_mass.size() - bin_mass.size() % 8;
	for (k; k < mass_bytes;)
	{
		uchar_byte temp_byte = 0;
		for (int i = 7; i >= 0; i--)
		{
			temp_byte += bin_mass[k]*((uchar_byte) pow(2, i));
			k++;
		}
		new_byte_mass.push_back(temp_byte);
		
	}
	cout << "new_byte_mass --- "<<new_byte_mass.size() << endl;
	bool t = (k == bin_mass.size());
		if (!t)
		{
			new_byte_mass[0] = (uchar_byte)bin_mass.size() % 8;
			uchar_byte temp_byte = 0;
			for (uchar_byte i = new_byte_mass[0]; i >0 ;i--)
			{
				temp_byte += bin_mass[k] * ((uchar_byte) pow(2, i-1));
				k++;			
			}
			new_byte_mass.push_back(temp_byte);
		}
	return new_byte_mass;
}

/*��������� �������� ������� ��� "�������������� �����"*/
vector<bool> decryption(vector<uchar_byte> byte_mass)
{
	vector<bool> bit_mass;
	uchar_byte counter;
	if ((counter = byte_mass.front() ) == 0) /*���� ��� ����� "������"*/
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
	else     /*���� ��������� "��������"*/
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

/*��������� �������������� ������������������ ���� (����� ������)*/
vector<uchar> byte_mass_recovery(Node* tree_root, vector<bool> bin_mass)
{
	vector<uchar> byte_mass;
	while (bin_mass.size() != 0)
	{
		Node* root = tree_root;
		while (!(root->sheet))
		{
			if (bin_mass.front() == 0)
				root = root->left;
			else
				root = root->right;
			bin_mass.erase(bin_mass.begin(), bin_mass.begin() + 1);
		}
		byte_mass.push_back(root->byte);
	}
	cout << "byte_mass --- " << byte_mass.size() << endl;
	return byte_mass;
}

/*���������� "z" + ����� � ���� */
void add_base_info_for_decode(map<uchar, vector<bool> > table, fstream &myfile,string str)
{
	myfile << (uchar) str.size();

	for (int i = 0; i < str.size(); i++)
		myfile << (uchar)str[i];
	cout << "table size in add... " << table.size() << endl;
	myfile << (uchar)(table.size()-1);
	cout << "file-size" <<file_size(myfile) << endl;

	map<uchar, short> compressed_tree;

	for (map<uchar, vector<bool> >::iterator i = table.begin(); i != table.end(); i++)
	{
		uchar k = (i->second).size();
		short temp = (short)pow(2, k);

		for (size_t j = 0; j <k; j++)
		{
			temp += (i->second)[j]*((short)pow(2, (k-j-1)));			
		}
		compressed_tree[i->first] = temp;
	}

	for (map<uchar, short> ::iterator i = compressed_tree.begin(); i != compressed_tree.end(); i++)
	{
		myfile.write((char*)&(i->first),1);
		myfile.write((char*)&(i->second), 2);
	}
	cout << "file_size === " <<file_size(myfile) << endl;
	
}

/*��������� ����� � ���������� ��� ��������������*/
void get_base_info_for_decode(fstream & myfile, map<uchar, vector<bool> > &table, string &file_name)
{
	cout << "get_base_info_for_decode" << endl;
	cout << "file_size === " << file_size(myfile) << endl;
	uchar str_length ;
	myfile.read((char*)&str_length, 1);
	cout << "str_len --- " << (int)str_length << endl;
	file_name.resize(str_length);
	for (int i = 0; i < str_length; i++)
	{
		file_name[i] = myfile.get();
	}

	cout << file_name << endl;

	uchar table_size;
	myfile.read((char*)&table_size, 1);
	cout << "table_size" <<(int) table_size << endl;

	map<uchar, short> compressed_tree;
	for (short i = 0; i <= table_size; i++)
	{
		uchar temp_byte;
		short temp_value;
		myfile.read((char*)&temp_byte, 1);
		myfile.read((char*)&temp_value, 2);
		compressed_tree[temp_byte] = temp_value;
	}

	for (map<uchar, short> ::iterator i = compressed_tree.begin(); i != compressed_tree.end(); i++)
	{
		vector<bool> code;
		while (i->second != 1)
		{
			bool temp = ((i->second % 2) != 0);
			code.push_back(temp);
			i->second /= 2;
		}

		for (int j = code.size() - 1; j>= 0; j--)
		{
			table[i->first].push_back(code[j]);
		}
	}
}

/*�������������� ������*/
Node* make_tree_for_decode(map<uchar, vector<bool> > &table)
{
	Node* tree =new Node ;
	for (map<uchar, vector<bool> >::iterator i = table.begin(); i != table.end(); i++)
	{
		Node *root = tree;
		for (size_t j = 0; j < i->second.size(); j++)
		{
			if (i->second[j] == 0)
			{
				if (root->left == NULL)
				{
					Node *r = new Node;
					root->left = r;
				}
				root = root->left;
			}
			else
			{
				if (root->right == NULL)
				{
					Node *r = new Node;
					root->right = r;
				}
				root = root->right;
			}
		}
		root->byte = i->first;
		root->sheet = true;
	}
	return tree;
}
