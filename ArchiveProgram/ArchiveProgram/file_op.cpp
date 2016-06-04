#include "stdafx.h"
#include "file_op.h"

/*size of file*/
streamoff file_size(fstream &myfile)
{
	streamoff size = 0;
	myfile.seekg(0, ios::end);
	size = myfile.tellg();
	myfile.seekg(0, ios::beg);
	return size;
}

/*writing bytes of the file in array*/
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
