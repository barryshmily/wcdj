#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main()
{
	//vector<string> vs = {"1", "+5", "-3"};
	vector<string> vs;
	vs.push_back("1");
	vs.push_back("+5");
	vs.push_back("-3");

	int sum = 0;

	//for (auto iter = vs.begin(); iter != vs.end(); ++iter) {
	for (vector<string>::iterator iter = vs.begin(); iter != vs.end(); ++iter) {
		sum += stoi(*iter);
	}

	cout << "sum = " << sum << endl;
	return 0;
}
