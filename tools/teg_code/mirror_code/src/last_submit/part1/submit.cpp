#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <vector>
#include <map>
#include <set>
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

const std::string g_input_dir  =  "./input/";
const std::string g_output_dir =  "./output/";
const int BUF_LEN_MAX          =  64 * 1024;

// -------------------------------------------------
enum TokenType
{
	TOKEN_ID,
	TOKEN_CHAR,
	TOKEN_STRING, 
	TOKEN_NUMBER,
	TOKEN_FNUMBER,
	TOKEN_DOT, //.
	TOKEN_ARROW, // ->
	TOKEN_COMMA, //,
	TOKEN_ASSIGN, // =
	TOKEN_OPERATOR, // +, - , *, /, << , >>, &, |, ^ , ~, %, ++, --
	TOKEN_LOPERATOR, // <, >, &&, || , !, <=, >= , == , !=
	TOKEN_BRACKET, //(, [ , } , ), ] , }
	TOKEN_QUESTION, // ?
	TOKEN_COLON, // :
	TOKEN_USING, // ::
	TOKEN_SHARP, //#
	TOKEN_SEMI, // ;
	TOKEN_SPACE,	//   
	TOKEN_UNKNOWN
	};

struct Token
{
public:
	std::string Content;
	TokenType Type;
	Token(){}
	Token(const std::string &content, TokenType type) :
		Content(content),
		Type(type)
	{
	}
};

static std::ostream& operator << (std::ostream& os, const TokenType &type)
{
	std::string text;
	switch (type)
	{
		case TOKEN_CHAR:
			text = "Char";
			break;
		case TOKEN_STRING:
			text = "String";
			break;
		case TOKEN_NUMBER:
		case TOKEN_FNUMBER:
			text = "Number";
			break;
		case TOKEN_ID:
			text = "ID";
			break;
		case TOKEN_OPERATOR:
			text = "Operator";
			break;
		case TOKEN_BRACKET:
			text = "Bracket";
			break;
		case TOKEN_LOPERATOR:
			text = "LogicOperator";
			break;
		default:
			text = "Other";
			break;
	}
	return os << text;
}

static const int MAX_STATE = 32;
static int ParseTable[MAX_STATE][256];
static bool ParseTableInited = false;

static const int STATE_NUMBER                    =  1;
static const int STATE_FLOAT_NUMBER_DOT_START    =  2;
static const int STATE_FLOAT_NUMBER              =  3;
static const int STATE_FLOAT_NUMBER_E_START      =  4;
static const int STATE_FLOAT_NUMBER_E_SIGN_START =  5;
static const int STATE_FLOAT_NUMBER_E_SIGNED     =  6;
static const int STATE_ID                        =  7;
static const int STATE_ASSIGN                    =  8;
static const int STATE_OPERATOR                  =  9;
static const int STATE_LOPERATOR                 =  10;
static const int STATE_BRACKET                   =  11;
static const int STATE_ARROW                     =  12;
static const int STATE_COLON                     =  13;
static const int STATE_USING                     =  14;
static const int STATE_SPACE                     =  15;
static const int STATE_CHAR_START                =  16;
static const int STATE_CHAR_ESCAPE               =  17;
static const int STATE_CHAR                      =  18;
static const int STATE_CHAR_END                  =  19;
static const int STATE_STRING                    =  20;
static const int STATE_STRING_ESCAPE             =  21;
static const int STATE_STRING_END                =  22;


