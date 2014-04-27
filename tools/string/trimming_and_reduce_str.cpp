#include <cstdio>
#include <iostream>
#include <string>
#include <ctype.h>

using namespace std;

string trim(const string& str,
		const string& whitespace = " \t")
{
	const size_t strBegin = str.find_first_not_of(whitespace);
	if (strBegin == string::npos)
		return "";// no content

	const size_t strEnd   = str.find_last_not_of(whitespace);
	const size_t strRange = strEnd - strBegin + 1;

	return str.substr(strBegin, strRange);
}

string reduce(const string& str,
		const string& fill = " ",
		const string& whitespace = " \t")
{
	// trim first
	string result = trim(str, whitespace);

	// replace sub ranges
	size_t beginSpace = result.find_first_of(whitespace);
	while (beginSpace != string::npos)
	{
		const size_t endSpace = result.find_first_not_of(whitespace, beginSpace);
		const size_t range = endSpace - beginSpace;
		
		result.replace(beginSpace, range, fill);

		const size_t newStart = beginSpace + fill.length();
		beginSpace = result.find_first_of(whitespace, newStart);
	}

	return result;
}

// strip a string, remove leading and trailling spaces
void strip(const string& in,
		string& out)
{
	string::const_iterator b = in.begin(), e = in.end();

	// skipping leading spaces
	while (isspace(*b)) {
		++b;
	}

	if (b != e) {
		// skipping trailling spaces
		while (isspace(*(e - 1))) {
			--e;
		}
	}

	out.assign(b, e);
}

int main(int argc, char **argv)
{
	const string foo = "   too much\t   \tspace\t\t\t   ";
	const string bar = "one\ntwo";

	// trimming
	cout << "[" << trim(foo) << "]" << endl;
	cout << "[" << trim(bar) << "]" << endl;

	// reduce
	cout << "[" << reduce(foo) << "]" << endl;
	cout << "[" << reduce(foo, "-") << "]" << endl;
	cout << "[" << reduce(bar) << "]" << endl;
	cout << "[" << reduce(bar, "-") << "]" << endl;

	// use isspace
	cout << "-------------------" << endl;
	string out;
	strip(foo, out);
	cout << "[" << out << "]" << endl;

	strip(bar, out);
	cout << "[" << out << "]" << endl;


	return 0;
}
/* g++ -o trimming_and_reduce_str trimming_and_reduce_str.cpp

output:
[too much	   	space]
[one
two]
[too much space]
[too-much-space]
[one
two]
[one
two]
-------------------
[too much	   	space]
[one
two]

*/

