#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
#include <fcntl.h>
using namespace std;

#define E_OK 0
#define E_ERR -1

template<class out_type, class in_value>
out_type convert(const in_value &t) {
	std::stringstream ss;
	ss << t;
	out_type result;
	ss >> result;
	return result;
}

const std::string g_sample1_dir =  "./sample1/";
const std::string g_sample2_dir =  "./sample2/";
const int BUF_LEN_MAX           =  64 * 1024;

const std::string get_sample1_file(int &i)
{
	std::string file = g_sample1_dir + convert<std::string>(i) + ".cpp";
	std::cout << file << "\n";
	return file;
}

const std::string get_sample2_file(int &i)
{
	std::string file = g_sample2_dir + convert<std::string>(i) + ".cpp";
	std::cout << file << "\n";
	return file;
}

int create_file(const std::string &input, const std::string &file)
{
	int fd = ::open(file.c_str(), O_RDWR | O_CREAT | O_APPEND, 0666);
	if (fd == -1) {
		std::cout << "open file err: " << file << "\n";
		return E_ERR;
	}

	int ret = ::write(fd, input.c_str(), input.size());
	if (ret < 0) {
		std::cout << "write file err: " << file << "\n";
		::close(fd);
		return E_ERR;
	}
	::close(fd);

	return E_OK;
}

std::string proc(const std::string &content_sample1, const std::string &content_sample2)
{

	std::string same = "Y";
	return same;
}

int main(int argc, char* argv[])
{
	// usage: check file_group_cnt
	if (argc != 2) {
		fprintf(stderr, "usage:%s file_group_cnt\n", argv[0]);
		return E_ERR;
	}
	int file_group_cnt = atoi(argv[1]);

	const std::string result_file = "result.txt";
	if (access(result_file.c_str(), F_OK) != -1) {
		if (remove(result_file.c_str()) != 0) {
			std::cout << "remove file err: " << result_file << "\n";
			return E_ERR;
		}
	}

	for (int i = 1; i != file_group_cnt + 1; ++i) {

		std::ifstream ifs_sample1;
		std::string file_sample1 = get_sample1_file(i);
		ifs_sample1.open(file_sample1.c_str(), std::ifstream::in);

		std::string content_sample1;  
		if (ifs_sample1.is_open()) {
			char *line = (char *)malloc(BUF_LEN_MAX);
			memset(line, 0x0, BUF_LEN_MAX);
			while (ifs_sample1.getline(line, BUF_LEN_MAX)) {
				content_sample1.append(line);
				content_sample1.push_back('\n');
			}
			delete line; line = NULL;
			//std::cout << "sample1 file[" << file_sample1 << "] content[" << content_sample1 << "]\n";  
			
		} else {
			std::cout << "sample1 file not found: " << file_sample1 << "\n";
		}

		std::ifstream ifs_sample2;
		std::string file_sample2 = get_sample2_file(i);
		ifs_sample2.open(file_sample2.c_str(), std::ifstream::in);

		std::string content_sample2;  
		if (ifs_sample2.is_open()) {
			char *line = (char *)malloc(BUF_LEN_MAX);
			memset(line, 0x0, BUF_LEN_MAX);
			while (ifs_sample2.getline(line, BUF_LEN_MAX)) {
				content_sample2.append(line);
				content_sample2.push_back('\n');
			}
			delete line; line = NULL;
			//std::cout << "sample2 file[" << file_sample2 << "] content[" << content_sample2 << "]\n";  

		} else {
			std::cout << "sample2 file not found: " << file_sample2 << "\n";
		}

		// TODO
		std::string same = proc(content_sample1, content_sample2);

		if (create_file(same + "\n", result_file) != E_OK) {
			std::cout << "create_file[" << result_file << "] err\n";  
			return E_ERR;
		}
	}

	return E_OK;
}


