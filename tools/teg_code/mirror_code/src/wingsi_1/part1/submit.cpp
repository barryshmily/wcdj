/*************************************************
 *
 * Author: ananliu
 * Create time: 2016-10-15 10:09:19
 * E-Mail: ananliu@tencent.com
 * version 1.1
 *
*************************************************/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>

using namespace std;
int add_zhushi(const vector<string>& input, vector<string>& output){
    output.clear();
    int count = input.size();
    for(int i = 0; i < count; i++){
        string line = input[i];
        output.push_back(line);
        //if (line.find("{") < line.length()){
        //    line += " // \\";
        //    output.push_back(line);
        //    string a = "finish";
        //    string str = "cout << \"" + a + "\" << endl;";
        //    output.push_back(str);
        //}
        //else{
        //    output.push_back(line);
        //}
    }
    string str = "/* int main(){int c = 0;*/\n// \\";
    output.push_back(str);
    str = "cout << \"finish\" << endl; retrun 0;}\n";
    output.push_back(str);
    return 0;
}

int replace_keyword(
        const map<string, string>& alias,
        const vector<string>& input,
        vector<string>& output)
{
    output.clear();
    for (vector<string>::const_iterator iter = input.begin();
            iter != input.end();
            ++iter)
    {
        if (iter->find("\"") != string::npos
                || iter->find("'") != string::npos)
        {
            output.push_back(*iter);
            continue;
        }
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
                tmp.append(1, keyword_iter->second[0]);
                tmp.append("\\\n");
                tmp.append(1, keyword_iter->second[1]);
                for (size_t idx = 2;
                        idx < keyword_iter->second.size();
                        ++idx)
                {
                    tmp.append(1, keyword_iter->second[idx]);
                }
                tmp.append(line.substr(start_pos+keyword_iter->first.size()));
                line = tmp;
                pos = start_pos + 1;
                start_pos = line.find(keyword_iter->first, pos);
                //cout << line << endl;
            }
        }
        output.push_back(line);
    }
    return 0;
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
void mirror(
        int file_id,
        const map<string, string>& alias)
{
    vector<string> inputs;
    string line;
    ostringstream input_file;
    input_file << "./input/" << file_id << ".cpp";
    /*
    ifstream fin(input_file.str().c_str());
    if (!fin || !fin.is_open())
    {
        return;
    }
    while (getline(fin, line))
    {
        inputs.push_back(line);
    }
    fin.close();
    */
    read_source_file(
            input_file.str().c_str(),
            inputs);
    vector<string> note_codes;
    add_zhushi(inputs, note_codes);
    vector<string> keyword_vec;
    replace_keyword(alias, note_codes, keyword_vec);

    ostringstream output_file;
    output_file << "./output/" << file_id << ".cpp";
    ofstream fout(output_file.str().c_str());
    for (map<string, string>::const_iterator iter = alias.begin();
            iter != alias.end();
            ++iter)
    {
        fout << "#define " << iter->second << " " << iter->first << endl;
    }
    for (vector<string>::const_iterator iter = keyword_vec.begin();
            iter != keyword_vec.end();
            ++iter)
    {
        for (size_t idx = 0; idx < iter->size(); ++idx)
        {
            if ((*iter)[idx] == '\\'
                    || (*iter)[idx] == '/'
                    || (*iter)[idx] == '*'
                    || idx == iter->size() - 1)
            {
                fout << (*iter)[idx];
            }
            else
            {
                fout << (*iter)[idx] << "\\\n";
            }
        }
        fout << endl;
    }
    fout.close();
}

int main(int argc, char** argv)
{
    map<string, string> alias;
    alias["asm"] = "a\\\ntn";
    alias["do"] = "e\\\np";
    alias["if"] = "j\\\ng";
    alias["return"] = "s\\\nfuvso";
    alias["typedef"] = "u\\\nzqfefg";
    alias["auto"] = "a\\\nvup";
    alias["double"] = "e\\\npvcmf";
    alias["inline"] = "j\\\nomjof";
    alias["short"] = "t\\\nipsu";
    alias["typeid"] = "u\\\nzqfje";
    alias["bool"] = "c\\\nppm";
    alias["dynamic_cast"] = "e\\\nzoanjd_datu";
    alias["int"] = "j\\\nou";
    alias["signed"] = "t\\\njhofe";
    alias["typename"] = "u\\\nzqfoanf";
    alias["break"] = "c\\\nsfal";
    alias["else"] = "f\\\nmtf";
    alias["long"] = "m\\\npoh";
    alias["sizeof"] = "t\\\njzfpg";
    alias["union"] = "v\\\nojpo";
    alias["case"] = "d\\\natf";
    alias["enum"] = "f\\\novn";
    alias["mutable"] = "n\\\nvuacmf";
    alias["static"] = "t\\\nuaujd";
    alias["unsigned"] = "v\\\notjhofe";
    alias["catch"] = "d\\\naudi";
    alias["explicit"] = "f\\\nyqmjdju";
    alias["namespace"] = "o\\\nanftqadf";
    alias["static_cast"] = "t\\\nuaujd_datu";
    alias["using"] = "v\\\ntjoh";
    alias["char"] = "d\\\nias";
    alias["export"] = "f\\\nyqpsu";
    alias["new"] = "o\\\nfx";
    alias["struct"] = "t\\\nusvdu";
    alias["virtual"] = "w\\\njsuvam";
    alias["class"] = "d\\\nmatt";
    alias["extern"] = "f\\\nyufso";
    alias["operator"] = "p\\\nqfsaups";
    alias["switch"] = "t\\\nxjudi";
    alias["void"] = "w\\\npje";
    alias["const"] = "d\\\npotu";
    alias["false"] = "g\\\namtf";
    alias["private"] = "q\\\nsjwauf";
    alias["template"] = "u\\\nfnqmauf";
    alias["volatile"] = "w\\\npmaujmf";
    alias["const_cast"] = "d\\\npotu_datu";
    alias["float"] = "g\\\nmpau";
    alias["protected"] = "q\\\nspufdufe";
    alias["this"] = "u\\\nijt";
    alias["wchar_t"] = "x\\\ndias_u";
    alias["continue"] = "d\\\npoujovf";
    alias["for"] = "g\\\nps";
    alias["public"] = "q\\\nvcmjd";
    alias["throw"] = "u\\\nispx";
    alias["while"] = "x\\\nijmf";
    alias["default"] = "e\\\nfgavmu";
    alias["friend"] = "g\\\nsjfoe";
    alias["register"] = "s\\\nfhjtufs";
    alias["true"] = "u\\\nsvf";
    alias["delete"] = "e\\\nfmfuf";
    alias["goto"] = "h\\\npup";
    alias["reinterpret_cast"] = "s\\\nfjoufsqsfu_datu";
    alias["try"] = "u\\\nsz";
    int x = 0;
    if (argc == 2)
    {
        istringstream sin(argv[1]);
        if (sin >> x)
        {
            // do something
        }
    }
    for (int i = 1; i <= x; ++i)
    {
        mirror(i, alias);
    }
    return 1;
}

