%{            
	// 全局变量声明部分
	// 词法规则部分
	// 函数定义部分
#include <stdio.h>  
#include <errno.h>

// 匹配模式的文本存储在这一变量中
extern char *yytext;
// FILE* 类型, 它指向lex正在解析的当前文件
extern FILE *yyin;
int sem_cnt  = 0;
%}

%%
[a-zA-Z][a-zA-Z0-9]*    { printf("WORD[%s] ", yytext); }
\"                      printf("QUOTE ");
\{                      printf("OBRACE ");
\}                      printf("EBRACE ");
;                       { ++sem_cnt; printf("SEMICOLON "); }
\n                      printf("\n");
[ \t]+                  /* ignore whitespace */;
%%

//以下为函数定义部分
int main(int argc, char *argv[]) 
{
    yyin = fopen(argv[1], "r");
    if (!yyin) {
		printf("fopen %s err[%d:%s]\n", 
				argv[1], errno, strerror(errno));
        return -1;
    }
	// 开始分析, 由lex自动生成
    yylex();
    fclose(yyin);

    return 0;
}

int yywrap() 
{ 
    printf("sem_cnt[%d]\n", sem_cnt);
	// 停止解析
    return 1; 
}

// refer : http://blog.csdn.net/ruglcc/article/details/7817619
// step 1:
// lex demo.lex
// step 2:
// gcc lex.yy.c -o analyse -ll
// step 3:
// ./analyse file.txt