void InitParseTable()	/*特殊处理 \ */
{
	if (ParseTableInited)
		return;

	memset(ParseTable, -1, sizeof(ParseTable));

	ParseTable[0]['.'] = STATE_FLOAT_NUMBER_DOT_START;
	for (int i = '0'; i <= '9'; ++i) {
		ParseTable[0][i] = STATE_NUMBER;
		ParseTable[STATE_NUMBER][i] = STATE_NUMBER;
		ParseTable[STATE_FLOAT_NUMBER_DOT_START][i] = STATE_FLOAT_NUMBER;
		ParseTable[STATE_FLOAT_NUMBER][i] = STATE_FLOAT_NUMBER;

		ParseTable[STATE_FLOAT_NUMBER_E_START][i] = ParseTable[STATE_FLOAT_NUMBER_E_SIGN_START][i] = STATE_FLOAT_NUMBER_E_SIGNED;
		ParseTable[STATE_FLOAT_NUMBER_E_SIGNED][i] = STATE_FLOAT_NUMBER_E_SIGNED;

		ParseTable[STATE_ID][i] = STATE_ID;
	}

	ParseTable[STATE_NUMBER]['.'] = STATE_FLOAT_NUMBER;

	ParseTable[STATE_NUMBER]['e'] = ParseTable[STATE_FLOAT_NUMBER]['e'] = ParseTable[STATE_NUMBER]['E'] = ParseTable[STATE_FLOAT_NUMBER]['E'] = STATE_FLOAT_NUMBER_E_START;

	ParseTable[STATE_FLOAT_NUMBER_E_START]['+'] = ParseTable[STATE_FLOAT_NUMBER_E_START]['-'] = STATE_FLOAT_NUMBER_E_SIGN_START;

	for (int i = 'A'; i <= 'Z'; ++i) {
		ParseTable[0][i] = STATE_ID;
		ParseTable[0][i + 'a' - 'A'] = STATE_ID;
		ParseTable[STATE_ID][i] = STATE_ID;
		ParseTable[STATE_ID][i + 'a' - 'A'] = STATE_ID;
	}

	ParseTable[0]['_']        =  STATE_ID;
	ParseTable[STATE_ID]['_'] =  STATE_ID;

	ParseTable[0]['='] = STATE_ASSIGN;

	ParseTable[0]['+'] = ParseTable[0]['-'] = ParseTable[0]['*'] = ParseTable[0]['/'] = \
		ParseTable[0]['&'] = ParseTable[0]['|'] = ParseTable[0]['^'] = ParseTable[0]['~'] = ParseTable[0]['%'] = STATE_OPERATOR;

	ParseTable[0]['<'] = ParseTable[0]['>'] = ParseTable[0]['!'] = STATE_LOPERATOR;

	//这么写是错误的，但是输入代码的语法是正确的情况下可以这么假设
	ParseTable[STATE_OPERATOR]['&'] = ParseTable[STATE_OPERATOR]['|'] = STATE_LOPERATOR; // &&, ||
	ParseTable[STATE_LOPERATOR]['='] = STATE_LOPERATOR;	// >= , <= , !=
	ParseTable[STATE_LOPERATOR]['<'] = ParseTable[STATE_LOPERATOR]['>'] = STATE_OPERATOR;	 // << , >>
	ParseTable[STATE_OPERATOR]['+'] = ParseTable[STATE_OPERATOR]['-'] = STATE_OPERATOR;	// ++, --
	ParseTable[STATE_OPERATOR]['>'] = STATE_ARROW;
	ParseTable[STATE_ASSIGN]['='] = STATE_LOPERATOR;

	ParseTable[0]['('] = ParseTable[0][')'] = ParseTable[0]['['] = ParseTable[0][']'] = ParseTable[0]['{'] = ParseTable[0]['}'] = STATE_BRACKET;

	ParseTable[0][':'] = STATE_COLON;
	ParseTable[STATE_COLON][':'] = STATE_USING;

	ParseTable[0][' '] = ParseTable[0]['\t'] = ParseTable[0]['\r'] = ParseTable[0]['\n'] = STATE_SPACE;
	ParseTable[STATE_SPACE][' '] = ParseTable[STATE_SPACE]['\t'] = ParseTable[STATE_SPACE]['\r'] = ParseTable[STATE_SPACE]['\n'] = STATE_SPACE;

	ParseTable[0]['\''] = STATE_CHAR_START;
	for (int i = 0; i <= 255; ++i) {
		ParseTable[STATE_CHAR_START][i] = STATE_CHAR;
		ParseTable[STATE_CHAR_ESCAPE][i] = STATE_CHAR;
	}
	ParseTable[STATE_CHAR_START]['\\'] = STATE_CHAR_ESCAPE;
	ParseTable[STATE_CHAR]['\''] = STATE_CHAR_END;

	ParseTable[0]['\"'] = STATE_STRING;
	for (int i = 0; i <= 255; ++i) {
		ParseTable[STATE_STRING][i] = STATE_STRING;
		ParseTable[STATE_STRING_ESCAPE][i] = STATE_STRING;
	}
	ParseTable[STATE_STRING]['\\'] = STATE_STRING_ESCAPE;
	ParseTable[STATE_STRING]['\"'] = STATE_STRING_END;

	ParseTableInited = true;
}

