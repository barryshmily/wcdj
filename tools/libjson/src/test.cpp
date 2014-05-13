

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

int jsonProc_for_facebook_proto(const char *buf, map<std::string, std::string> &result, string &strErrMsg)
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
		if (json_type(*i) == JSON_ARRAY)
			continue;
		if (json_type(*i) == JSON_NODE)
		{
			if (string("info") == json_name(*i))
			{
				JSONNODE *sub_jn          =  json_as_node(*i);
				json_char *all_node_value =  json_write(sub_jn);
				result["info"]            =  all_node_value ? all_node_value : "";

				JSONNODE_ITERATOR j = json_begin(sub_jn);
				for (; j != json_end(sub_jn); ++j)
				{
					if (json_type(*j) == JSON_ARRAY || json_type(*j) == JSON_NODE)
						continue;

					json_char *sub_node_name  =  json_name(*j);
					json_char *sub_node_value =  json_as_string(*j);
					result[sub_node_name]     =  sub_node_value ? sub_node_value : "";

					json_free(sub_node_name);
					json_free(sub_node_value);
				}
				json_delete(sub_jn);
			}
		}

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

