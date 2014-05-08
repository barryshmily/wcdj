

int jsonProc(const char *buf, map<std::string, std::string> &result, string &strErrMsg)
{
	JSONNODE *jn = json_parse(buf);
	if (!jn)
	{
		strErrMsg = string("json_parse fail");
		return -1;
	}

	JSONNODE_ITERATOR i = json_begin(jn);
	for (; i != json_end(jn); ++i)
	{
		if (json_type(*i) == JSON_ARRAY || json_type(*i) == JSON_NODE)
			continue;
		json_char *node_name  =  json_name(*i);
		json_char *node_value =  json_as_string(*i);

		string s =  node_name;
		//transform(s.begin(), s.end(), s.begin(), ::tolower);

		result[s] = node_value ? node_value : "";

		json_free(node_name);
		json_free(node_value);
	}

	json_delete(jn);
	return 0;
}

