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
extern int yydebug = 0;
extern int yy_flex_debug = 0;

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

// Chomp the last character from a buffer if it is delim.
void chomp (char* string, char delim) {
   size_t len = strlen (string);
   if (len == 0) return;
   char* nlpos = string + len - 1;
   if (*nlpos == delim) *nlpos = '\0';
}

// Run cpp against the lines of the file.
//void cpplines (FILE* pipe, char* filename) {
   // ASG1 STUFF
   // int linenr = 1;
   // char inputname[LINESIZE];
   // strcpy (inputname, filename);
   // for (;;) {
   //    char buffer[LINESIZE];
   //    char* fgets_rc = fgets (buffer, LINESIZE, pipe);
   //    if (fgets_rc == NULL) break;
   //    chomp (buffer, '\n');
   //    //printf ("%s:line %d: [%s]\n", filename, linenr, buffer);
   //    // http://gcc.gnu.org/onlinedocs/cpp/Preprocessor-Output.html
   //    int sscanf_rc = sscanf (buffer, "# %d \"%[^\"]\"",
   //                            &linenr, filename);
   //    if (sscanf_rc == 2) {
   //       continue;
   //    }
   //    char* savepos = NULL;
   //    char* bufptr = buffer;
   //    for (int tokenct = 1;; ++tokenct) {
   //       char* token = strtok_r (bufptr, " \t\n", &savepos);
   //       bufptr = NULL;
   //       if (token == NULL) break;
   //       //printf ("token %d.%d: [%s]\n",linenr, tokenct, token);
   //       //const string* str = intern_stringset (token);
   //       intern_stringset(token);
   //    }
   //    ++linenr;
   // }
//}

//
// Scan the options, -D -y -l -@ and check for operands.
//
void scan_options (int argc, char** argv) {
   opterr = 0;
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

void write_str(char *filename) {   
   // Strip the filename to it's basename
   // Remove it's suffix and replace with .str
   string outfile = basename(filename);
   size_t i = outfile.find_last_of('.');
   outfile.erase(i+1, 2);
   outfile.append("str");
   FILE *out = fopen(outfile.c_str(), "w");
   dump_stringset(out);
   fclose(out);
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
   yyin_cpp_pclose();
   write_str(filename);
   return get_exitstatus();
}
