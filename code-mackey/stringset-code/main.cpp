// $Id: main.cpp,v 1.2 2014-10-09 15:09:33-07 - - $

#include <string>
using namespace std;

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stringset.h"

int main (int argc, char **argv) {
   for (int i = 1; i < argc; ++i) {
      const string* str = intern_stringset (argv[i]);
      printf ("intern (\"%s\") returned %p->\"%s\"\n",
              argv[i], str, str->c_str());
   }
   dump_stringset (stdout);
   return EXIT_SUCCESS;
}