std::string TrimSplash(const std::string &text)
{
	std::string res;
	const char *sp = text.c_str() , * ed, *cur;
	while ((ed = strchr(sp, '\n')) != NULL) {
		cur = ed;
		bool splah_new_line = false;
		while (cur >= sp && (*cur == '\n' || *cur == '\r' || *cur == '\t' || *cur == ' ')) --cur;
		if (cur >= sp && *cur == '\\') {
			if (!((cur - 1) >= sp && *(cur - 1) == '\\')) {
				splah_new_line = true;
				if (cur > sp) {
					res.append(std::string(sp, cur));
				}
			}
		}
		if (!splah_new_line) {
			res.append(sp, ed + 1);
		}
		sp = ed + 1;
	}
	return res;
}

// 抽取 . , ? # ;
void TokenParse(const std::string &text, std::vector<Token> &tokens)
{
	InitParseTable();

	size_t index = 0;
	const char *str = text.c_str();
	while (str[index]) {
		if (str[index] == '.' || str[index] == ',' || str[index] == '?' || str[index] == '#' || str[index] == ';')
		{
			std::string content;
			content += str[index];

			if (str[index] == '.') {
				tokens.push_back(Token(content, TOKEN_DOT));
			}
			else if (str[index] == ',') {
				tokens.push_back(Token(content, TOKEN_COMMA));
			}
			else if (str[index] == '?') {
				tokens.push_back(Token(content, TOKEN_QUESTION));
			}
			else if (str[index] == '#') {
				tokens.push_back(Token(content, TOKEN_SHARP));
			}
			else {
				tokens.push_back(Token(content, TOKEN_SEMI));
			}
			++index;
			continue;
		}
		else if (str[index] == '/' && (str[index + 1] == '/' || str[index + 1] == '*')) {
			if (str[index + 1] == '/') {
				while (str[++index] != '\n');
				++index;
			}
			else {
				index += 2;
				while (!(str[index] == '*' && str[index + 1] == '/')) ++index;
				index += 2;
			}
			continue;
		}

		size_t pre_index = index;
		int8_t state = 0, pre_state = 0;
		while (true) {
			pre_state = state;
			state = ParseTable[state][(unsigned char)str[index]];
			if (state == -1) {
				break;
			}
			++index;
		}

		std::string content(str + pre_index, index - pre_index);
		switch (pre_state)
		{
			case STATE_NUMBER:
				tokens.push_back(Token(content, TOKEN_NUMBER));
				break;
			case STATE_FLOAT_NUMBER:
			case STATE_FLOAT_NUMBER_E_SIGNED:
				tokens.push_back(Token(content, TOKEN_FNUMBER));
				break;
			case STATE_ID:
				tokens.push_back(Token(content, TOKEN_ID));
				break;
			case STATE_OPERATOR:
				tokens.push_back(Token(content, TOKEN_OPERATOR));
				break;
			case STATE_LOPERATOR:
				tokens.push_back(Token(content, TOKEN_LOPERATOR));
				break;
			case STATE_ASSIGN:
				tokens.push_back(Token(content, TOKEN_ASSIGN));
				break;
			case STATE_BRACKET:
				tokens.push_back(Token(content, TOKEN_BRACKET));
				break;
			case STATE_ARROW:
				tokens.push_back(Token(content, TOKEN_ARROW));
				break;
			case STATE_COLON:
				tokens.push_back(Token(content, TOKEN_COLON));
				break;
			case STATE_USING:
				tokens.push_back(Token(content, TOKEN_USING));
				break;
			case STATE_CHAR_END:
				tokens.push_back(Token(content, TOKEN_CHAR));
				break;
			case STATE_STRING_END:
				tokens.push_back(Token(content, TOKEN_STRING));
				break;
			case STATE_SPACE:
				tokens.push_back(Token(content, TOKEN_SPACE));
				break;
			default:
				tokens.push_back(Token(content, TOKEN_UNKNOWN));
				break;
		}
	}
}

std::string RandSName(const string &pre, const string &suf)
{
	std::string name;
	int len = rand() % 5 + 6;
	for (int i = 0; i < len; ++i) {
		name += (rand() % 26) + 'a';
	}
	if (rand() & 1) {
		name = pre + name + suf;
	}
	else
	{
		name = suf + name + pre;
	}
	return name;
}

