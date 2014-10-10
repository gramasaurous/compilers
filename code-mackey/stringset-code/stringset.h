// $Id: stringset.h,v 1.2 2014-10-03 18:47:35-07 - - $

#ifndef __STRINGSET__
#define __STRINGSET__

#include <string>
#include <unordered_set>
using namespace std;

#include <stdio.h>

const string* intern_stringset (const char*);

void dump_stringset (FILE*);

#endif

