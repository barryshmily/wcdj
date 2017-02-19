#include "COption.h"
#include "CParsepara.h"

void COption::read_arg(int argc, char **argv) throw(runtime_error)
{
	for (int i = 1; i < argc; ++i) 
	{
		// ./prog -para1=value1 -para2=value2 ... dest
		if (argv[i][0] == '-') 
		{
			wcdj::util::CSpacePara para("=");
			para.decode(argv[i] + 1);// argv[i] + 1 -> para1=value1

			const vector<string>& v = para.get_pairs();
			if (v.size() != 2) 
			{
				throw runtime_error(string("option invalid:") + argv[i]);
			}

			_option[v[0]] = v[1];
		} 
		else 
		{
			_dest = argv[i];
			break;
		}
	}
}

const string& COption::operator [](const string& name)
{
	map<string, string>::const_iterator it = _option.find(name);
	if (it == _option.end()) return _escape;
	return (*it).second;
}

