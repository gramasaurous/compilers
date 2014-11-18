// Graham Greving
// ggreving@ucsc.edu
// CMPS104a: asg3: stringset.h

#ifndef __STRINGSET__
#define __STRINGSET__

#include <string>
#include <unordered_set>
using namespace std;

#include <stdio.h>

const string* intern_stringset (const char*);

void dump_stringset (FILE*);

#endif

