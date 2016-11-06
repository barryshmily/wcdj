#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <string>
#include <stdint.h>
#include <ctype.h>
#include <iostream>
#include <set>
#include <stdlib.h>
#include <vector>

using namespace std;

#define LOWER_CASE(ch) (('A' <= (ch) && (ch) <= 'Z') ? (ch) - 'A' + 'a' : (ch))

inline bool IsBlank(char ch)
{
    switch(ch)
    {
        case ' ':
        case '\t':
        case '\r':
        case '\n':
            return true;

        default:
            return false;
    }
    return false;
}

inline bool IsSplit(char ch)
{
    return IsBlank(ch) || strchr("+-[](){}*/<>.&^%!~|:?=,\\", ch);
}

inline bool IsFileName(char ch)
{
    switch(ch)
    {
        case '.':
        case '/':
        case '_':
        case '-':
            return true;

        default:
            return isalnum(ch);
    }
    return false;
}


size_t FindLineTail(const string &sStr, int iHeadPos)
{
    while (iHeadPos < (int)sStr.length())
    {
        size_t ulNext = sStr.find('\n', iHeadPos);
        if (string::npos == ulNext)
        {
            return string::npos;
        }
        int j;
        for (j = ulNext - 1; j > iHeadPos; --j)
        {
            if (!IsBlank(sStr[j]))
            {
                break;
            }
        }

        if (j > iHeadPos && sStr[j] == '\\')
        {
            iHeadPos = ulNext + 1;
            continue;
        }
        return ulNext;
    }
    
    return string::npos;
}

    
size_t FindStrTail(const string &sStr, int iHeadPos)
{
    size_t ulMax = FindLineTail(sStr, iHeadPos);
    
    for (uint32_t j = iHeadPos + 1; j < sStr.length() && (uint64_t)j < (uint64_t)ulMax; ++j)
    {
        if (sStr[j] == '\\')    // 转义
        {
            ++j;
            continue;
        }
        
        if (sStr[j] == sStr[iHeadPos])
        {
            return j;
        }
    }

    return ulMax;

}


string FilterComment(string sStr)
{
    for (uint32_t i = 0; i < sStr.length(); ++i)
    {
        if (sStr[i] == '"' || sStr[i] == '\'') // 跳过字符串或字符
        {
            size_t ulNext = FindStrTail(sStr, i);
            if (ulNext == string::npos)
            {
                break;
            }
            i = ulNext;
            continue;
        }

        if (sStr[i] == '/')
        {
            size_t iEnd = 0;
            if (sStr[i + 1] == '/')     // 形如 // 的注释
            {
                iEnd = FindLineTail(sStr, i + 2);
                if (iEnd == string::npos)
                {
                    sStr.resize(i);
                    return sStr;
                }
                else
                {
                    if (sStr[iEnd - 1] == '\r')
                    {
                        --iEnd;
                    }
                    sStr.erase(i, iEnd - i);
                }
                --i;
                continue;
            }
            else if (sStr[i + 1] == '*')   // 形如 /
            {
                iEnd = sStr.find("*/", i + 2); /* faf" fasd*/
                if (iEnd == string::npos)
                {
                    sStr.resize(i);
                    return sStr;
                }
                else
                {
                    sStr.erase(i, iEnd - i + 2);
                }
                --i;
                continue;
            } 

        }
    }

    return sStr;
}

string LoadSource(const char *szFileName)
{
    FILE *pFile = fopen(szFileName, "rb");
    if (pFile == NULL)
    {
        fprintf(stderr, "Can not open file :%s.\n", szFileName);
        exit(-1);
    }

    string sResult;
    char szBuffer[1024 * 8];

    int iRet;

    while ((iRet = fread(szBuffer, 1, sizeof szBuffer, pFile)) > 0)
    {
        sResult.append(szBuffer, iRet);
    }
    fclose(pFile);
    return sResult;
}


static int ConvertChar(char ch)    // 处理字符串的字符转义
{
    switch(ch)
    {
        case 'a': return'\a';
        case 'b': return'\b';
        case 'r': return'\r';
        case 'n': return'\n';
        case 't': return'\t';
        case 'v': return'\v';
        case '\\': return'\\';
        case '\'': return'\'';
        case '\"': return'\"';
        case '0': return'\0';
        case '\n': return '\n';
    }
    return -1;
}

