#include <cstdio>
#include <string>
#include <map>
#include <vector>
using namespace std;

int main()
{
	vector<string> in_vec;
	in_vec.push_back("ServiceCode1:ProductID1");
	in_vec.push_back("ServiceCode2:ProductID2");
	in_vec.push_back("ServiceCode3:ProductID3");

	map<string ,string> mapChildrenServiceCodeToProductId;

	if (in_vec.size() > 0)
	{
		// ∏Ò Ω: ServiceCode1:ProductID1
		map<string, string> mapItem;

		vector<string>::iterator it = in_vec.begin();
		for (; it != in_vec.end(); ++it)
		{
			mapItem.insert(make_pair(*it, *it));
		}

		const char *pch = NULL;
		map<string, string>::iterator mit = mapItem.begin();
		for (; mit != mapItem.end(); ++mit)
		{
			pch = strchr((mit->first).c_str(), ':');
			if (pch != NULL)
			{
				mapChildrenServiceCodeToProductId[string((mit->first).c_str(), pch)] = string(pch + 1);
			}
		}
	}

	map<string, string>::iterator mit = mapChildrenServiceCodeToProductId.begin();
	for (; mit != mapChildrenServiceCodeToProductId.end(); ++mit)
	{
		printf("%s->%s\n", mit->first.c_str(), mit->second.c_str());
	}

	return 0;
}
