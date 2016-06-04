#include "stdafx.h"
#include "option.h"
/*choosing mode*/
int option(int argc, char* argv[])
{
	if (argc != 5)
	{
		cout << "Wrong count of argument" << endl;
		return -1;
	}

	string i_name;
	string o_name;
	/*"-i" for input file
	  "-o" for output file*/
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
		return -1;
	}

	string file_extention = "";
	if (i_name.find_last_of(".") != string::npos)
		file_extention = i_name.substr(i_name.find_last_of("."), i_name.size() - 1);
	
	if (file_extention.compare(".hfmn") == 0)
	{
		decompressing(i_file, o_file);
	}
	else
	{
		compressing(i_file, o_file);
	}

	i_file.close();
	o_file.close();
	cout << "Done" << endl;
	return 0;
}