string FmtString(const string &sStr)
{

    if (sStr[0] != '"')
    {
        return sStr;
    }
    
    string sResult;
    string sTemp;
    const char *p = sStr.c_str() + 1;
    while (*p)
    {
        if (*p == '\\')     // 转义符号
        {
            ++p;

            if (isdigit(*p))      // 八进制表示
            {
                sTemp.clear();
                while (isdigit(*p) && sTemp.length() < 3)
                {
                    sTemp.push_back(*p++);
                }
                if (sTemp.length() <= 0)
                {
                    return sStr;
                }
                sResult.push_back((char)strtol(sTemp.c_str(), NULL, 8));
            }
            else if (*p == 'x' || *p == 'X')     // 十六进制表示
            {
                ++p;
                sTemp.clear();
                while (isxdigit(*p) && sTemp.length() < 2)
                {
                    sTemp.push_back(*p++);
                }
                if (sTemp.length() <= 0)
                {
                    return sStr;
                }
                int iVal = strtol(sTemp.c_str(), NULL, 16);
                sResult.push_back((char)iVal);
            }
            else    // 其他情况, 查转义表
            {
                int iRet = ConvertChar(*p);
                if (iRet == -1)
                {
                    return sStr;
                }
                
                sResult.push_back((char)iRet);
                ++p;
            }
        }
        else if (*p == '"')
        {
            char szBuffer[32];
            sTemp = "\"";
            for (uint32_t i = 0; i < sResult.length(); ++i)
            {
                sprintf(szBuffer, "\\x%02x", (uint8_t)sResult[i]);
                sTemp += szBuffer;
            }

            sTemp.push_back('"');
            return sTemp;
        }
        else
        {
            sResult.push_back(*p++);
        }
        
    }

    return sStr;
}


class CSourcePaser
{
public:


    int ParseHeader(const char *szCodeSeg, int iLen)
    {
        int i, j;
        for (i = 0; i < iLen; ++i)
        {
            if (IsFileName(szCodeSeg[i]))
            {
                break;
            }
        }
        for (j = i + 1; j < iLen; ++j)
        {
            if (!IsFileName(szCodeSeg[j]))
            {
                break;
            }
        }

        m_setHeaderList.insert(string(szCodeSeg + i, j - i));

        size_t ulNext = FindLineTail(string(szCodeSeg + j, iLen - j), 0);
        if (ulNext == string::npos)
        {
            return iLen;
        }
        return ulNext + j;
    }


    // 提取整数、名字等信息, 返回使用长度
    int ParseCodeSeg(const char *szCodeSeg, int iLen, char cPrefix)
    {
        if (isalpha(szCodeSeg[0]) || szCodeSeg[0] == '_')
        {
            //name
            int i;
            for (i = 1; i < iLen; ++i)
            {
                if (!isalnum(szCodeSeg[i]) && szCodeSeg[i] != '_')
                {
                    break;
                }
            }

            m_setNameList.insert(string(szCodeSeg, i));
            return i;
        }

        char *pEnd;
        uint64_t ullVal;
        if (isdigit(szCodeSeg[0]))
        {
            if (cPrefix == '.')
            {
                double dVal = strtod(szCodeSeg - 1, &pEnd);   // 数字前面连着个小数点
                m_setDoubleList.insert(dVal);
                return pEnd - (char*)szCodeSeg;
            }

            if (szCodeSeg[0]=='0')
            {
                if (szCodeSeg[1] == 'x' || szCodeSeg[1] == 'x')   // 十六进制
                {
                    ullVal = strtoull(szCodeSeg, &pEnd, 16);
                }
                else        // 8进制
                {
                    ullVal = strtoull(szCodeSeg, &pEnd, 8);
                }
            }
            else
            {
                // 十进制整数
                ullVal = strtoull(szCodeSeg, &pEnd, 10);
            }

            if ('.' == *pEnd || 'e' == LOWER_CASE(*pEnd) )
            {
                // 浮点
                double dVal = strtod(szCodeSeg, &pEnd); 
                m_setDoubleList.insert(dVal);
                return pEnd - (char*)szCodeSeg;
            }

            while (*pEnd)
            {
                char cLower = LOWER_CASE(*pEnd++);
                if (cLower != 'u' && cLower != 'l')
                {
                    break;
                }
            }
            
            m_setIntList.insert(ullVal);
            
            return pEnd - (char*)szCodeSeg;
        }


        int i;
        for (i = 1; i < iLen; ++i)
        {
            if (isalnum(szCodeSeg[i]) || szCodeSeg[i] == '_' || IsBlank(szCodeSeg[i]) || szCodeSeg[i] == '\'' || szCodeSeg[i] == '"')
            {
                break;
            }
        }
        m_setOpList.insert(string(szCodeSeg, i));
        return i;
        
    }


