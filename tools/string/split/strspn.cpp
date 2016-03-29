#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <string>

/* 支持解析每个元素的最大长度为128字节
 * */
void split_str(const char* orig, const char* splitor, std::vector<std::string>& result)
{
	const int element_max_len = 128;

	size_t head = strspn(orig, splitor);
	size_t end = 0;

	const char * head_var = orig + head;
	const char * end_var = head_var;

	while ('\0' != *head_var) {

		char element[element_max_len] = {0};
		end = strcspn(head_var, splitor);
		end_var = head_var + end;
		if ('\0' == *end_var ) {
			strncpy(element, head_var, sizeof(element));
			result.push_back(element);
			break;
		}
		strncpy(element, head_var, end_var - head_var);
		result.push_back(element);
		head = strspn(end_var, splitor);
		head_var = end_var + head;
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
	std::vector<std::string> result;
	split_str("123;456;78,;9", ";,", result);
	print_vec(result);

	return 0;
}
/*
output:
123
456
78
9
 */