vector<Token> SplitString(const vector<Token> &tokens)
{
	vector<Token> res;

	for (size_t i = 0;i < tokens.size(); ++i) {
		if (tokens[i].Type == TOKEN_STRING && tokens[i].Content.length() > 4 && tokens[i].Content[1] !='\\') {
			string val = tokens[i].Content.substr(0, 2) + "\"\"" + tokens[i].Content.substr(2);
			res.push_back(Token(val, TOKEN_STRING));
		} else {
			res.push_back(tokens[i]);
		}
	}
	return res;
}

vector<Token> AddDummyThings(const vector<Token> &tokens)	//加变量，函数，宏，注释，typedef
{
	vector<Token> res;

	if (rand() & 1) {
		res.push_back(Token("\\\n", TOKEN_ID));
	}

	const int Max_Add_Fun_Cnt = 5;
	vector<string> name_s;

	for (int i = 0;i < Max_Add_Fun_Cnt; ++i)
	{
		name_s.push_back(RandSName("_papa_", string("_") + char('0' + i) + string("_")));

		res.push_back(Token("int", TOKEN_ID));
		res.push_back(Token(" ", TOKEN_SPACE));
		res.push_back(Token(name_s[i], TOKEN_ID));
		res.push_back(Token("(", TOKEN_BRACKET));
		res.push_back(Token(")", TOKEN_BRACKET));
		res.push_back(Token("{", TOKEN_BRACKET));
		res.push_back(Token("return", TOKEN_ID));
		res.push_back(Token(" ", TOKEN_SPACE));
		res.push_back(Token(convert<string, int>(i + 1), TOKEN_ID));
		res.push_back(Token(";", TOKEN_SEMI));
		res.push_back(Token("}", TOKEN_BRACKET));
		res.push_back(Token("\n", TOKEN_SPACE));
	}
	int put_def_0 = (rand() % 3 <= 1);
	if (put_def_0) {
		res.push_back(Token("#", TOKEN_SHARP));
		res.push_back(Token("if", TOKEN_ID));
		res.push_back(Token(" ", TOKEN_SPACE));
		int val = rand();
		res.push_back(Token(convert<string, int>(val), TOKEN_NUMBER));
		res.push_back(Token("-", TOKEN_OPERATOR));
		res.push_back(Token(convert<string, int>(val), TOKEN_NUMBER));
		res.push_back(Token("\n", TOKEN_SPACE));
		std::string dec_msg;
		for (int j = 128; j >= 0; --j) {
			dec_msg += RandSName("", "") + " ";
		}
		res.push_back(Token(dec_msg, TOKEN_SPACE));
		res.push_back(Token("\n", TOKEN_SPACE));
		res.push_back(Token("#", TOKEN_SHARP));
		res.push_back(Token("else", TOKEN_ID));
		res.push_back(Token("\n", TOKEN_SPACE));
	}

	int max_comment_cnt = 32;

	for (size_t i = 0;i < tokens.size(); ++i) {
		if (tokens[i].Type == TOKEN_ID && (tokens[i].Content == "int" || tokens[i].Content == "double" || tokens[i].Content == "long")) {	//char is to short
			if (max_comment_cnt > 0 && (rand() % tokens.size()) < max_comment_cnt) {
				res.push_back(Token(" /*team_7s 666! */", TOKEN_SPACE));
				--max_comment_cnt;
			}
			res.push_back(tokens[i]);
			if (max_comment_cnt > 0 && (rand() % tokens.size()) < max_comment_cnt) {
				res.push_back(Token(" /*team_7s 999! */", TOKEN_SPACE));
				--max_comment_cnt;
			}
		}
		else
		{
			res.push_back(tokens[i]);
		}
	}

	if (put_def_0) {
		res.push_back(Token("#", TOKEN_SHARP));
		res.push_back(Token("endif", TOKEN_ID));
		res.push_back(Token("\n", TOKEN_SPACE));
	}
	return res;
}