    // 处理源码, 干掉注释, 提取字符串、数值、头文件等数据
    // 返回去除注释之后的代码
    string ParseSource(const string &sStr)
    {
        m_sOrigSource = sStr;
        
        m_setStrList.clear();
        m_setIntList.clear();
        m_setDoubleList.clear();
        m_setNameList.clear();
        m_setOpList.clear();
        m_setHeaderList.clear();

        string sCode = FilterComment(sStr);
        m_sFilterSource = sCode;

        for (uint32_t i = 0; i < sCode.length(); ++i)
        {
            if ('"' == sCode[i] ||  '\'' == sCode[i])
            {
                
                // 字符串
                size_t ulNext = FindStrTail(sCode, i);
                if (ulNext == string::npos)
                {
                    // 出错了
                    
                    return sCode;
                }
                else
                {
                    m_setStrList.insert(FmtString(sCode.substr(i, ulNext - i + 1)));
                }
                i = ulNext;
            }
            else if ('#' == sCode[i])       // #include \ #define \ #if \ #pragma ...
            {
                if (memcmp(sCode.c_str() + i, "#include", 8) == 0)
                {
                    // 头文件
                    i += 9;
                    i += ParseHeader(sCode.c_str() + i, sCode.length() - i) - 1;
                }
                else if (memcmp(sCode.c_str() + i, "#define", 7) == 0)
                {
                    // 宏
                    i += 7;
                    continue;
                }
                else
                {
                    
                    size_t ulNext = FindLineTail(sCode, i + 1);
                    if (ulNext == string::npos)
                    {
                        break;
                    }
                    else
                    {
                        i = ulNext;
                    }
                }
            }
            else if (IsBlank(sCode[i]))
            {
                continue;
            }
            else 
            {
                char cPrefix = i > 0 ? sCode[i - 1] : ' ';
                i += ParseCodeSeg(sCode.c_str() + i, sCode.length() - i, cPrefix) - 1;                
            }
        }

        return sCode;
    }


    bool HaveOperator(const string &sOperator) const
    {
        set<string>::const_iterator Iter;
        for(Iter = m_setOpList.begin(); Iter != m_setOpList.end(); Iter++)
        {
            if (Iter->find(sOperator) != string::npos)
            {
                return true;
            }
        }
        return false;
    }

    bool HaveName(const string &sName) const
    {
        return m_setNameList.find(sName) != m_setNameList.end();
    }

    bool HaveInt(uint64_t ullVal) const
    {
        return m_setIntList.find(ullVal) != m_setIntList.end();
    }

    bool HaveDouble(double dVal) const
    {
        return m_setDoubleList.find(dVal) != m_setDoubleList.end();
    }

    bool HaveString(const string &sStr) const
    {
        return m_setStrList.find(sStr) != m_setStrList.end();
    }
    
    bool HaveHeader(const string &sStr) const
    {
        return m_setHeaderList.find(sStr) != m_setHeaderList.end();
    }
    
    bool HaveSubStr(const string &sSubStr) const
    {
        return m_sFilterSource.find(sSubStr) != string::npos;
    }

    string           m_sOrigSource;      // 原始代码
    string           m_sFilterSource;    // 过滤注释之后的代码

