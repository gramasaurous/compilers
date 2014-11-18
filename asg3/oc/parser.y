%{
// Graham Greving
// ggreving@ucsc.edu
// CMPS104A: parser.y

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

%token TOK_DECLID TOK_INDEX TOK_NEWSTRING TOK_RETURNVOID
%token TOK_VARDECL TOK_FUNCTION TOK_PARAMLIST TOK_PROTOTYPE

%right TOK_IF TOK_ELSE
%right '='
%left  TOK_EQ TOK_NE TOK_GT TOK_LE TOK_LT TOK_GE
%left  '+' '-'
%left  '*' '/' '%'
%right TOK_NEG TOK_POS '!' TOK_ORD TOK_CHR
%left  TOK_ARRAY TOK_FIELD TOK_FUNCTION 
%left  '[' '.'

%nonassoc TOK_NEW
%nonassoc '('

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
               $2 = change_sym($2, TOK_TYPEID);
               $1 = kidnap_children($1, $4);
               $$ = adopt1($1, $2);
            }
            | TOK_STRUCT TOK_IDENT '{' '}' { 
               free_ast2($3, $4); 
               $2 = change_sym($2, TOK_TYPEID); 
               $$ = adopt1($1, $2);
            }
            ;

fielddecls  : fielddecl             { $$ = $1; }
            | fielddecls fielddecl { 
               //free_ast($2);
               $$ = adopt1($1, $2);
            }
            ;

fielddecl   : basetype TOK_IDENT ';' { 
               free_ast($3); 
               $2 = change_sym($2, TOK_FIELD);
               $$ = adopt1($1, $2);
            }
            | basetype TOK_ARRAY TOK_IDENT ';' {
               free_ast($4);
               $3 = change_sym($3, TOK_FIELD);
               $$ = adopt2($2, $1, $3);
            }
            ;

basetype    : TOK_VOID        { $$ = $1; }
            | TOK_BOOL        { $$ = $1; }
            | TOK_CHAR        { $$ = $1; }
            | TOK_INT         { $$ = $1; }
            | TOK_STRING      { $$ = $1; }
            | TOK_IDENT       { $$ = change_sym($1, TOK_TYPEID); }
            ;

function    : identdecl '(' identdecls ')' block {
               free_ast($4);
               $2 = change_sym($2, TOK_PARAMLIST);
               //printf("kidnapping the children\n");
               $2 = kidnap_children($2, $3);
               //$2 = adopt1sym($2, $3, TOK_PARAMLIST);
               $$ = adopt3(new_astree(TOK_FUNCTION, $1->filenr,
                     $1->linenr, $1->offset, ""), $1, $2, $5);
            }
            | identdecl '(' identdecls ')' ';' {
               free_ast($4); free_ast($5);
               $2 = change_sym($2, TOK_PARAMLIST);
               $2 = kidnap_children($2, $3);
               //$2 = adopt1sym($2, $3, TOK_PARAMLIST);
               $$ = adopt2(new_astree(TOK_PROTOTYPE, $1->filenr,
                     $1->linenr, $1->offset, ""), $1, $2);   
            }
            | identdecl '(' ')' block { 
               free_ast($3);
               $$ = adopt3(new_astree(TOK_PROTOTYPE, $1->filenr,
                     $1->linenr, $1->offset, ""), $1, $2, $4);   
            }
            | identdecl '(' ')' ';' {
               free_ast($3); free_ast($4);
               $$ = adopt2(new_astree(TOK_PROTOTYPE, $1->filenr,
                     $1->linenr, $1->offset, ""), $1, $2);   
                  
            }
            ;

identdecls  : identdecl                { $$ = $1; }
            | identdecls ',' identdecl {
               free_ast($2);
               $$ = adopt1($1, $3);;
            }
            ;

identdecl   : basetype TOK_IDENT {
               //free_ast($3);
               $2 = change_sym($2, TOK_DECLID);
               $$ = adopt1($1, $2);
            }
            | basetype TOK_ARRAY TOK_IDENT {
               //free_ast($4);
               $3 = change_sym($3, TOK_DECLID);
               $$ = adopt2($2, $1, $3);
            }
            ;

block       : '{' statements '}' {
               $1 = change_sym($1, TOK_BLOCK);
               free_ast($3);
               $$ = kidnap_children($1, $2);
               //$$ = adopt1($1, $2);
            }
            | '{' '}' { 
               free_ast($2);
               $$ = change_sym($1, TOK_BLOCK);
            } 
            ;

statements  : statement             { $$ = $1; }
            | statements statement  { 
               $$ = adopt1($1, $2);
            }
            ;

statement   : block     { $$ = $1; }
            | vardecl   { $$ = $1; }
            | while     { $$ = $1; }
            | ifelse    { $$ = $1; }
            | return    { $$ = $1; }
            | expr ';'  {
               free_ast($2);
               $$ = $1;
            }
            | ';'       { $$ = $1; }
            ;

