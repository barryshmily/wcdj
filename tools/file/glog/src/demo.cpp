#include <iostream>
#include <glog/logging.h>

int main(int argc, char* argv[])
{
	FLAGS_log_dir = "../log";
	google::InitGoogleLogging(argv[0]);

	int a = 1;
	LOG(INFO) << "gerry test, a[" << a << "]";

	return 0;
}
