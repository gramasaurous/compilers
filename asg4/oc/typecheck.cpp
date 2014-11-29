// Graham Greving
// ggreving@ucsc.edu
// CMPS104a: asg4: typecheck.cpp

#include <string>
#include <unordered_map>
#include <vector>

#include "typecheck.h"
#include "astree.h"
#include "lyutils.h"

// Data Structures and symbol tables:
// We need symbol tables (stacks?) for:
// - function and variable names
// - type names
// - fields

symbol_stack idents; // names all function and variable names
symbol_table types;  // names all type idents -- structs + primitives
symbol_stack fields; // names all possible struct fields

// Global block counter
size_t blockctr = 0;
size_t next_block = 1;
vector<size_t> blockstack;

// This is where the large switch statement will appear. But for now
// I will just print out the symbol of the node
void visit_node(astree* n) {
   if (n == NULL) return;
   DEBUGF('z', "%s\n", get_yytname(n->symbol));
   switch (n->symbol) {
      default: break;
   }
}

// Create a new symbol -- copy the filenr, linenr, offset from the ast
// node. Get the blocknr from the global blockptr
symbol *new_symbol (astree *sym_node) {
   if (sym_node == NULL) return NULL;
   symbol *newsym = new symbol();
   newsym->fields = NULL;
   newsym->filenr = sym_node->filenr;
   newsym->linenr = sym_node->linenr;
   newsym->offset = sym_node->offset;
   newsym->block_nr = blockstack[0];
   newsym->parameters = NULL;
   return newsym;
}

symbol_table *enter_block() {
   blockctr = next_block;
   next_block++;
   blockstack.push_back(blockctr);
   symbol_table *new_block = new symbol_table();

   return new_block;
}

void exit_block() {
   blockstack.pop_back();
}

void new_var(astree *var_node) {
   if (var_node == NULL || var_node->children.empty()) return;
   astree *var_type = var_node->children[0];
   astree *var_name = var_type->children[1];
   astree *var_val = var_node->children[2];
   symbol *var_sym = new_symbol(var_node);
   idents[blockstack.back()]->insert(
      make_pair((string*)var_name->lexinfo, var_sym));
}

// Enter declarations for a new block onto the 
// current block (as seen from the top of the block stack)
// note that enter_block()  should be called before this function
// if you wish to have this inside a new scope  
void new_block(astree *block_node) {
   for (auto &i : block_node->children) {
      if (i->symbol == TOK_VARDECL) new_var(i);
   }
}

void new_fn(astree *fn_node) {
   if (fn_node == NULL || fn_node->children.empty()) return;
   astree *type_node = fn_node->children[0];
   if (type_node == NULL || type_node->children.empty()) return;
   astree *name_node = type_node->children[0];
   if (name_node == NULL) return;
   astree *params_node = fn_node->children[1];
   if (params_node == NULL) return;
   astree *fnblock_node = fn_node->children[2];
   if (fnblock_node == NULL) return;

   symbol *fn_symbol = new_symbol(fn_node);
   if (fn_symbol == NULL) return;
   fn_symbol->attributes.set(ATTR_function);
   fn_symbol->parameters = new vector<symbol*>();

   // Enter the new block
   symbol_table *fn_table = enter_block();
   printf("New Function: %s %s() \n",
      type_node->lexinfo->c_str(), name_node->lexinfo->c_str());
   
   // get parameters
   for (auto &param_type : params_node->children) {
      if (param_type->children.empty()) return;
      astree *param_name = param_type->children[0];
      if (param_name == NULL) return;
      symbol *param_sym = new_symbol(param_name);
      if (param_sym == NULL) return;
      param_sym->attributes.set(ATTR_param);
      printf("parameter: %s %s\n", param_type->lexinfo->c_str(),
         param_name->lexinfo->c_str());
      fn_symbol->parameters->push_back(param_sym);
      fn_table->insert(make_pair((string*)param_name->lexinfo, param_sym));
   }
   // get function block
   new_block(fnblock_node);
   exit_block();
   idents[0]->insert(make_pair((string*)name_node->lexinfo, fn_symbol));
   idents.push_back(fn_table);
}

// for testing purposes only
void dump_tables() {
   printf("Idents:\n");
   int scope = 0;
   for (auto v: idents) {
      printf("%d\n", scope);
      for (auto i = v->cbegin(); i != v->cend(); i++) {
         printf("%s\n", i->first->c_str());
      }
      scope++;
   }
}

// Perform a depth-first traversal of the tree pointed to by root
// for now -- think of this as enter_program()
// somewhat similar to enter_block() should be ...?
void get_fn_names(astree *root) {
   if (root == NULL) return;
   blockstack.push_back(blockctr);
   if (root->children.empty()) {
      return;
   } else {
      for (auto &child : root->children) {
         // visit_node(child);
         if (child->symbol == TOK_FUNCTION) new_fn(child);
         //if (child->symbol == TOK_VARDECL) new_var(child);
         get_fn_names(child);
      }
   }
}

void typecheck_init() {
   symbol_table *global = new symbol_table();
   idents.push_back(global);
}

// Return a string which is the concatenation of all of the attributes
// This is helpful for the modified printing necessary for the .ast file
// Potentially change the passed variable to an AST node so I can 
// conditionally concatenate the typeid of a struct
string get_attr_name(attr_bitset attributes) {
   string attrs = "";
   if (attributes.test(0))  attrs += "void ";
   if (attributes.test(1))  attrs += "bool ";
   if (attributes.test(2))  attrs += "char ";
   if (attributes.test(3))  attrs += "int ";
   if (attributes.test(4))  attrs += "null ";
   if (attributes.test(5))  attrs += "string ";
   if (attributes.test(6))  attrs += "struct ";
   if (attributes.test(7))  attrs += "array ";
   if (attributes.test(8))  attrs += "function ";
   if (attributes.test(9))  attrs += "variable ";
   if (attributes.test(10)) attrs += "field ";
   if (attributes.test(11)) attrs += "typeid ";
   if (attributes.test(12)) attrs += "param ";
   if (attributes.test(13)) attrs += "lval ";
   if (attributes.test(14)) attrs += "const ";
   if (attributes.test(15)) attrs += "vreg ";
   if (attributes.test(16)) attrs += "vaddr ";
   return attrs;
}
