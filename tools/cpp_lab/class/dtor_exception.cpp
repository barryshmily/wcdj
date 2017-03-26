#include <iostream>
#include <exception>
#include <vector>
using namespace std;

class Widget {
public:
	Widget()
	{
		cout << "Widget()\n";
	}
	~Widget()
	{
		// 析构函数里如果抛出异常，需要自己捕获处理，否则会资源泄漏
		try {
			cout << "~Widget()\n";
			throw std::runtime_error("~Widget()");
		} catch (std::runtime_error &e) {
			cout << "catch exception at ~Widget()\n";
		}
	}
private:
	int v;
};

int main()
{
	try {
		vector<Widget> w_vec;
		w_vec.resize(3);

	} catch (...) {
		cout << "catch exception at main()\n";
	}

	return 0;
}
/*
Widget()
Widget()
Widget()
~Widget()
catch exception at ~Widget()
~Widget()
catch exception at ~Widget()
~Widget()
catch exception at ~Widget()
*/

