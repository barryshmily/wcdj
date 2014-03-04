#ifndef __WCDJ_OPTION_H__
#define __WCDJ_OPTION_H__

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdexcept>
#include <iostream>
#include <assert.h>
#include <map>

using namespace std;

/*
 * usage: ./prog -para1=value1 -para2=value2 ... dest
 */
class COption {
public:

	void read_arg(int argc, char **argv) throw (runtime_error);

	const string& operator [](const string& name);
	const string& dest();

private:
	map<string, string> _option;
	string _dest;
	string _escape;
};

#endif