vardecl     : identdecl '=' expr ';' {
               free_ast($4);
               $$ = adopt2sym($2,$1,$3,TOK_VARDECL);
            }
            ;

while       : TOK_WHILE '(' expr ')' statement {
               free_ast($2);
               free_ast($4);
               $$ = adopt2($1, $3, $5);
            }
            ;

ifelse      : TOK_IF '(' expr ')' statement TOK_ELSE statement {
               free_ast($2);
               free_ast($4);
               free_ast($6);
               $1 = change_sym($1, TOK_IFELSE);
               $$ = adopt3($1, $3, $5, $7);
            }
            | TOK_IF '(' expr ')' statement %prec TOK_IF {
               free_ast($2);
               free_ast($4);
               $$ = adopt2($1, $3, $5);
            }
            ;

return      : TOK_RETURN ';' {
               free_ast($2);
               $$ = change_sym($1, TOK_RETURNVOID);
            }
            | TOK_RETURN expr ';'{
               free_ast($3);
               $$ = adopt1($1, $2);
            }
            ;

exprs       : exprs ',' expr { free_ast($2); $$ = adopt1($1, $3); }
            | expr { $$ = $1; }
            ;

expr        : binop        { $$ = $1; }
            | unop         { $$ = $1; }
            | allocator    { $$ = $1; }
            | call         { $$ = $1; }
            | '(' expr ')' {
               free_ast($1); free_ast($3);
               $$ = $2;
            }
            | variable {$$ = $1; }
            | constant {$$ = $1;}
            ;

binop       : expr '+' expr      { $$ = adopt2($2, $1, $3); }
            | expr '-' expr      { $$ = adopt2($2, $1, $3); }
            | expr '*' expr      { $$ = adopt2($2, $1, $3); }
            | expr '/' expr      { $$ = adopt2($2, $1, $3); }
            | expr TOK_LT expr   { $$ = adopt2($2, $1, $3); }
            | expr TOK_GT expr   { $$ = adopt2($2, $1, $3); }
            | expr TOK_LE expr   { $$ = adopt2($2, $1, $3); }
            | expr TOK_GE expr   { $$ = adopt2($2, $1, $3); }
            | expr '=' expr      { $$ = adopt2($2, $1, $3); }
            | expr TOK_EQ expr   { $$ = adopt2($2, $1, $3); }
            | expr TOK_NE expr   { $$ = adopt2($2, $1, $3); }
            | expr '%' expr      { $$ = adopt2($2, $1, $3); }
            ;

unop        : '+' expr { $$ = adopt1sym($1, $2, TOK_POS); }
            | '-' expr { $$ = adopt1sym($1, $2, TOK_NEG); }
            | '!' expr { $$ = adopt1($1, $2); }
            | TOK_CHR expr { $$ = adopt1($1, $2); }
            | TOK_ORD expr { $$ = adopt1($1, $2); }
            ;

allocator   : TOK_NEW TOK_IDENT '(' ')' { 
               free_ast($3);
               free_ast($4);
               $2 = change_sym($2, TOK_TYPEID);
               $$ = adopt1($1, $2);
            }
            | TOK_NEW TOK_STRING '(' expr ')' {
               free_ast($3);
               free_ast($5);
               $$ = adopt1sym($1, $4, TOK_NEWSTRING);
            }
            | TOK_NEW basetype '[' expr ']' {
               free_ast($3); free_ast($5);
               $$ = adopt2sym($1, $2, $4, TOK_NEWARRAY);
            }
            ;

call        : TOK_IDENT '(' exprs ')' {
               free_ast($4);
               $2 = kidnap_children($2, $3);
               $$ = adopt1sym($2, $1, TOK_CALL);
            }
            | TOK_IDENT '(' ')' {
               free_ast($3);
               $2 = change_sym($2, TOK_CALL);
               $$ = adopt1($2, $1);
            }
            ;

variable    : TOK_IDENT         { $$ = $1; }
            | expr '[' expr ']' { 
               free_ast($4);
               $2 = change_sym($2, TOK_INDEX);
               $$ = adopt2($2, $1, $3);
            }
            | expr '.' TOK_IDENT {
               $3 = change_sym($3, TOK_FIELD); 
               $$ = adopt2($2, $1, $3);
            }
            ;

constant    : TOK_INTCON      { $$ = $1; }
            | TOK_CHARCON     { $$ = $1; }
            | TOK_STRINGCON   { $$ = $1; }
            | TOK_FALSE       { $$ = $1; }
            | TOK_TRUE        { $$ = $1; }
            | TOK_NULL        { $$ = $1; }
            ;

%%

const char *get_yytname (int symbol) {
   return yytname [YYTRANSLATE (symbol)];
}


bool is_defined_token (int symbol) {
   return YYTRANSLATE (symbol) > YYUNDEFTOK;
}
