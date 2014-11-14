%{
// Dummy parser for scanner project.

#include "lyutils.h"
#include "astree.h"

%}

%debug
%defines
%error-verbose
%token-table
%verbose

%token TOK_VOID TOK_BOOL TOK_CHAR TOK_INT TOK_STRING
%token TOK_IF TOK_ELSE TOK_WHILE TOK_RETURN TOK_STRUCT
%token TOK_FALSE TOK_TRUE TOK_NULL TOK_NEW TOK_ARRAY
%token TOK_EQ TOK_NE TOK_LT TOK_LE TOK_GT TOK_GE
%token TOK_IDENT TOK_INTCON TOK_CHARCON TOK_STRINGCON

%token TOK_BLOCK TOK_CALL TOK_IFELSE TOK_INITDECL
%token TOK_POS TOK_NEG TOK_NEWARRAY TOK_TYPEID TOK_FIELD
%token TOK_ORD TOK_CHR TOK_ROOT

%token TOK_ROOT TOK_DECLID TOK_TYPEID TOK_FIELD TOK_INDEX
%token TOK_POS TOK_NEG TOK_CALL TOK_NEWARRAY TOK_NEWSTRING
%token TOK_IFELSE TOK_RETURNVOID TOK_BLOCK TOK_VARDECL
%token TOK_FUNCTION TO_PARAMLIST TOK_PROTOTYPE

%start start

%%

start       : program           { yyparse_astree = $1; }
            ;
program     : program structdef { $$ = adopt1 ($1, $2); }
            | program function  { $$ = adopt1 ($1, $2); }
            | program statement { $$ = adopt1 ($1, $2); }
            | program error '}' { $$ = $1; }
            | program error ';' { $$ = $1; }
            |                   { $$ = new_parseroot(); }
            ;
structdef   : TOK_STRUCT TOK_IDENT '{' fielddecls '}' {
               free_ast2($3, $5);
               change_sym($2, TOK_TYPEID);
               adopt2($1, $2, $4);
            }
            ;
fielddecls  : fielddecl             { $$ = $1; }
            | fielddecls fielddecl  { $$ = $1; }
            ;
fielddecl   : basetype TOK_IDENT ';' { 
               free_ast($3); 
               change_sym($2, TOK_FIELD);
               $$ = adopt1($1, $2);
            }
            | basetype TOK_ARRAY TOK_IDENT ';' {
               free_ast($4);
               change_sym($3, TOK_FIELD);
               $$ = adopt2($2, $1, $3);
            }
            ;
basetype    : TOK_VOID        { $$ = $1; }
            | TOK_BOOL        { $$ = $1; }
            | TOK_CHAR        { $$ = $1; }
            | TOK_INT         { $$ = $1; }
            | TOK_STRING      { $$ = $1; }
            | TOK_TYPEID      { $$ = $1; }
            ;
function    : identdecl '(' identdecls ')' block {
               /* do stuff */
            }
            ;
identdecls  : identdecl                { $$ = $1; }
            | identdecls identdecl     { $$ = $1; }
            ;
identdecl   : basetype TOK_IDENT ';' {
               free_ast($3);
               change_sym($2, TOK_DECLID);
               $$ = adopt1($1, $2);
            }
            | basetype TOK_ARRAY TOK_IDENT ';' {
               free_ast($4);
               change_sym($3, TOK_DECLID);
               $$ = adopt2($2, $2, $3);
            }
            |
            ;
block       : '{' statements '}'
            | ';'
            ;
statement   : block
            | vardecl
            | while
            | ifelse
            | return
            | expr ';'
            ;
vardecl     : identdecl '=' expr ';'
            ;
while       : TOK_WHILE '(' expr ')' statement
            ;
ifelse      : TOK_IF '(' expr ')' statement
            | TOK_IF '(' expr ')' statement TOK_ELSE statement
            ;
return      : TOK_RETURN ';'
            | TOK_RETURN expr ';'
            ;
expr        : expr BINOP expr
            | UNOP expr
            | allocator
            | call
            | '(' expr ')'
            | variable
            | constant
            ;
allocator   : TOK_NEW TOK_TYPEID '(' ')'
            | TOK_NEW TOK_STRING '(' expr ')'
            | TOK_NEW basetype '[' expr ']'
            ;
call        : TOK_IDENT '(' exprs ')'
            ;
exprs       : exprs ',' expr
            | expr
            ;
variable    : TOK_IDENT
            | expr '[' expr ']'
            | expr '.' TOK_FIELD
            ;
constant    : TOK_INTCON
            | TOK_CHARCON
            | TOK_STRINGCON
            | TOK_FALSE
            | TOK_TRUE
            | TOK_NULL
            ;


%%

const char *get_yytname (int symbol) {
   return yytname [YYTRANSLATE (symbol)];
}


bool is_defined_token (int symbol) {
   return YYTRANSLATE (symbol) > YYUNDEFTOK;
}


