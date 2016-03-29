#include <iostream>
#include <vector>
#include <string>

void split(const std::string& str, const std::string& delimiters, std::vector<std::string>& tokens)
{
	// Skip delimiters at beginning.
	std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	// Find first "non-delimiter".
	std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

	while (std::string::npos != pos || std::string::npos != lastPos)
	{
		// Found a token, add it to the vector.
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		// Skip delimiters. Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);
		// Find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}
}

void print_vec(std::vector<std::string> &vec)
{
	std::vector<std::string>::const_iterator cit = vec.begin();
	for (; cit != vec.end(); ++cit) {
		std::cout << *cit << std::endl;
	}
}

int main()
{
	std::vector<std::string> tokens;
	split("123;456;78,;9", ";,", tokens);
	print_vec(tokens);


	return 0;
}
/*
output:
123
456
78
9
 */
