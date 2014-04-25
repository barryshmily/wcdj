/*  
 * 防止串尾汉字(GBK)出现乱码 
 * gerryyang 
 * 2012-07-14 
 */  
  
#include <stdio.h>  
#include <string.h>  
#include <string>  
using std::string;  
  
/* 
 * 功能：计算GBK中文串合理被截断的长度以防止串尾汉字出现乱码 
 * @para s: 含有GBK编码字符串的头指针 
 * @para iLeft: 函数外部能够使用的缓冲区大小 
 * @para ret: 函数返回字符串s合理被截取的长度 
 */  
  
int GbkSubString(const char *s, int iLeft)  
{  
    int len = 0, i = 0;  
  
    if( s == NULL || *s == 0 || iLeft <= 0 )   
        return(0);  
  
    while( *s )  
    {  
        if( (*s & 0x80) == 0 )  
        {  
            i ++;  
            s ++;  
            len ++;  
        }  
        else  
        {  
            if( *(s + 1) == 0 ) break;  
  
            i += 2;  
            s += 2;  
            len += 2;  
        }  
  
        if( i == iLeft ) break;  
        else if( i > iLeft )  
        {  
            len -= 2;  
            break;  
        }  
    }  
  
    return(len);  
}  
  
  
int main(int argc, char **argv)  
{  
    char szBuf[10] = {0};  
    string str = "123abc你好";  
  
    /*  
     * [1] 未处理中文截断的情况 
     */  
    snprintf(szBuf, sizeof(szBuf), "%s", str.c_str());  
    printf("szBuf: %s\n", szBuf);  
  
    /* 
     * [2] 处理中文可能被截断的情况 
     */  
    memset(szBuf, 0x0, sizeof(szBuf));  
    int iBufLeftLen = sizeof(szBuf)-1;  
  
    // 计算合法的长度为iGbkValidLen  
    int iGbkValidLen = GbkSubString(str.c_str(), iBufLeftLen);  
    puts("");  
    printf("str被合理截取的长度为: %d\n", iGbkValidLen);  
    snprintf(szBuf, iGbkValidLen + 1, "%s", str.c_str());  
    printf("szBuf: %s\n", szBuf);  
  
  
    return 0;  
}  
  
/*  
g++ -Wall -g -o code_test code_test.cpp 
 
output: 
szBuf: 123abc你? 
 
str被合理截取的长度为: 8 
szBuf: 123abc你 
*/  

