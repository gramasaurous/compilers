// Graham Greving
// ggreving@ucsc.edu
// CMPS104a: asg4: typecheck.h

#ifndef __TYPECHECK_H__
#define __TYPECHECK_H__

#include <string>
#include <iostream>
#include <unordered_map>
#include <bitset>
#include <vector>

#include "auxlib.h"

using namespace std;

// enum defining attribute values
enum {   ATTR_void, ATTR_bool, ATTR_char, ATTR_int, ATTR_null,
         ATTR_string, ATTR_struct, ATTR_array, ATTR_function,
         ATTR_variable, ATTR_field, ATTR_typeid, ATTR_param, ATTR_lval,
         ATTR_const, ATTR_vreg, ATTR_vaddr, ATTR_bitset_size
};

// forward declaration to resolve circular dependencies
struct symbol;

// attributes, symbol tables, and symbol table entries
using attr_bitset = bitset<ATTR_bitset_size>;
using symbol_table = unordered_map<string*, symbol*>;
using symbol_entry = pair<string*,symbol*>;
using symbol_stack = vector<symbol_table*>; // 

struct symbol {
   attr_bitset attributes;
   symbol_table* fields;
   size_t filenr, linenr, offset, block_nr;
   symbol* parameters;
};
//vector<symbol_table*> struct_stack;

string get_attr_name(attr_bitset attributes);

#endif

