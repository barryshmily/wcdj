#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>

using std::string;
using std::ifstream;
using std::ios;

void err_exit()
{
	fprintf(stderr, "unable to open input file");
	exit(1);
}

long long file_size(ifstream &input)
{
	long long len = 0;
	input.seekg(0, ios::end);
	len = input.tellg()/1024;// KB
	input.seekg(0, ios::beg);
	return len;
}


int main(int argc, char **argv)
{
	if (argc != 2)
		fprintf(stderr, "usage: %s filename\n", argv[0]), exit(1);

	//ifstream input(argv[1], ios::binary|ios::in);
	ifstream input;
	string strContent;
	long long len = 0;


	input.open(argv[1]);
	if (!input)
		err_exit();
	else
	{
		len = file_size(input);

		time_t begin, end;
		begin = clock();
		while (getline(input, strContent, '\0'));
		end = clock();
		input.clear();

		printf("Used: %lf ms, Size: %lld KB\n", static_cast<double>(end-begin)/CLOCKS_PER_SEC*1000, len); 
	}
	input.close();

	return 0;
}

/*
g++ -Wall -g -o read read.cpp
*/


