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

using namespace std;

// enum defining attribute values
enum {  ATTR_void, ATTR_bool, ATTR_char, ATTR_int, ATTR_null,
        ATTR_string, ATTR_struct, ATTR_array, ATTR_function, ATTR_proto,
        ATTR_variable, ATTR_field, ATTR_typeid, ATTR_param, ATTR_lval,
        ATTR_const, ATTR_vreg, ATTR_vaddr, ATTR_bitset_size
};

// attributes, symbol tables, and symbol table entries
struct symbol;
using attr_bitset = bitset<ATTR_bitset_size>;
using symbol_table = unordered_map<string*, symbol*>;
using symbol_entry = pair<string*,symbol*>;
using symbol_stack = vector<symbol_table*>; // 

#include "astree.h"
#include "auxlib.h"

struct symbol {
   attr_bitset attributes;
   symbol_table* fields;
   size_t filenr, linenr, offset, block_nr;
   vector<symbol*>* parameters;
};

// For now sets up the global symbol stack
void typecheck_init();
// Perform a depth-first traversal of the astree pointed at by root
// and insert new variable and function names into the symbol table
void get_fn_names(astree* root);
void get_var_names(astree* root);
// Set the attributes so we don't have to worry about them later
void set_attributes_rec(astree *root);

// Return a string which is the concatenation of all of the attributes
// This is helpful for the modified printing necessary for the .ast file
// Potentially change the passed variable to an AST node so I can 
// conditionally concatenate the typeid of a struct
const char *get_attr_string(attr_bitset attributes);

void dump_tables();

#endif

