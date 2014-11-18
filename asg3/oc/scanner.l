%{
// Graham Greving
// ggreving@ucsc.edu
// CMPS104a: asg3: scanner.l

#include "auxlib.h"
#include "lyutils.h"

#define YY_USER_ACTION  { scanner_useraction (); }
#define IGNORE(THING)   { }

%}

%option 8bit
%option debug
%option ecs
%option nodefault
%option nounput
%option noyywrap
%option perf-report
%option verbose
%option warn

LETTER      [A-Za-z_]
DIGIT       [0-9]
NUMBER      ({DIGIT}+)
STRINGCON   (\"([^\\"\n]|\\[\\'"0nt])*\")
CHARCON     ('([^\\'\n]|\\[\\'"0nt])')

IDENT       ({LETTER}({LETTER}|{DIGIT})*)

%%

"#".*       { scanner_include(); }
"//".*      { IGNORE (comments) }
[ \t]+      { IGNORE (white space) }
\n          { scanner_newline(); }

"[]" { return yylval_token (TOK_ARRAY); }
"("  { return yylval_token ('('); }
")"  { return yylval_token (')'); }
"["  { return yylval_token ('['); }
"]"  { return yylval_token (']'); }
"{"  { return yylval_token ('{'); }
"}"  { return yylval_token ('}'); }
";"  { return yylval_token (';'); }
","  { return yylval_token (','); }
"."  { return yylval_token ('.'); }
"="  { return yylval_token ('='); }
"==" { return yylval_token (TOK_EQ); }
"!=" { return yylval_token (TOK_NE); }
"<"  { return yylval_token (TOK_LT); }
"<=" { return yylval_token (TOK_LE); }
">"  { return yylval_token (TOK_GT); }
">=" { return yylval_token (TOK_GE); }
"+"  { return yylval_token ('+'); }
"-"  { return yylval_token ('-'); }
"*"  { return yylval_token ('*'); }
"/"  { return yylval_token ('/'); }
"%"  { return yylval_token ('%'); }
"!"  { return yylval_token ('!'); }

"void"      { return yylval_token(TOK_VOID); }
"bool"      { return yylval_token(TOK_BOOL); }
"char"      { return yylval_token(TOK_CHAR); }
"int"       { return yylval_token(TOK_INT); }
"string"    { return yylval_token(TOK_STRING); }
"struct"    { return yylval_token(TOK_STRUCT); }
"if"        { return yylval_token(TOK_IF); }
"else"      { return yylval_token(TOK_ELSE); }
"while"     { return yylval_token(TOK_WHILE); }
"return"    { return yylval_token(TOK_RETURN); }
"new"       { return yylval_token(TOK_NEW); }
"false"     { return yylval_token(TOK_FALSE); }
"true"      { return yylval_token(TOK_TRUE); }
"null"      { return yylval_token(TOK_NULL); }
"ord"       { return yylval_token(TOK_ORD); }
"chr"       { return yylval_token(TOK_CHR); }

{CHARCON}   { return yylval_token (TOK_CHARCON); }
{STRINGCON} { return yylval_token (TOK_STRINGCON); }
{IDENT}     { return yylval_token (TOK_IDENT); }
{NUMBER}    { return yylval_token (TOK_INTCON); }

.    { scanner_badtoken (yytext); }

%%

