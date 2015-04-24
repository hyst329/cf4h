%{
#include "../F4Helen/AST.h"
using namespace F4Helen;
#define YYSTYPE AST*

#include <cstdlib>
#include <string>
#include "cf4.parser.hh"

using namespace std;

void yyerror(AST* result, const char *s)
{
    printf("Error in line %d: %s\n", yylineno, s);
};

%}

%option yylineno
%%
if                       return IF;
else                     return ELSE;
endif                    return ENDIF;
fun                      return FUN;
endfun                   return ENDFUN;
declare                  return DECLARE;
loop                     return LOOP;
endloop                  return ENDLOOP;
return                   return RETURN;
use                      return USE;
resize                   return RESIZE;
size                     return SIZE;
int                      return INT;
real                     return REAL;
logical                  return LOGICAL;
char                     return CHAR;
string                   return STRING;
in                       return IN;
out                      return OUT;
debugvar                 return DEBUGVAR;

[ \t]                    ;
[a-zA-Z][a-zA-Z0-9]*     yylval = new AST("ID", yytext); return IDENT;
\n                       return NEWLINE; yylineno++;
[0-9]+[\.\,][0-9]+       yylval = new AST("REAL", yytext); return REALLIT;
[0-9]+                   yylval = new AST("INT", yytext); return INTLIT;
'[^(')]'                 yylval = new AST("CHAR", string() + yytext[0]); return CHARLIT;
\"[^(\")]+\"             yylval = new AST("STR", yytext); return STRLIT;
==                       return EQUALS;
\<=                      return LEQ;
\>=                      return GEQ;
\<                       return LESS;
\>                       return GREATER;
\+=                      return APLUS;
\-=                      return AMINUS;
\-\>                     return LARROW;
\<\-                     return RARROW;
\+                       return PLUS;
\-                       return MINUS;
\*                       return MULT;
\/                       return DIVIDE;
=                        return ASSIGN;
\(                       return LPAREN;
\)                       return RPAREN;
\:                       return COLON;
\;                       return SEMI;
,                        return COMMA;
\.                       return POINT;
.                        yyerror(nullptr, yytext);
<<EOF>>                  static int once = 0; return once++ ? 0 : NEWLINE;
%%
