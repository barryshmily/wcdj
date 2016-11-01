#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include <string.h>
//#include <algorithm>


using namespace std;

inline int taop_min(int a, int b)
{
	return ( a>b ? b : a);
}

int get_vector(const string& src, const char chSplit, vector<string>& vecResult)
{
	if (src.empty())
		return 0;

	const char* pBeg = src.c_str();

	const char* pPos = pBeg;

	char szValue[src.length()];

	while ( *pPos!='\0' )
	{
		if ( *pPos==chSplit )
		{
			memset(szValue,0x00,sizeof(szValue));
			memcpy(szValue,pBeg,taop_min(sizeof(szValue)-1,pPos-pBeg) );


			if ( strlen(szValue)>0 )
				vecResult.push_back(szValue);

			pBeg = pPos + 1;
		}
		++pPos;
	} //while


	if ( pPos-pBeg>0 )
	{
		memset(szValue,0x00,sizeof(szValue));
		memcpy(szValue,pBeg,taop_min(sizeof(szValue)-1,pPos-pBeg) );


		if ( strlen(szValue)>0 )
			vecResult.push_back(szValue);
	}

	return 0;
}

int preproc(char* dir, int index, string& c)
{
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;

	char filename[100];
	snprintf(filename, sizeof(filename), "./%s/%d.cpp", dir, index);

	fp = fopen(filename, "r");
	if ( fp == NULL )
	{
		std::cout << "fopen " << filename << " fail." << std::endl;
		return -1;
	}

	while ((read = getline(&line, &len, fp)) != -1)
	{
		char *p = line;
		char *res = new char[read];

		int i = 0, space = 0, cl = 0, t = 0, note = 0, nd = 1;
		for ( ; p != line + read; p++ )
		{
			if ( i == 0 && ( *p == '\t' || *p == ' ') )
				continue;

			if ( *p == '\t' )
				*p = ' ';

			if ( *p == ' ' && space == 1 )
				continue;

			
			if ( *p == '/' && note == 1 )
			{
				i--;
				break;
			}
			if ( *p == '/' )
				note = 1;
			else
				note = 0;

			if ( *p == ' ' )
				space = 1;
			else
				space = 0;

			if ( *p == '\\' )
			{   
				cl = 1;
				t = 1;
			}   
			else if ( *p == ' ' && cl == 1 ) 
			{   
				cl = 1;
				t++;
			}   
			else if ( *p == '\n' && cl == 1 ) 
			{   
				i = i -t;
				nd = 0;
				break;
			}   
			else
			{   
				cl = 0;
				t = 0;
			}


			res[i++] = *p;
		}
		if ( i > 1)
		{
			res[i - 1] = '\0';

			c.append(res);
			if ( nd == 1 && c[c.length() - 1] != ';')
				c.append(";");
		}

		delete [] res;
	}

	fclose(fp);
	if (line)
		free(line);

	while(1)
	{
		std::size_t pre = c.find("/*");
		if ( pre == std::string::npos )
			break;

		std::size_t pos = c.find("*/");
		if ( pos != std::string::npos )
		{
			c.replace(pre, pos - pre + 2, "");
		}
	}

	while(1)
	{   
		std::size_t pre = c.find("#ifdef 0");
		if ( pre == std::string::npos )
			break;

		std::size_t pos = c.find("#endif");
		if ( pos != std::string::npos )
		{   
			c.replace(pre, pos - pre + 6, "");
		}   
	}

	map<string, string> define;
	while(1)
	{
		std::size_t pre = c.find("#define");
		if ( pre == std::string::npos )
			break;

		std::size_t pos = c.find(";", pre);
		if ( pos != std::string::npos )
		{
			string def = c.substr(pre, pos - pre);
			vector<string> v;
			get_vector(def, ' ', v);
			if ( v.size() == 3 && v[1] != v[2] )
				define[v[1]] = v[2];

			c.replace(pre, pos - pre + 1, "");
		}
	}

	for ( map<string, string>::iterator it = define.begin(); it != define.end(); it++ )
	{
		while(1)
		{
			std::size_t pre = c.find(it->first);
			if ( pre == std::string::npos )
				break;

			c.replace(pre, it->first.length(), it->second);
		}
	}


	return 0;
}

int get_num(string& s, string c)
{
	int count = 0;
	std::size_t pre = 0;
	while(1)
	{
		pre = s.find(c, pre);
		if ( pre == std::string::npos )
			break;

		count++;
	}

	return count;
}

int compare(string& c1, string& c2)
{
	int if_n_1 = get_num(c1, "if");
	int if_n_2 = get_num(c2, "if");

	int else_n_1 = get_num(c1, "else");
	int else_n_2 = get_num(c2, "else");

	int for_n_1 = get_num(c1, "for");
	int for_n_2 = get_num(c2, "for");

	int while_n_1 = get_num(c1, "while");
	int while_n_2 = get_num(c2, "while");

	int do_n_1 = get_num(c1, "do");
	int do_n_2 = get_num(c2, "do");

	int switch_n_1 = get_num(c1, "switch");
	int switch_n_2 = get_num(c2, "switch");


	int loop_n_1 = for_n_1 + while_n_1 + do_n_1;
	int loop_n_2 = for_n_2 + while_n_2 + do_n_2;

	int judge_n_1 = if_n_1 + else_n_1 + switch_n_1;
	int judge_n_2 = if_n_2 + else_n_2 + switch_n_2;

	int diff = abs( loop_n_1 - loop_n_2);
	diff += abs( judge_n_1 - judge_n_2);

	if ( diff > 20 )
		return 1;

	return 0;
}

int proc(int index)
{
	string c1 = "";
	int ret = preproc((char *)"sample1", index, c1);
	if ( ret != 0 )
		return ret;

	string c2 = "";
	ret = preproc((char *)"sample2", index, c2);
	if ( ret != 0 )
		return ret;

	ret = compare(c1, c2);
	string r = "N";
	if ( ret == 0 )
		r = "Y";
	r.append("\n");

	FILE * fp; 
	fp = fopen("./result.txt", "a+");
	if ( fp == NULL )
	{
		std::cout << "open file ./result.txt err: " << std::endl;
		return -1;
	}

	fwrite(r.c_str(), r.length(), 1, fp);
	
	fclose(fp);

	return 0;
}

int main(int argc, char* argv[])
{
	if ( argc != 2 )
	{
		std::cout << argv[0] << " num" << std::endl;
		return -1;
	}

	int num = atoi(argv[1]);
	for ( int i = 1; i <= num; i++ )
	{
		proc(i);
	}


	return 0;
}