    set<string>      m_setStrList;
    set<uint64_t>    m_setIntList;
    set<double>      m_setDoubleList;
    set<string>      m_setNameList;
    set<string>      m_setOpList;
    set <string>     m_setHeaderList;
};


class CChecker
{
public:

    bool CheckSingleOpSimple(const string &sOperator, const string &sAnother, const CSourcePaser &Source, const CSourcePaser &Dest)
    {
        if (!Source.HaveOperator(sOperator))
        {
            return true;
        }
        if (Dest.HaveSubStr(sOperator) || (!sAnother.empty() && Dest.HaveSubStr(sAnother)))
        {
            return true;
        }
        return false;
    }


    bool CheckHeaderSimple(const CSourcePaser &Source, const CSourcePaser &Dest)
    {
        int iHaveTot = 0;
        set<string>::const_iterator Iter;
        for (Iter = Source.m_setHeaderList.begin(); Iter != Source.m_setHeaderList.end(); Iter++)
        {
            if (Iter->length() < 2)
            {
                ++iHaveTot;
                continue;
            }
            if (Dest.HaveSubStr(*Iter))
            {
                ++iHaveTot;
                continue;
            }
            string sName(*Iter);
            if (sName[sName.length() - 2] == '.')
            {
                sName = string("c") + sName.substr(0, sName.length() - 2);  // stdio.h  ->  cstdio
            }
            else if (sName[0] == 'c')
            {
                sName = sName.substr(1) + ".h"; // cstdio -> stdio.h
            }
            else
            {
                continue;
            }

            if (Dest.HaveSubStr(*Iter))
            {
                iHaveTot++;
            }
        }
        return iHaveTot + 1 >= (int)Source.m_setHeaderList.size() ;
    }



    bool CheckSimple(const CSourcePaser &Source, const CSourcePaser &Dest)
    {
        // 单字节操作符定义
        const char * const SINGLE_OP = "+-*/%[]<>.:?=";

        #define CHECK_OP(OP, ANOTHER)\
        do\
        {\
            if (!CheckSingleOpSimple(OP, ANOTHER, Source, Dest))\
            {\
                return false;\
            }\
        }while (0)
    
        for (int i = 0; SINGLE_OP[i]; ++i)
        {
            CHECK_OP(string(1, SINGLE_OP[i]), "");
        }

        CHECK_OP("&", "and");
        CHECK_OP("|", "or");
        CHECK_OP("!", "not");
        CHECK_OP("^", "xor");
        CHECK_OP("~", "compl");

        #undef CHECK_OP

        if (!CheckHeaderSimple(Source, Dest))
        {
            return false;
        }
        return true;
    }

    int CheckOp(const CSourcePaser &Source, const CSourcePaser &Dest)
    {
        int iOrigTot = 0;
        int iHaveTot = 0;

        static const char *const NAME_LIST[] = 
        {
            "++","--","->","<<",">>","==",">=","<=","!=","&&","||","/=","+=","*=","%=","-=",">>=","<<=","^=","|=","&=","::"
        };


        for (uint32_t i = 0; i < sizeof (NAME_LIST) / sizeof (NAME_LIST[0]); ++i)
        {
            if (Source.HaveOperator(NAME_LIST[i]))
            {
                iOrigTot++;
                if (Dest.HaveOperator(NAME_LIST[i]))
                {
                    iHaveTot++;
                }
            }
        }

        if (0 == iOrigTot)
        {
            return -1;
        }
        return iHaveTot * 100 / iOrigTot;
    }