vector<Token> ReplaceConstNumber(const vector<Token> &tokens)
{
	vector<Token> res;

	for (size_t i = 0;i < tokens.size(); ++i) {
		if (tokens[i].Type == TOKEN_NUMBER && tokens[i].Content.length() < 8) {
			int st = rand() % 2;
			int val = atoi(tokens[i].Content.c_str());
			if (st == 0) {
				int a = rand();
				int b = val - a;
				res.push_back(Token("(", TOKEN_BRACKET));
				res.push_back(Token(convert<string, int>(a), TOKEN_NUMBER));
				res.push_back(Token("+", TOKEN_OPERATOR));
				res.push_back(Token(convert<string, int>(b), TOKEN_NUMBER));
				res.push_back(Token(")", TOKEN_BRACKET));
			}
			else if (st == 1) {
				res.push_back(Token("(", TOKEN_BRACKET));
				res.push_back(Token("1", TOKEN_NUMBER));
				res.push_back(Token("==", TOKEN_LOPERATOR));
				res.push_back(Token("1", TOKEN_NUMBER));
				res.push_back(Token("?", TOKEN_QUESTION));
				res.push_back(Token(tokens[i].Content, TOKEN_NUMBER));
				res.push_back(Token(":", TOKEN_COLON));
				res.push_back(Token(tokens[i].Content, TOKEN_NUMBER));
				res.push_back(Token(")", TOKEN_BRACKET));
			}
		}
		else
		{
			res.push_back(tokens[i]);
		}
	}
	return res;
}

vector<Token> ReplaceVarName(vector<Token> &tokens)
{
	int level = 0;
	vector<Token> res;
	for (size_t i = 0;i < tokens.size(); ++i) {
		if (tokens[i].Content == "{") {

		}
	}
	return res;
}

vector<Token> ReplaceFor(vector<Token> &tokens)
{
	vector<Token> res;
	int pos = 0;
	while (pos < tokens.size()) {
		if (tokens[pos].Content == "for") {
			++pos;
			while (pos < tokens.size() && tokens[pos].Content != "(") ++pos;
			++pos;
			vector<Token> fisrt_part;
			while (pos < tokens.size() && tokens[pos].Content != ";") {
				fisrt_part.push_back(tokens[pos]);
				++pos;
			}
			fisrt_part.push_back(Token(";", TOKEN_SEMI));
			++pos;
			vector<Token> second_part;
			while (pos < tokens.size() && tokens[pos].Content != ";") {
				second_part.push_back(tokens[pos]);
				++pos;
			}
			++pos;
			vector<Token> third_part;
			while (pos < tokens.size() && tokens[pos].Content != ")") {
				third_part.push_back(tokens[pos]);
				++pos;
			}
			third_part.push_back(Token(";", TOKEN_SEMI));
			++pos;
			vector<Token> content;
			while (pos < tokens.size() && tokens[pos].Content != "{" && tokens[pos].Content != ";") {
				content.push_back(tokens[pos]);
				++pos;
			}
			if (pos < tokens.size() ) {
				if (tokens[pos].Content == ";")
				{
					content.push_back(tokens[pos]);
				}
				else {
					content.push_back(tokens[pos]);
					int lv = 1;
					while (pos < tokens.size() && lv > 0) {
						++pos;
						if (tokens[pos].Content == "{") ++lv;
						else if (tokens[pos].Content == "}") --lv;
						content.push_back(tokens[pos]);
					}
				}
			}
			res.push_back(Token("{", TOKEN_BRACKET));
			res.push_back(Token("\n", TOKEN_SPACE));
			for (size_t i = 0;i < fisrt_part.size(); ++i) {
				res.push_back(fisrt_part[i]);
			}
			res.push_back(Token("\n", TOKEN_SPACE));
			res.push_back(Token("while", TOKEN_ID));
			res.push_back(Token("(", TOKEN_BRACKET));
			for (size_t i = 0;i < second_part.size(); ++i) {
				res.push_back(second_part[i]);
			}
			res.push_back(Token(")", TOKEN_BRACKET));

			res.push_back(Token("{", TOKEN_BRACKET));
			res.push_back(Token("\n", TOKEN_SPACE));

			for (size_t i = 0;i < content.size(); ++i) {
				res.push_back(content[i]);
			}
			res.push_back(Token("\n", TOKEN_SPACE));

			for (size_t i = 0;i < third_part.size(); ++i) {
				res.push_back(third_part[i]);
			}
			res.push_back(Token("}", TOKEN_BRACKET));
			res.push_back(Token("\n", TOKEN_SPACE));
			res.push_back(Token("}", TOKEN_BRACKET));
			res.push_back(Token("\n", TOKEN_SPACE));
		}
		else
		{
			res.push_back(tokens[pos]);
		}
		++pos;
	}

	return res;
}

