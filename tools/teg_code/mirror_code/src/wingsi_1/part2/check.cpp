/*************************************************
 *
 * Author: viperhan
 * Create time: 2016-10-15 10:51:50
 * E-Mail: viperhan@tencent.com
 *
*************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>

#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include <map>

#include <fstream>
#include <sstream>

using namespace std;

string tags[] = {
"sm",
"do",
"if",
"return",
"typedef",
"auto",
"double",
"inline",
"short",
"typeid",
"bool",
"dynamic_cast",
"int",
"signed",
"typename",
"break",
"else",
"long",
"sizeof",
"union",
"case",
"enum",
"mutable",
"static",
"unsigned",
"catch",
"explicit",
"namespace",
"static_cast",
"using",
"char",
"export",
"new",
"struct",
"virtual",
"class",
"extern",
"operator",
"switch",
"void",
"const",
"false",
"private",
"template",
"volatile",
"const_cast",
"float",
"protected",
"this",
"wchar_t",
"continue",
"for",
"public",
"throw",
"while",
"default",
"friend",
"register",
"true",
"delete",
"goto",
"reinterpret_cast",
"try"};

void split(const std::string& source, std::vector<std::string>& ret,
    int maxsplit=-1) {
  std::string str(source);
  int numsplit = 0;
  size_t pos;
  for (pos = 0; pos < str.size() && (str[pos] == ' ' || str[pos] == '\t'
        || str[pos] == '\n' || str[pos] == '\r'); ++ pos);
  str = str.substr(pos);

  ret.clear();
  while (str.size() > 0) {
    pos = std::string::npos;

    for (pos = 0; pos < str.size() && (str[pos] != ' ' 
          && str[pos] != '\t'
          && str[pos] != '\r'
          && str[pos] != '\n'
          && str[pos] != '{'
          && str[pos] != '}'
          && str[pos] != '('
          && str[pos] != ')'
          && str[pos] != ';'
          && str[pos] != ','
          ); ++ pos);

    if (pos == str.size()) {
      pos = std::string::npos;
    }   

    if (maxsplit >= 0 && numsplit < maxsplit) {
      ret.push_back(str.substr(0, pos));
      ++ numsplit;
    } else if (maxsplit >= 0 && numsplit == maxsplit) {
      ret.push_back(str);
      ++ numsplit;
    } else if (maxsplit == -1) {
      ret.push_back(str.substr(0, pos));
      ++ numsplit;
    }   

    if (pos == std::string::npos) {
      str = "";
    } else {
      for (; pos < str.size() && (str[pos] == ' '
            || str[pos] == '\t'
            || str[pos] == '\n'
            || str[pos] == '\r'
            || str[pos] == '{'
            || str[pos] == '}'
            || str[pos] == '('
            || str[pos] == ')'
            || str[pos] == ';'
            || str[pos] == ','
            ); ++ pos);
        str = str.substr(pos);
    }   
  }
}

void trim(std::string& str) {
  int len = (int)str.size();
  if (len == 0) { return; }
  while (len -1 >=0 && (str[len-1] == ' ' 
        || str[len-1]=='\t'
        || str[len-1] == '\r'
        || str[len-1] == '\n')) {
    -- len;
  }
  str = str.substr(0, len);

  int i = 0;
  while (i < len && (str[i] == ' ' ||
         str[i] == '\t' ||
         str[i] == '\r' ||
         str[i] == '\n')) { i ++; }
  str = str.substr(i);
}

map<string, int> feature_space;

void read_source_file(
        const char* file,
        vector<string>& codes)
{
    codes.clear();
    ifstream fin(file);
    if (!fin || !fin.is_open())
    {
        return;
    }
    string line;
    bool flag = false;
    while (getline(fin, line))
    {
        if (line.size() != 0)
        {
            bool append_flag = flag;
            if (codes.size() == 0)
            {
                append_flag = false;
            }
            if (line[line.size() - 1] == '\\')
            {
                flag = true;
                line = line.substr(0, line.size() - 1);
            }
            else
            {
                flag = false;
            }

            if (append_flag)
            {
                codes[codes.size()-1].append(line);
            }
            else
            {
                codes.push_back(line);
            }
        }
    }
    fin.close();
}

void build_feature_space()
{
    int offset = 0;
    for(int i = 0; i < 63; ++i)
    {
        feature_space.insert(make_pair(tags[i], offset));
        offset ++;
    }

    feature_space["for"] = 54;
}


int replace_keyword(
        const map<string, string>& alias,
        const vector<string>& input,
        vector<string>& output)
{
    for (vector<string>::const_iterator iter = input.begin();
            iter != input.end();
            ++iter)
    {
        string line = *iter;
        for (map<string, string>::const_iterator keyword_iter = alias.begin();
                keyword_iter != alias.end();
                ++keyword_iter)
        {
            int pos = 0;
            size_t start_pos = line.find(keyword_iter->first, pos);
            while (start_pos != string::npos
                    && line.size() > start_pos + keyword_iter->first.size() + 1
                    && line[start_pos + keyword_iter->first.size()] == ' ')
            {
                string tmp = line.substr(0, start_pos);
                for (size_t idx = 0;
                        idx < keyword_iter->second.size();
                        ++idx)
                {
                    tmp.append(1, keyword_iter->second[idx]);
                }
                tmp.append(line.substr(start_pos+keyword_iter->first.size()));
                line = tmp;
                pos = start_pos + 1;
                start_pos = line.find(keyword_iter->first, pos);
            }
        }
        output.push_back(line);
    }
    return 0;
}

int clean_notes(const vector<string>& input, vector<string>& output)
{
    string text;

    string line;
    bool is_num_note = false;
    bool is_if_note = false;
    bool is_num_note_first = true;
    bool is_if_note_first = true;
    for (vector<string>::const_iterator iter = input.begin();
            iter != input.end();
            ++iter)
    {
        line = (*iter);
        size_t end = 0;
        if (!is_if_note && (end = line.find("#if")) != string::npos)
        {
            for (size_t begin = end; begin < line.size(); ++begin)
            {
                if (line[begin] == '0')
                {
                    line = line.substr(0, end);
                    is_if_note = true;
                }
            }
        }
        if (!is_num_note && (end = line.find("/")) != string::npos)
        {
            char ch = line[end+1];
            if (ch == '/')
            { 
                line = line.substr(0, end);
            }
            else if (ch == '*')
            {
                line = line.substr(0, end);
                is_num_note = true;
            }
        }
        if (is_num_note && (end = line.find("*/")) != string::npos)
        {
            line.replace(0, end+2-0, "");
            is_num_note = false;
        }
        if (is_if_note && (end = line.find("#endif")) != string::npos)
        {
            line.replace(0, end+6-0, "");
            is_if_note = false;
        }

        if (!is_num_note && !is_if_note)
        {
            if (line.size() != 0)
            {
                output.push_back(line);
            }
        }
        if (is_num_note_first)
        {
            if (line.size() != 0)
            {
                output.push_back(line);
            }
            is_num_note_first = false;
        }
        if (is_if_note_first)
        {
            if (line.size() != 0)
            {
                output.push_back(line);
            }
            is_if_note_first = false;
        }
    }
    
    return 1;
}

