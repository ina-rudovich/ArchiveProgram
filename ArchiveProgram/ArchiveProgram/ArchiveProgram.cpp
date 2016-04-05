// ArchiveProgram.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include<fstream>
#include<iostream>
#include<string>
#include <Windows.h>
#include<stdio.h>
#include <vector>
using namespace std;

typedef unsigned char uchar;

streamoff file_size(fstream &myfile);
fstream open_file(void);
vector<uchar> read_file(fstream &myfile);
void write_file(vector<uchar> & buffer);

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	fstream myfile;
	//myfile = open_file();
	myfile.open("D:\\123.txt");
	vector <uchar> buffer;
	buffer = read_file(myfile);
	myfile.close();
	write_file(buffer);
	//system("cls");
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
		/*for (int i = 0; i < v.size(); i++)
		cout << v[i] << "  ";*/
	return v;
}
/*������ ������ �� ������� � ����*/
void write_file(vector<uchar> & buffer)
{
	string str;
	cout << "������� ��� ��� ������� �����" << endl;
	cin >> str;
	//str += ".txt";
	fstream myfile(str, ios::binary | ios::out);

	for (int i = 0; i < buffer.size(); i++) {
		myfile << buffer[i];
	}
	cout << "���� �������" << endl;
	myfile.close();
	system("pause");
}