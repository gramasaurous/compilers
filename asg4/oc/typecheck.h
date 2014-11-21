// Graham Greving
// ggreving@ucsc.edu
// CMPS104a: asg4: typecheck.h

#include <string>
#include <unordered_map>
#include <bitset>

#include "astree.h"
#include "auxlib.h"

using namespace std;

// enum defining attribute values
enum {   ATTR_VOID, ATTR_bool, ATTR_char, ATTR_int, ATTR_null,
         ATT_string, ATTR_struct, ATTR_array, ATTR_function,
         ATTR_variable, ATTR_field, ATTR_typeid, ATTR_param, ATTR_lval,
         ATTR_const, ATTR_vreg, ATTR_vaddr, ATTR_bitset_size
};

// forward declaration to resolve circular dependencies
struct symbol;

// attributes, symbol tables, and symbol table entries
using attr_bitset = bitset<ATTR_bitset_size>;
using symbol_table = unordered_map<string*, symbol*>;
using symbol_entry = pair<string*,symbol*>;
using symbol_stack = vector<symbol_table*>;

// symbol type
struct symbol {
   attr_bitset attributes;
   symbol_table* fields;
   size_t filenr;
   size_t linenr;
   size_t offset;
   size_t block_nr;
   symbol* parameters;
};

// depth-first traversal of the abstract syntax tree
//void traverse_ast(astree *root);