int add_notes(const vector<string>& input, vector<string>& output)
{
    for (vector<string>::const_iterator iter = input.begin();
            iter != input.end();
            ++iter)
    {
        ;
    }
    return 1;
}

void hong_expand(const vector<string> & input, vector<string> & output)
{
    map<string, string> hong;
    vector<string> input_tmp;
    for (size_t j = 0; j < input.size(); ++j)
    {
        string cur = input[j];
        trim(cur);
        istringstream sin(cur);

        string flag, key, value;
        if (sin >> flag >> key >> value)
        {   

            if (flag == "#define")
            {
                hong.insert(make_pair(key, value));
            }
            else
            {
                input_tmp.push_back(input[j]);
            }
        }
        else
        {
            input_tmp.push_back(input[j]);
        }
    }
    replace_keyword(hong, input_tmp, output);
}

void extract_feature(char * path, int * instance, map<string, int>& token_num)
{
    string line;
    vector<string> raw_codes;
    vector<string> codes_without_notes;
    vector<string> codes;
    token_num.clear();

    read_source_file(path, raw_codes);
    //for (size_t j = 0; j < raw_codes.size(); ++j)
    //{
    //    cout << raw_codes[j] << endl;
    //}
    clean_notes(raw_codes, codes_without_notes);
    hong_expand(codes_without_notes, codes);
    for (size_t j = 0; j < codes.size(); ++j)
    {
        //cout << codes[j] << endl;
        vector<string> words;
        split(codes[j], words);
        for (size_t i = 0; i < words.size(); ++i)
        {
             map<string ,int>::iterator iter; 
             iter = feature_space.find(words[i]);
             if (iter != feature_space.end())
             {
                 instance[iter->second] += 1;
             }
             iter = token_num.find(words[i]);
             if (iter == token_num.end())
             {
                 token_num[words[i]] = 1;
             }
             else
             {
                 ++token_num[words[i]];
             }
        }
    }
}

