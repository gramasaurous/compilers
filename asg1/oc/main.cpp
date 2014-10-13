// Graham Greving
// ggreving@ucsc.edu
// cmps104a: fall 2014
// asg1: stringset + preprocessor

// includes
// cpp libs
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <vector>
// local libs
#include "debug.h"
#include "util.h"
#include "stringset.h"

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
            debugflags::setflags (optarg);
            break;
         case 'D':
            // Pass 'string' to cpp
            cout << "D set" << endl;
            DEBUGF('o', "opt D set.");
            break;
         case 'l':
            // Debug yylex() yy_flex_debug = 1;
            DEBUGF('o', "opt l set");
            break;
         case 'y':
            // Debug yyparse() with yydebug = 1; 
            DEBUGF('o', "opt y set");
            break;
         default:
            complain() << "-" << (char) optopt << ": invalid option"
                       << endl;
            break;
      }
   }
}

int main (int argc, char **argv) {
   sys_info::execname (argv[0]);
   scan_options (argc, argv);
   vector<string> args (&argv[optind], &argv[argc]);
   if (args.size() < 1) {

   }
}