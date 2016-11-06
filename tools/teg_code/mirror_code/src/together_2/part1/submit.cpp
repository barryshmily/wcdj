#include <string>
#include <string.h>
#include <errno.h>
#include <map>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdint.h>

using namespace std;

#define LOWER_CASE(ch) (('A' <= (ch) && (ch) <= 'Z') ? (ch) - 'A' + 'a' : (ch))


inline uint64_t BKDRHash(const char *szStr, uint32_t ulLen)
{
    uint64_t ulHash = 0;
    for (uint32_t i = 0; i < ulLen; ++i)
    {
        ulHash = ulHash * 131 + uint8_t(szStr[i]);
    }
    return ulHash;
}

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

string GenRandomName()  // 生存一个随机名字
{
    static const char * const szTable = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789";

    char szName[64];

    int iNameLen = rand() % 10 + 10;
    szName[0] = szTable[rand() % (26 + 26 + 1)]; // 首字母不允许为数字

    for (int i = 1; i < iNameLen; ++i)
    {
        szName[i] = szTable[rand() % (26 + 26 + 1 + 10)]; 
    }
    return string(szName, iNameLen);
    
}

size_t FindStrTail(const string &sStr, int iHeadPos)
{
    for (int j = iHeadPos + 1; j < (int)sStr.length(); ++j)
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
    return string::npos;
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
                iEnd = sStr.find('\n', i + 2);
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

size_t GetPrevChar(const string &sStr, int iPos)
{
    while (--iPos >= 0)
    {
        if (!IsBlank(sStr[iPos]))
        {
            return iPos;
        }
    }
    return string::npos;
}

size_t GetNextChar(const string &sStr, int iPos)
{
    while (++iPos < (int)sStr.length())
    {
        if (!IsBlank(sStr[iPos]))
        {
            return iPos;
        }
    }
    return string::npos;
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
            return sStr;
            ++p;

            if (isdigit(*p))      // 八进制表示
            {
                return sStr;    // 暂时不支持
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





class CUpset
{
public:

    CUpset()
    {
        m_bUpsetOK = false;
    }


    bool LoadFromFile(const char *szFile)
    {
        FILE *pFile = fopen(szFile, "rb");
        if (pFile == NULL)
        {
            fprintf(stderr, "Can not open file :%s.\n", szFile);
            return false;
        }
    
        m_bUpsetOK = false;
        m_sDest.clear();
        m_sSrouce.clear();
        m_MarcoMap.clear();
        
        char szBuffer[1024 * 8];

        int iRet;

        while ((iRet = fread(szBuffer, 1, sizeof szBuffer, pFile)) > 0)
        {
            m_sSrouce.append(szBuffer, iRet);
        }
        fclose(pFile);
        return true;
    }

    bool SaveToFile(const char *szFile)
    {
        FILE *pFile = fopen(szFile, "wb");
        if (pFile == NULL)
        {
            fprintf(stderr, "Can not open file :%s.\n", szFile);
            return false;
        }

        if (m_bUpsetOK)
        {
            fwrite(m_sDest.c_str(), m_sDest.length(), 1, pFile);
        }
        else
        {
            // 失败则写原始文件
            fwrite(m_sSrouce.c_str(), m_sSrouce.length(), 1, pFile);
        }
        fclose(pFile);
        return true;
    }

    string GenRubbish()
    {
        // 生存一段无意义代码
        char szBuffer[1024 * 8];
        string sFuncName = GenRandomName();
        string sRand = GenRandomName();
        
        sprintf(szBuffer,
            "#include <cstdio>\n"
            "#include <cstring>\n"
            "#include <cstdlib>\n"
            "#include <ctime>\n"
            "#include <cstdio>\n"
            "#include <cmath>\n"
            "using namespace std;\n"
            "int %s(int i, unsigned j, double k, short l, char n, float *p, long k1)\n"
            "{\n"
            "    /* fsdjkahfkjsda $$$\n fsd*/\n"
            "    static const char *szStr = \"%s\\\\\\\"  /*  \\\\\";\n"
            "    i++,++j;\n"
            "    i--,--j;\n"
            "    k1 += -(i) + (j) - j > (l) / ((n >> 1) << 1) + (i|j) + (i&j) + (i^j) + i %% *(unsigned*)(&j) - ~i + sizeof(k); \n"
            "    k1 += k1 > k && !(k1 == l || k1 != k); \n"
            "    k1 -= k1 ? k1 : j + *p - 0.1;\n"
            "    k1 *= k1; k1 += k1; k1 /= k1; k1 %%= k1; k1 <<= k1; k1 >>= k1; k1 &= k1; k1 ^= k1; k1 |= 123, i=456;\n"
            "    return k1 + szStr[0]-i;\n"
            "}\n"
            "#if 123456-1==99999\n"
            "int main(int argc, char *argv[])\n"
            "{\n{{{\nprintf(\"fdsafdsafdsa\n\n"
            "#endif\n"
            "#ifndef C0mB\n"
            "#define C0mB(X,Y) Y##X\n"
            "#endif\n"
            , sFuncName.c_str(), sRand.c_str());

        string sCode(szBuffer);
        for (uint32_t i = 0; i < sCode.length(); ++i)
        {
            if (sCode[i] == '$')
            {
                sCode[i] = '\0';
            }
        }

        return sCode;
        
    }

    void AddString(const string &sCode, size_t iPos, size_t iLen)
    {
        
        string sStr(FmtString(sCode.substr(iPos, iLen)));
        uint64_t ullHash = BKDRHash(sStr.c_str(), sStr.length());
        char szName[64];

        sprintf(szName, "M_%llx", (unsigned long long)ullHash);
        m_MarcoMap[szName] = sStr;

        m_sDest += szName;
        m_sDest.push_back('\n');
    }

    void AddBlock(string sBlock)
    {
        if (sBlock.length() < 0)
        {
            assert(false);
            return;
        }

        uint64_t ullHash = BKDRHash(sBlock.c_str(), sBlock.length());
        char szName[64];

        sprintf(szName, "M_%llx", (unsigned long long)ullHash);

        m_sDest += szName;
        m_sDest.push_back('\n');

        map<string, string>::iterator Iter = m_MarcoMap.find(szName);
        if (Iter != m_MarcoMap.end())
        {
            return;
        }
        
        uint32_t i;
        for (i = 0; i < sBlock.length(); ++i)
        {
            if (!isalnum(sBlock[i]) && sBlock[i] != '_')        // 只有名字才允许分隔
            {
                break;
            }
        }
        
        if (sBlock.length() == 1 || i < sBlock.length())
        {
            m_MarcoMap[szName] = sBlock;
        }
        else
        {
            assert(sBlock.length() > 1);
            int iMid = rand() % (sBlock.length() - 1) + 1;
            string &sVal = m_MarcoMap[szName];
            sVal = "C0mB(";
            sVal += sBlock.substr(iMid);
            sVal.push_back(',');
            sVal += sBlock.substr(0, iMid);
            sVal.push_back(')');
        }
    }

    int GetName(const char *szStr)
    {
        int i = 1;
        while (isalnum(szStr[i]) || szStr[i] == '_')
        {
            ++i;
        }
        AddBlock(string(szStr, i));
        return i;
    }

    int GetNum(const char *szStr)    // 添加十进制整数, 把十进制数换成十六进制, 然后返回使用长度, -1 表示失败
    {
        char *pEnd;
        uint64_t ullVal = strtoull(szStr, &pEnd, 10);

        if (*pEnd == '.' || LOWER_CASE(*pEnd) == 'e')
        {
            return -1;   // 忽略浮点数
        }

        string sSufix;
        while (LOWER_CASE(*pEnd) == 'u' || LOWER_CASE(*pEnd) == 'l')
        {
            sSufix.push_back(*pEnd++);
        }

        if (isalnum(*pEnd))
        {
            return -1;
        }

        char szBuffer[128];

        sprintf(szBuffer, "0x%lx%s", ullVal, sSufix.c_str());
        AddBlock(szBuffer);
        return pEnd - (char*)szStr;
    }

    int GetHex(const char *szStr)    // 添加十六进制整数, 反过来把十六进制转成十进制
    {

        char *pEnd;
        uint64_t ullVal = strtoull(szStr, &pEnd, 16);

        string sSufix;
        while (LOWER_CASE(*pEnd) == 'u' || LOWER_CASE(*pEnd) == 'l')
        {
            sSufix.push_back(*pEnd++);
        }

        if (isalnum(*pEnd))
        {
            return -1;
        }

        char szBuffer[128];

        sprintf(szBuffer, "%lu%s", ullVal, sSufix.c_str());
        AddBlock(szBuffer);
        return pEnd - (char*)szStr;
    }

    int GetOperator(const char *szStr)    // 添加各种运算符
    {
        int i = 1;
        while (szStr[i] && !isalnum(szStr[i]) && szStr[i] != '_')
        {
            if (szStr[i] == '.' && isdigit(szStr[i + 1]))
            {
                return -1;  // 浮点忽略
            }
            ++i;
        }
        AddBlock(string(szStr, i));
        return i;
    }

    bool CanUseMarco(const string &sCode, size_t iPos, size_t iLen = string::npos)
    {

        if (string::npos == iLen)
        {
            return true;
        }
        size_t ulRet = GetPrevChar(sCode, iPos);
        if (string::npos == ulRet || (sCode[ulRet] != ',' && sCode[ulRet] != '(' ))
        {
            return true;
        }

        ulRet = GetNextChar(sCode, iPos + iLen - 1);
        if (string::npos == ulRet || (sCode[ulRet] != ',' && sCode[ulRet] != ')' ))
        {
            return true;
        }

        for (uint32_t i = iPos; i < iPos + iLen; ++i)
        {
            if (sCode[i] != '_' && !isalnum(sCode[i]))
            {
                return true;
            }
        }
        return false;
    }


    void AddCode(const string &sCode, size_t iPos, size_t iLen = string::npos)
    {

        string sStr(sCode.substr(iPos, iLen));

        if (!CanUseMarco(sCode, iPos, iLen))
        {
            m_sDest += sStr;
            return;
        }

        int i;
        
        for (i = 0; i < (int)sStr.length();)
        {
            if (isalpha(sStr[i]) || '_' == sStr[i])
            {
                // 名字
                int iRet = GetName(sStr.c_str() + i);
                if (iRet > 0)
                {
                    i += iRet;
                    continue;
                }
                else
                {
                    AddBlock(sStr.substr(i));
                    break;
                }
            }
            else if('1' <= sStr[i] && sStr[i] <= '9')
            {
                // 数字, 写成16进制格式
                int iRet = GetNum(sStr.c_str() + i);
                if (iRet > 0)
                {
                    i += iRet;
                    continue;
                }
                else
                {
                    AddBlock(sStr.substr(i));
                    break;
                }
            }
            else if ('0' == sStr[i])
            {
                if (sStr[i + 1] == 'x' || sStr[i + 1] == 'X')   // 十六进制
                {
                    int iRet = GetHex(sStr.c_str() + i);
                    if (iRet > 0)
                    {
                        i += iRet;
                        continue;
                    }
                }
                AddBlock(sStr.substr(i));
                break;

            }
            else
            {
                if ('.'  == sStr[i] && isdigit(sStr[i+1]))  // 浮点数
                {
                    AddBlock(sStr.substr(i));
                    break;
                }
                int iRet = GetOperator(sStr.c_str() + i);
                if (iRet > 0)
                {
                    i += iRet;
                    continue;
                }
                else
                {
                    AddBlock(sStr.substr(i));
                    break;
                }
            }

            assert(false);
        }
        
    }

    bool AlreadyInclude(const string &sInc)
    {
        if (sInc.find("stdio.h") != string::npos)
        {
            return true;
        }
        if (sInc.find("string.h") != string::npos)
        {
            return true;
        }
        if (sInc.find("stdlib.h") != string::npos)
        {
            return true;
        }
        if (sInc.find("<time.h>") != string::npos)
        {
            return true;
        }
        if (sInc.find("<math.h>") != string::npos)
        {
            return true;
        }
        return false;
    }

    bool UpsetCode()
    {
/*        string sResult;

        string sCombineMarco = GenRandomName();

        sResult += "#define %s(a, b) a##b\n";
*/

        string sCode = FilterComment(m_sSrouce);

        int iFirstCharIdx = -1;

        for (uint32_t i = 0; i < sCode.length(); ++i)
        {
            if ('"' == sCode[i] ||  '\'' == sCode[i])
            {
                if (iFirstCharIdx >= 0)
                {
                    AddCode(sCode, iFirstCharIdx, i - iFirstCharIdx);
                    iFirstCharIdx = -1;
                }
                
                // 字符串
                size_t ulNext = FindStrTail(sCode, i);
                if (ulNext == string::npos)
                {
                    // 出错了
                    fprintf(stderr, "Cannot get the next \\\". near:%s", sCode.c_str() + i);
                    return false;
                }
                else
                {
                    AddString(sCode, i, ulNext - i + 1);
                }
                i = ulNext;
            }
            else if ('#' == sCode[i])       // #include \ #define \ #if \ #pragma ...
            {
                if (iFirstCharIdx >= 0)
                {
                    AddCode(sCode, iFirstCharIdx, i - iFirstCharIdx);
                    iFirstCharIdx = -1;
                }


                size_t ulNext = FindLineTail(sCode, i + 1);

                string sLine;
                if (ulNext == string::npos)
                {
                    sLine = sCode.substr(i);
                }
                else
                {
                    sLine = sCode.substr(i, ulNext - i + 1);
                }

                if (memcmp("#include", sCode.c_str() + i, 8) != 0 || !AlreadyInclude(sLine))
                {
                    m_sDest += sLine;
                }

                if (ulNext == string::npos)
                {
                    break;
                }
                else
                {
                    i = ulNext;
                }
            }
            else if ('(' == sCode[i] || ')' == sCode[i] || ',' == sCode[i])
            {
                if (iFirstCharIdx >= 0)
                {
                    AddCode(sCode, iFirstCharIdx, i - iFirstCharIdx);
                    iFirstCharIdx = -1;
                }
                m_sDest.push_back(sCode[i]);
            }
            else if (IsBlank(sCode[i]))
            {
                if (iFirstCharIdx >= 0)
                {
                    AddCode(sCode, iFirstCharIdx, i - iFirstCharIdx);
                    iFirstCharIdx = -1;
                }
                if ('\n' == sCode[i])
                {
                    m_sDest.push_back('\n');
                }
            }
            else
            {
                if (iFirstCharIdx < 0)
                {
                    iFirstCharIdx = i;
                }
            }
        }

        if (iFirstCharIdx >= 0)
        {
            AddCode(sCode, iFirstCharIdx);
            iFirstCharIdx = -1;
        }

        string sPrefix(GenRubbish());

        map<string, string>::iterator Iter;
        for (Iter = m_MarcoMap.begin(); Iter != m_MarcoMap.end(); Iter++)
        {
            sPrefix += "#ifndef ";
            sPrefix += Iter->first;
            sPrefix += "\n";

            if (rand() & 1)
            {
                sPrefix += "#if (12345 + 1) == 12345\n";
                sPrefix += "#define ";
                sPrefix += Iter->first;
                sPrefix.push_back(' ');
                sPrefix += GenRandomName();
                
                sPrefix += "\n#else\n";
                sPrefix += "#define ";
                sPrefix += Iter->first;
                sPrefix.push_back(' ');
                sPrefix += Iter->second;
                sPrefix += "\n\n#endif\n";
                    
            }
            else
            {
                sPrefix += "#if (12345 + 1) == 12346\n";
                sPrefix += "#define ";
                sPrefix += Iter->first;
                sPrefix.push_back(' ');
                sPrefix += Iter->second;
                sPrefix += "\n\n#else\n";
                sPrefix += "#define ";
                sPrefix += Iter->first;

                sPrefix.push_back(' ');
                sPrefix += GenRandomName();
                sPrefix += "\n#endif\n";
            }
            sPrefix += "#endif\n";
        }

        m_sDest = sPrefix + m_sDest;

        m_sDest += "\n/*  \"  */\n//";
        m_bUpsetOK = true;

        return true;
    }

private:
    bool        m_bUpsetOK;
    string      m_sSrouce;
    string      m_sDest;
    map<string, string> m_MarcoMap;
};

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, 
            "Usage:\n"
            "    %s <X>\n", argv[0]);
        return -1;
    }

    timeval stTime;
    gettimeofday(&stTime, NULL);
    srandom(stTime.tv_sec * (stTime.tv_usec + 1));

    static CUpset Obj;

    int X = atoi(argv[1]);

    char szSrc[1024];
    char szDst[1024];

    for (int i = 1; i <= X; ++i)
    {
        sprintf(szSrc, "./input/%d.cpp", i);
        sprintf(szDst, "./output/%d.cpp", i);
        assert(Obj.LoadFromFile(szSrc));
        assert(Obj.UpsetCode());
        assert(Obj.SaveToFile(szDst));
    }
    
    return 0;
}