    int CheckName(const CSourcePaser &Source, const CSourcePaser &Dest)
    {
        int iOrigTot = 0;
        int iHaveTot = 0;

        static const char *const NAME_LIST[] = 
        {
            "break", "operator", "private", "protected", "public", "new", "try", "void",
            "while", "return", "for", "continue", "namespace", "switch", "using", "virtual",
            "volatile", "case", "sizeof", "catch", "delete", "if", "goto", "friend", "else", "do", 
            "const", "extern", "static", "this", "throw", "typedef", "default", "dynamic_cast", 
            "reinterpret_cast", "const_cast", "static_cast", "template", "export", "mutable", 
            "typename", "typeid", "std", "vector", "map", "set", "iterator", "tuple", "pair", 
            "list", "memcpy", "memcmp", "memset", "malloc", "free", "bzero", "puts", "putchar",
            "fputs", "gets", "getchar", "fgets", "sprintf", "printf", "scanf", "sscanf", "fscanf",
            "cout", "endl", "cin", "cerr", "clog", "queue", "stack", "deque", "priority_queue", "string",
            "c_str", "substr", "istringstream", "ostringstream", "stringstream", "ifstream", "ofstream",
            "fstream", "strcpy", "strncpy", "strcat", "strncat", "strcmp", "strncmp", "strlen", "strchr",
            "strstr", "atoi", "itoa", "getline", "erase", "find", "insert", "count", "begin", "end", "empty",
            "size", "append", "clear", "swap", "push_back", "push", "pop", "top", "front", "back", "lower_bound", 
            "upper_bound", "struct", "class", "enum", "union", "assert", "stdin", "stdout",
            "floor", "ceil", "pow", "log", "sqrt", "abs", "rand"
        };

        for (uint32_t i = 0; i < sizeof (NAME_LIST) / sizeof (NAME_LIST[0]); ++i)
        {
            if (Source.HaveName(NAME_LIST[i]))
            {
                iOrigTot++;
                if (Dest.HaveName(NAME_LIST[i]))
                {
                    iHaveTot++;
                }
            }
        }

        if (0 == iOrigTot)
        {
            return -1;
        }
        return iHaveTot * 100 / iOrigTot;
    }


    int CheckString(const CSourcePaser &Source, const CSourcePaser &Dest)
    {
        int iHaveTot = 0;
        
        set<string>::const_iterator Iter;
        for (Iter = Source.m_setStrList.begin(); Iter != Source.m_setStrList.end(); Iter++)
        {
            if (Dest.HaveString(*Iter))
            {
                ++iHaveTot;
            }
        }

        if (Source.m_setStrList.empty())
        {
            return -1;
        }

        return iHaveTot * 100 / Source.m_setStrList.size();
    }


    int CheckInt(const CSourcePaser &Source, const CSourcePaser &Dest)
    {
        int iHaveTot = 0;
        
        set<uint64_t>::const_iterator Iter;
        for (Iter = Source.m_setIntList.begin(); Iter != Source.m_setIntList.end(); Iter++)
        {
            if (Dest.HaveInt(*Iter))
            {
                ++iHaveTot;
            }
        }

        if (Source.m_setIntList.empty())
        {
            return -1;
        }

        return iHaveTot * 100 / Source.m_setIntList.size();
    }

    int CheckDouble(const CSourcePaser &Source, const CSourcePaser &Dest)
    {
        int iHaveTot = 0;
        
        set<double>::const_iterator Iter;
        for (Iter = Source.m_setDoubleList.begin(); Iter != Source.m_setDoubleList.end(); Iter++)
        {
            if (Dest.HaveDouble(*Iter))
            {
                ++iHaveTot;
            }
        }

        if (Source.m_setDoubleList.empty())
        {
            return -1;
        }

        return iHaveTot * 100 / Source.m_setDoubleList.size();
    }

    int CheckHeader(const CSourcePaser &Source, const CSourcePaser &Dest)
    {
        int iHaveTot = 0;
        
        set<string>::const_iterator Iter;
        for (Iter = Source.m_setHeaderList.begin(); Iter != Source.m_setHeaderList.end(); Iter++)
        {
            if (Iter->length() < 2 || Dest.HaveHeader(*Iter))
            {
                ++iHaveTot;
                continue;
            }
            string sName(*Iter);
            if (sName[sName.length() - 2] == '.')
            {
                sName = string("c") + sName.substr(0, sName.length() - 2);  // stdio.h  ->  cstdio
            }
            else if (sName[0] == 'c')
            {
                sName = sName.substr(1) + ".h"; // cstdio -> stdio.h
            }
            else
            {
                continue;
            }

            if (Dest.HaveHeader(sName))
            {
                iHaveTot++;
            }
        }

        if (Source.m_setHeaderList.empty())
        {
            return -1;
        }

        return iHaveTot * 100 / Source.m_setHeaderList.size();
    }

