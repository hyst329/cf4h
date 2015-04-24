%{
#define YYSTYPE AST*
#include <vector>
#include <string>
#include <stdio.h>
#include "../F4Helen/AST.h"

using namespace F4Helen;

int yylex(void);
void yyerror(AST*, const char*);

%}

%token IF ELSE ENDIF
%token LOOP ENDLOOP
%token FUN ENDFUN DECLARE
%token SIZE RESIZE
%token RETURN
%token IN OUT DEBUGVAR
%token USE
%token INT REAL LOGICAL CHAR STRING
%token INTLIT REALLIT CHARLIT STRLIT
%token IDENT
%token NEWLINE
%token EQUALS LEQ GEQ LESS GREATER
%token PLUS MINUS MULT DIVIDE APLUS AMINUS
%token ASSIGN LARROW RARROW
%token LPAREN RPAREN
%token COLON SEMI COMMA POINT
%start program

%parse-param {AST *&result}
%%

program: instseq {
    $1->type = "ROOT";
    result = $1;
    $$ = $1;
}
instseq: instseq instruction {
    $$ = $1;
    $$->children.push_back($2);
}
| instruction {
    $$ = new AST("SEQ");
    $$->children.push_back($1);
}
instruction: NEWLINE {
    $$ = nullptr;
}
| statement NEWLINE {
    $$ = $1;
}
| IF expression NEWLINE instseq ENDIF {
    $$ = new AST("IF", 3, $2, $4, nullptr);
}
| IF expression NEWLINE instseq ELSE NEWLINE instseq ENDIF {
    $$ = new AST("IF", 3, $2, $4, $7);
}
| LOOP statement COMMA expression COMMA statement NEWLINE instseq ENDLOOP {
    $$ = new AST("LOOP", 4, $2, $4, $6, $8);
}
| FUN funprot NEWLINE instseq ENDFUN {
    $$ = new AST("FUN", 4, $2->children[0], $2->children[1], $2->children[2], $4);
}
| DECLARE funprot {
    $$ = new AST("DECL", 4, $2->children[0], $2->children[1], $2->children[2], nullptr);
}
| RETURN expression {
    $$ = new AST("RET", 1, $2);
}
| RESIZE IDENT LPAREN expression RPAREN {
    $$ = new AST("RSZ", 2, $2, $4);
}
funprot: IDENT COLON arglist RARROW type {
    $$ = new AST("PROT", 3, $1, $3, $5);
}
| IDENT COLON arglist {
    $$ = new AST("PROT", 3, $1, $3, nullptr);
}
| IDENT RARROW type {
    $$ = new AST("PROT", 3, $1, nullptr, $3);
}
| IDENT {
    $$ = new AST("PROT", 3, $1, nullptr, nullptr);
}
statement: declaration {
    $$ = $1;
}
| IN IDENT {
    $$ = new AST("IN", 2, $2, nullptr);
}
| IN IDENT POINT factor {
    $$ = new AST("IN", 2, $2, new AST("IND", 2, $2, $4));
}
| OUT expression {
    $$ = new AST("OUT", 1, $1);
}
| assignment {
    $$ = $1;
}
| DEBUGVAR {
    $$ = new AST("DEBUGVAR");
}
| expression {
    $$ = new AST("EXPR", 1, $1);
}
arglist: etype IDENT COMMA arglist {
    $$ = $4;
    $$->children.insert($$->children.begin(), new AST($1->type, $2->value));
}
| etype IDENT {
    $$ = new AST("ARGL", 1, new AST($1->type, $2->value));
}
etype: type {
    $$ = new AST("TYPE", $1->value);
}
| type POINT INTLIT {
    $$ = new AST("TYPE", $1->value + "." + $3->value);
}
exprlist: expression COMMA exprlist {
    $$ = $3;
    $$->children.insert($$->children.begin(), $1);
}
| expression {
    $$ = new AST("EXPL", 1, $1);
}
type: INT {
    $$ = new AST("TYPE", "INT");
}
| REAL {
    $$ = new AST("TYPE", "REAL");
}
| CHAR {
    $$ = new AST("TYPE", "CHAR");
}
| STRING {
    $$ = new AST("TYPE", "STR");
}
expression: expression PLUS term {
    $$ = new AST("ADD", 2, $1, $3);
}
| expression MINUS term {
    $$ = new AST("SUB", 2, $1, $3);
}
| factor EQUALS factor {
    $$ = new AST("EQL", 2, $1, $3);
}
| factor LEQ factor {
    $$ = new AST("LEQ", 2, $1, $3);
}
| factor GEQ factor {
    $$ = new AST("GEQ", 2, $1, $3);
}
| factor LESS factor {
    $$ = new AST("LES", 2, $1, $3);
}
| factor GREATER factor {
    $$ = new AST("GTR", 2, $1, $3);
}
| term {
    $$ = $1;
}
term: term MULT factor {
    $$ = new AST("MUL", 2, $1, $3);
}
| term DIVIDE factor {
    $$ = new AST("DIV", 2, $1, $3);
}
| factor {
    $$ = $1;
}
| MINUS factor {
    $$ = new AST("NEG", 1, $2);
}
factor: INTLIT | REALLIT | CHARLIT | STRLIT {
    $$ = $1;
}
| IDENT {
    $$ = $1;
}
| LPAREN expression RPAREN {
    $$ = $2;
}
| IDENT POINT factor {
    $$ = new AST("IND", 2, $1, $3);
}
| IDENT LPAREN exprlist RPAREN {
    $$ = new AST("CALL", 2, $1, $3);
}
| IDENT LPAREN RPAREN {
    $$ = new AST("CALL", 2, $1, nullptr);
}
| SIZE IDENT {
    $$ = new AST("SIZE", 1, $2);
}
declaration: type IDENT {
    $$ = new AST("NEW", 3, $1, $2, nullptr);
}
| type IDENT ASSIGN expression {
    $$ = new AST("NEW", 3, $1, $2, $4);
}
| type IDENT POINT factor {
    $$ = new AST("NEWARR", 3, $1, $2, $4);
}
assignment: IDENT ASSIGN expression {
    $$ = new AST("MOV", 3, $1, $3, nullptr);
}
| IDENT POINT factor ASSIGN expression {
    $$ = new AST("MOV", 3, $1, $5, $3);
}

%%
