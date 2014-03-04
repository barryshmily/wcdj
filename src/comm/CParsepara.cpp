#include "CParsepara.h"

using namespace std;

/* 
 * CSpacePara 
 * */
void wcdj::util::CSpacePara::decode(const string & s)
{
	string value;
	char *p = const_cast<char *>(s.c_str());
	char *p1;
	_paralist.clear();

	while (*p != 0) 
	{
		if (is_split((unsigned char)*p)) 
		{
			p++; 
			continue;
		}
		
		p1 = p;
		while (*p1 != 0) 
		{
			if (is_split((unsigned char)*p1)) 
			{
				break;
			}
			p1++;
		}

		value = string(p, 0, p1-p);
		_paralist.push_back(value);
		p = p1;
	}

	return;
}

bool wcdj::util::CSpacePara::is_split(unsigned char c)
{
	char *p = const_cast<char *>(_split.c_str());

	while (*p != 0) 
	{
		if ((unsigned char)*p == (unsigned char)c)
			return true;

		++p;
	}

	return false;
}