float calculate_score(int * a, int * b)
{
    float sum_a = 0.0, sum_b = 0.0, dot_product = 0.0;
    for (int i = 0; i < 63; i++)
    {
        sum_a += 1.0 * a[i] * a[i];
        sum_b += 1.0 * b[i] * b[i];
        dot_product += 1.0 * a[i] * b[i];
    }

    sum_a = sqrt(sum_a);
    sum_b = sqrt(sum_b);

    return  dot_product / (sum_a * sum_b);
}

int get_token_len(const map<string, int>& tokens)
{
    map<string, int>::const_iterator t;
    int sum = 0;
    for (t = tokens.begin();
            t != tokens.end();
            ++t)
    {
        if (t->first != "")
        {
            sum += t->second * t->second;
            //cout << t->first << ':' << t->second << ' ';
        }
    }
    //cout << endl;
    return sum;
}

float calculate_score(
        const map<string, int>& tokens1,
        const map<string, int>& tokens2)
{
    map<string, int>::const_iterator t1, t2;
    int len1 = get_token_len(tokens1);
    int len2 = get_token_len(tokens2);
    if (len1 == 0
            || len2 == 0)
    {
        return 0.0;
    }
    int sum = 0;
    for (t1 = tokens1.begin();
            t1 != tokens1.end();
            ++t1)
    {
        if (t1->first == "")
        {
            continue;
        }
        t2 = tokens2.find(t1->first);
        if (t2 != tokens2.end())
        {
            sum += t1->second * t2->second;
        }
        //if (t1->first == "#include")
        //{
        //    cout << t1->second << ' ' << t2->second << endl;
        //}
    }
    //cout << sum << ' ' << len1 << ' ' << len2 << endl;
    return 1.0 * sum /(sqrt(1.0 * len1) * sqrt(1.0 * len2));
}

int main(int argc, char** argv)
{
    map<string, int>::iterator iter;
    build_feature_space();

    char buffer1[100];
    char buffer2[100];

    ofstream out("result.txt");
    int N = atoi(argv[1]);

    int count = 0;
    for (int i = 0; i < N; ++i)
    {
        sprintf(buffer1, "sample1/%d.cpp", i + 1);
        sprintf(buffer2, "sample2/%d.cpp", i + 1);
        int instance1[63] = {0,};
        int instance2[63] = {0,};
        map<string, int> tokens1;
        map<string, int> tokens2;
        extract_feature(buffer1, instance1, tokens1);
        extract_feature(buffer2, instance2, tokens2);
        float score = calculate_score(instance1, instance2);
        float token_score = calculate_score(tokens1, tokens2);
        char sim_flag = 'N';
        if (score > 0.75
                || token_score > 0.7)
        {
            sim_flag = 'Y';
        }
        //cout << "kenword_score=" << score << "\ttoken_score=" << token_score << endl;
        out << sim_flag << endl;
    }

    return 0;
}

