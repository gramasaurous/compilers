// Graham Greving
// ggreving@ucsc.edu
// CMPS104a: asg2: main.cpp

// Use cpp to scan a file and print line numbers.
// Generate and intern tokens with a flex scanner.

// Remove need for BS std::nonsense
using namespace std;

// Global C++/C Modules
#include <string>
#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>
#include <unistd.h>

// Local Modules
#include "auxlib.h"
#include "stringset.h"
#include "lyutils.h"

const string cpp_name = "/usr/bin/cpp";
string yyin_cpp_command;
const size_t LINESIZE = 1024;
int oc_include = 0;
extern int yydebug;
extern int yy_flex_debug;
FILE *tok_file;
FILE *ast_file;

// Open a pipe from the C preprocessor.
// Exit failure if can't.
// Assignes opened pipe to FILE* yyin.
void yyin_cpp_popen (const char* filename) {
   if (oc_include == 0) {
      yyin_cpp_command += cpp_name;
      yyin_cpp_command += " ";
   }
   yyin_cpp_command += filename;
   yyin = popen (yyin_cpp_command.c_str(), "r");
   if (yyin == NULL) {
      syserrprintf (yyin_cpp_command.c_str());
      exit (get_exitstatus());
   }
}
void yyin_cpp_pclose (void) {
   int pclose_rc = pclose (yyin);
   eprint_status (yyin_cpp_command.c_str(), pclose_rc);
   if (pclose_rc != 0) set_exitstatus (EXIT_FAILURE);
}

//
// Scan the options, -D -y -l -@ and check for operands.
//
void scan_options (int argc, char** argv) {
   opterr = 0;
   yy_flex_debug = 0;
   for (;;) {
      int option = getopt (argc, argv, "D:yl@:");
      if (option == EOF) break;
      switch (option) {
         case '@':
            set_debugflags(optarg);
            break;
         case 'D':
            // Pass 'string' to cpp
            yyin_cpp_command = cpp_name;
            yyin_cpp_command += " -D";
            yyin_cpp_command += optarg;
            yyin_cpp_command += " ";
            oc_include = 1;
            DEBUGF('o', "Opt D set with flag: %c", optarg);
            break;
         case 'l':
            // Debug yylex() yy_flex_debug = 1;
            yy_flex_debug = 1;
            DEBUGF('o', "opt l set");
            break;
         case 'y':
            // Debug yyparse() with yydebug = 1; 
            yydebug = 1;
            DEBUGF('o', "opt y set");
            break;
         default:
            errprintf("%c: %s", (char) optopt, "invalid option.\n");
            break;
      }
   }
   if (optind >= argc) {
      errprintf ("Usage: %s [-ly] [filename]\n", get_execname());
      exit (get_exitstatus());
   }
}

void write_str(string base) {  
   // Strip the filename to it's basename
   // Remove it's suffix and replace with .str
   base.append("str");
   FILE *out = fopen(base.c_str(), "w");
   if (out == NULL) {
      syserrprintf (base.c_str());
      exit (get_exitstatus());
   }
   dump_stringset(out);
   fclose(out);
}

void open_tok_file(string base) {
   base.append("tok");
   tok_file = fopen(base.c_str(), "w");
   if (tok_file == NULL) {
      syserrprintf (base.c_str());
      exit (get_exitstatus());
   }
}

string get_filebase(char *filename) {
   string base = basename(filename);
   size_t i = base.find_last_of('.');
   // remove oc suffix
   base.erase(i+1, 2);
   return base;
}

void open_ast_file(string base) {
   base.append("ast");
   ast_file = fopen(base.c_str(), "w");
   if (ast_file == NULL) {
      syserrprintf (base.c_str());
      exit(get_exitstatus());
   }
}
 
int main (int argc, char** argv) {
   set_execname (argv[0]);
   scan_options(argc, argv);
   DEBUGF('c', "argc: %d optind: %d\n", argc, optind);
   char *filename = argv[optind];
   char *ext = strrchr(filename, '.');
   if (ext == NULL || strcmp(ext, ".oc") != 0) {
      errprintf("Error: bad file. oc requires oc files.\n");
      return (get_exitstatus());
   }
   string filebase = get_filebase(filename);
   open_tok_file(filebase);
   yyin_cpp_popen(filename);
   //int token;
   //while ((token = yylex()) != YYEOF) {
   //   if (yy_flex_debug) fflush(NULL);
   //   intern_stringset(yytext);
   //}
   yyparse();
   yyin_cpp_pclose();
   fclose(tok_file);
   fclose(ast_file);
   write_str(filename);
   return get_exitstatus();
}
