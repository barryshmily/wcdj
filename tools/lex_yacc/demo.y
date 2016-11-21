%{ 
#include <ctype.h> 
#include <stdio.h> 
#define YYSTYPE double /*double type for YACC stack; for yylval*/ 
 
 
void yyerror(const char *str) 
{ 
    fprintf(stderr, "error:%s\n", str ); 
} 
%} 
%token NUMBER 

%% 
lines   : lines expr '\n'           { printf("%g\n", $2); } 
        | lines '\n' 
        | /* e */ 
        | error '\n'                { yyerror("reenter last line:"); /*yyerrok(); */} 
        ; 
 
expr    : expr '+' term                {= $1 + $3; }  
        | expr '-' term                {= $1 - $3; } 
        | term 
        ; 
 
term    : term '*' factor            {= $1 * $3; }  
        | term '/' factor            {= $1 / $3; } 
        | factor 
        ; 
 
factor  : '(' expr ')'              {= $2; }  
        | '(' expr error            {= $2; yyerror("missing ')'"); /*yyerrok(); */} 
        | '-' factor                { $$ = -$2; } 
        | NUMBER 
        ; 

%% 
int main(void) 
{ 
    return yyparse(); 
} 
 
int yylex(void) 
{ 
    int c; 
    while ((c = getchar()) == ' '); 
    if (c == '.' || isdigit(c)) { 
        ungetc(c, stdin); 
        scanf("%lf", &yylval); 
        return NUMBER; 
    } 
    return c; 
}

// yacc demo.y
// gcc y.tab.c -o analyse -ll