    bool CheckScore(const vector<int> &vecScore)
    {
        int iHighScoreTot = 0;
        int iLowScoreTot = 0;
        int iNoneScoreTot = 0;

        for (uint32_t i = 0; i < vecScore.size(); ++i)
        {
            //printf("%d\t", vecScore[i]);
            if (vecScore[i] > 90)
            {
                iHighScoreTot++;
            }
            else if (-1 == vecScore[i])
            {
                iNoneScoreTot++;
            }
            else
            {
                iLowScoreTot++;
            }
        }
        //putchar('\n');
        
        if (iHighScoreTot >= 4 || iLowScoreTot == 0)
        {
            return true;
        }

        if (iHighScoreTot >= 3 && iLowScoreTot <= 2)
        {
            return true;
        }

        return false;
    }


    bool Check(const CSourcePaser &Source, const CSourcePaser &Dest)
    {
        if (!CheckSimple(Source, Dest))
        {
            return false;
        }

        vector<int> ScoreList;
        ScoreList.push_back(CheckHeader(Source, Dest));
        ScoreList.push_back(CheckString(Source, Dest));
        ScoreList.push_back(CheckInt(Source, Dest));
        ScoreList.push_back(CheckDouble(Source, Dest));
        ScoreList.push_back(CheckName(Source, Dest));
        ScoreList.push_back(CheckOp(Source, Dest));
        return CheckScore(ScoreList);
    }
    
    bool Check(const char *szLeftFile, const char *szRightFile)
    {
        string sSource = LoadSource(szLeftFile);
        m_Left.ParseSource(sSource);

        sSource = LoadSource(szRightFile);
        m_Right.ParseSource(sSource);

        return Check(m_Left, m_Right) || Check(m_Right, m_Left);
        
        return true;
    }


private:
    CSourcePaser        m_Left, m_Right;
};

// for debug.
void Output(const set<string> &A)
{
    set<string>::const_iterator Iter;
    for (Iter = A.begin(); Iter != A.end(); Iter++)
    {
        cout << *Iter << endl;
    }
}

void Output(const set<double> &A)
{
    set<double>::const_iterator Iter;
    for (Iter = A.begin(); Iter != A.end(); Iter++)
    {
        cout << *Iter << endl;
    }
}

void Output(const set<uint64_t> &A)
{
    set<uint64_t>::const_iterator Iter;
    for (Iter = A.begin(); Iter != A.end(); Iter++)
    {
        cout << *Iter << endl;
    }
}

void Output(const CSourcePaser &Paser)
{
    cout << " =============== m_setStrList: ==========" << endl;
    Output(Paser.m_setStrList);
    
    cout << " =============== m_setIntList: ==========" << endl;
    Output(Paser.m_setIntList);
    
    cout << " =============== m_setDoubleList: ==========" << endl;
    Output(Paser.m_setDoubleList);
    
    cout << " =============== m_setNameList: ==========" << endl;
    Output(Paser.m_setNameList);
    
    cout << " =============== m_setOpList: ==========" << endl;
    Output(Paser.m_setOpList);
    
    cout << " =============== m_setHeaderList: ==========" << endl;
    Output(Paser.m_setHeaderList);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, 
            "Usage:\n"
            "    %s <X>\n", argv[0]);
        return -1;
    }

    static CChecker Checker;

    // for debug.
    //puts(Checker.Check(argv[1], argv[2]) ? "Y" : "N");
    //return 0;
    
    FILE *pOutput = fopen("result.txt", "w");
    if (NULL == pOutput)
    {
        fprintf(stderr, "Can not create result.txt\n");
        return -1;
    }

    int X = atoi(argv[1]);

    char szSrc[1024];
    char szDst[1024];

    for (int i = 1; i <= X; ++i)
    {
        sprintf(szSrc, "./sample1/%d.cpp", i);
        sprintf(szDst, "./sample2/%d.cpp", i);

        fprintf(pOutput, Checker.Check(szSrc, szDst) ? "Y\n" : "N\n");
    }
    fclose(pOutput);
    
    return 0;
    
}




