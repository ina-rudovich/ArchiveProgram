// ArchiveProgram.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <Windows.h>
#include <clocale>
#include "option.h"

int main(int argc, char* argv[])
{
	setlocale(LC_CTYPE, "rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
    return option(argc, argv);
}