vector<Token> ReplaceFuncName(const vector<Token> &tokens)
{
	vector<Token> res;
	map<string, string> def_list;
	set<string> mac_names;
	int pos = 0;
	while (pos < tokens.size()) {
		if (tokens[pos].Type == TOKEN_ID) {
			if ((pos + 1) < tokens.size() && tokens[pos + 1].Type == TOKEN_SPACE) {
				if ((pos + 2) < tokens.size() && tokens[pos + 2].Type == TOKEN_ID) {
					if (((pos + 3) < tokens.size() && tokens[pos + 3].Content == "(") || (((pos + 3) < tokens.size() && tokens[pos + 3].Type == TOKEN_SPACE) && ((pos + 4) < tokens.size() && tokens[pos + 4].Content == "("))) {
						if (def_list.find(tokens[pos + 2].Content) == def_list.end()) {
							string mac_name = RandSName("team_7s", "haha");
							while (mac_names.find(mac_name) != mac_names.end()) {
								mac_name = RandSName("team_7s", "haha");
							}
							def_list.insert(make_pair(tokens[pos + 2].Content, mac_name));
							mac_names.insert(mac_name);
						}
					}
				}
			}
		}
		++pos;
	}
	for (map<string, string>::iterator ite = def_list.begin(); ite != def_list.end(); ++ite) {
		res.push_back(Token("#", TOKEN_SHARP));
		res.push_back(Token("define", TOKEN_ID));
		res.push_back(Token(" ", TOKEN_SPACE));
		res.push_back(Token(ite->second, TOKEN_ID));
		res.push_back(Token(" ", TOKEN_SPACE));
		res.push_back(Token(ite->first, TOKEN_ID));
		res.push_back(Token("\n", TOKEN_SPACE));
	}
	for (size_t i = 0;i < tokens.size(); ++i) {
		if (tokens[i].Type == TOKEN_ID && def_list.find(tokens[i].Content) != def_list.end()) {
			res.push_back(Token(def_list[tokens[i].Content], TOKEN_ID));
		}
		else
		{
			res.push_back(tokens[i]);
		}
	}
	return res;
}

std::string ToCode(const vector<Token> & tokens) 
{
	std::string res;
	for (size_t i = 0;i < tokens.size(); ++i) {
		res += tokens[i].Content;
	}
	return res;
}

// -------------------------------------------------

const std::string get_inputfile(int &i)
{
	std::string file = g_input_dir + convert<std::string>(i) + ".cpp";
	std::cout << file << "\n";
	return file;
}

const std::string get_outputfile(int &i)
{
	std::string file = g_output_dir + convert<std::string>(i) + ".cpp";
	std::cout << file << "\n";
	return file;
}

int create_file(const std::string &input, std::string &file)
{
	int fd = ::open(file.c_str(), O_RDWR | O_CREAT, 0666);
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

std::string proc(const std::string &content)
{
	std::string str = TrimSplash(content);
	//cout << str << endl;

	vector<Token> tokens;
	TokenParse(str, tokens);

	for (size_t i = 0; i < tokens.size(); ++i) {
		//std::cout << tokens[i].Type << " : " << tokens[i].Content << endl;
	}

	tokens = SplitString(tokens);
	tokens = ReplaceConstNumber(tokens);
	tokens = ReplaceFuncName(tokens);
	tokens = ReplaceFor(tokens);
	tokens = AddDummyThings(tokens);

	return ToCode(tokens);
}

int main(int argc, char* argv[])
{
	// usage: submit file_cnt
	if (argc != 2) {
		fprintf(stderr, "usage:%s file_cnt\n", argv[0]);
		return E_ERR;
	}
	int file_cnt = atoi(argv[1]);
	srand(time(NULL));

	for (int i = 1; i != file_cnt + 1; ++i) {
		std::ifstream ifs;
		std::string file = get_inputfile(i);
		ifs.open(file.c_str(), std::ifstream::in);

		if (ifs.is_open()) {
			std::string content;  
			char *line = (char *)malloc(BUF_LEN_MAX);
			memset(line, 0x0, BUF_LEN_MAX);
			
			while (ifs.getline(line, BUF_LEN_MAX)) {
				content.append(line);
				content.push_back('\n');
			}
			delete line; line = NULL;
			//std::cout << "file[" << file << "] content[" << content << "]\n";  

			// TODO
			std::string new_content = proc(content);

			std::string new_file = get_outputfile(i);
			if (create_file(new_content, new_file) != E_OK) {
				std::cout << "create_file[" << new_file << "] err\n";  
				return E_ERR;
			}

		} else {
			std::cout << "file not found: " << file << "\n";
		}
	}

	return E_OK;
}


