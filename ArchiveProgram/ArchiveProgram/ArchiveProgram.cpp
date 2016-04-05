// ArchiveProgram.cpp: определяет точку входа для консольного приложения.
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
		/*for (int i = 0; i < v.size(); i++)
		cout << v[i] << "  ";*/
	return v;
}
/*Запись байтов из массива в файл*/
void write_file(vector<uchar> & buffer)
{
	string str;
	cout << "Введите имя для сжатого файла" << endl;
	cin >> str;
	//str += ".txt";
	fstream myfile(str, ios::binary | ios::out);

	for (int i = 0; i < buffer.size(); i++) {
		myfile << buffer[i];
	}
	cout << "Файл Записан" << endl;
	myfile.close();
	system("pause");
}