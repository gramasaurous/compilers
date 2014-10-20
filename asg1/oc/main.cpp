// Graham Greving
// ggreving@ucsc.edu
// CMPS104a: asg1: main.cpp

// Use cpp to scan a file and print line numbers.
// Print out each input line read in, then strtok it for
// tokens.

#include <string>
using namespace std;

#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>
#include <unistd.h>

#include "auxlib.h"
#include "stringset.h"

string CPP = "/usr/bin/cpp";
const size_t LINESIZE = 1024;
int yydebug = 0;
int yy_flex_debug = 0;

// Chomp the last character from a buffer if it is delim.
void chomp (char* string, char delim) {
   size_t len = strlen (string);
   if (len == 0) return;
   char* nlpos = string + len - 1;
   if (*nlpos == delim) *nlpos = '\0';
}

// Run cpp against the lines of the file.
void cpplines (FILE* pipe, char* filename) {
   int linenr = 1;
   char inputname[LINESIZE];
   strcpy (inputname, filename);
   for (;;) {
      char buffer[LINESIZE];
      char* fgets_rc = fgets (buffer, LINESIZE, pipe);
      if (fgets_rc == NULL) break;
      chomp (buffer, '\n');
      //printf ("%s:line %d: [%s]\n", filename, linenr, buffer);
      // http://gcc.gnu.org/onlinedocs/cpp/Preprocessor-Output.html
      int sscanf_rc = sscanf (buffer, "# %d \"%[^\"]\"",
                              &linenr, filename);
      if (sscanf_rc == 2) {
         continue;
      }
      char* savepos = NULL;
      char* bufptr = buffer;
      for (int tokenct = 1;; ++tokenct) {
         char* token = strtok_r (bufptr, " \t\n", &savepos);
         bufptr = NULL;
         if (token == NULL) break;
         //printf ("token %d.%d: [%s]\n",linenr, tokenct, token);
         //const string* str = intern_stringset (token);
         intern_stringset(token);
      }
      ++linenr;
   }
}

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
            CPP += " -D";
            CPP += optarg;
            CPP += " ";
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
}


int main (int argc, char** argv) {
   set_execname (argv[0]);
   scan_options(argc, argv);

   DEBUGF('c', "argc: %d optind: %d\n", argc, optind);
   if (optind >= argc) {
      errprintf("%s %s %s %s", "Usage: ", "[-ly] [-@ flag ...]",
         " [-D string] filename.oc\n", get_execname());
   } else {
      char *filename = argv[optind];
      char *ext = strrchr(filename, '.');
      if (ext == NULL || strcmp(ext, ".oc") != 0) {
         errprintf("Error: bad file. oc requires oc files.\n");
         return (get_exitstatus());
      }
      string command = CPP + " " + filename;
      FILE* pipe = popen (command.c_str(), "r");
      if (pipe == NULL) {
         syserrprintf (command.c_str());
      } else {
         cpplines (pipe, filename);
         int pclose_rc = pclose (pipe);
         eprint_status (command.c_str(), pclose_rc);
         printf("CPP Exit: %d\n", pclose_rc);
      }
      // Strip the filebname to it's basename
      // Remove it's suffix and replace with .str
      string outfile = basename(filename);
      size_t i = outfile.find_last_of('.');
      outfile.erase(i+1, 2);
      outfile.append("str");
      FILE *out = fopen(outfile.c_str(), "w");
      dump_stringset(out);
      fclose(out);
   }
   return get_exitstatus();
}
