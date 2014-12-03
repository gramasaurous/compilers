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

// Symbol table output file
extern FILE *sym_file;

// Initialize the symboltables
void typecheck_init() {
   symbol_table *global = new symbol_table();
   blockstack.push_back(blockctr);
   idents.push_back(global);
}
// Enter a block
// Call this to create a new scope
// Returns a pointer to a symbol table to be used for the new scope
symbol_table *enter_block() {
   blockctr = next_block;
   next_block++;
   blockstack.push_back(blockctr);
   symbol_table *new_block = new symbol_table();
   return new_block;
}
// Leave a block
// Drops 'down' from the current block
void exit_block() {
   blockstack.pop_back();
}

// do a print
void dump_sym (symbol *sym, string *name) {
   if (sym == NULL || name == NULL) return;   
   size_t depth = sym->block_nr;
   for (size_t i = 0; i < depth; i++) fprintf(sym_file, "\t");
   fprintf(sym_file, "%s (%zu.%zu.%zu) {%zu} %s\n",
           name->c_str(),
           sym->filenr,
           sym->linenr,
           sym->offset, 
           sym->block_nr,
           get_attr_string(sym->attributes));
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
   newsym->block_nr = blockstack.back();
   newsym->attributes = sym_node->attributes;
   newsym->parameters = NULL;
   newsym->type_id = NULL;
   return newsym;
}

// Insert a symbol + name into a symbol table
bool insert_symbol(symbol_table *table, symbol *sym, string *name) {
   if (table == NULL || sym == NULL || name == NULL) return false;
   if (table->find(name) != table->end()) return false;
   table->insert(make_pair(name, sym));
   dump_sym(sym, name);
   return true;
}

// Create a new struct
void new_type (astree *struct_node) {
   if (struct_node == NULL || struct_node->children.empty()) return;
   astree *struct_name = struct_node->children[0];
   if (struct_name == NULL) return;
   symbol *type_sym = new_symbol(struct_name);
   insert_symbol(&types, type_sym, (string*)struct_name->lexinfo);
   symbol_table *field_table = new symbol_table();
   // use c to skip the first child in the following
   // for:each loop 
   bool c = true;
   for (auto &i : struct_node->children) {
      if (i->symbol == TOK_TYPEID && c) {
         c = false;
         continue;
      }
      if (i->children.empty()) return;
      astree *field_name = i->children[0];
      if (field_name == NULL) return;
      symbol *field = new_symbol(i);
      if (field == NULL) return;
      field->attributes.set(ATTR_field);
      insert_symbol(field_table, field, (string*)field_name->lexinfo);
   }
   fields.push_back(field_table);
   type_sym->fields = field_table;
   struct_node->visited = true;
   struct_node->struct_entry = new symbol_entry(
            (string*)struct_name->lexinfo, type_sym);
}

// Create a new variable
void new_var(astree *var_node) {
   //printf("new var...\n");
   if (var_node == NULL || var_node->children.empty()) return;
   astree *var_type = var_node->children[0];
   if (var_type == NULL) return;
   astree *var_name = var_type->children[0];
   if (var_name == NULL) return;
   astree *var_val = var_node->children[1];
   if (var_val == NULL) return;
   symbol *var_sym = new_symbol(var_node);
   if (var_sym == NULL) return;
   var_sym->attributes.set(ATTR_variable);
   insert_symbol(idents[blockstack.back()],
                 var_sym, (string*)var_name->lexinfo);
   var_node->visited = true;
}

// Enter declarations for a new block onto the 
// current block (as seen from the top of the block stack)
// note that enter_block()  should be called before this function
// if you wish to have this inside a new scope  
void new_block(astree *block_node) {
   //printf("new block...\n");
   for (auto &i : block_node->children) {
      switch(i->symbol) {
      case (TOK_VARDECL):
         new_var(i);
         break;
      case (TOK_BLOCK): {
         symbol_table *iblock = enter_block();
         idents.push_back(iblock);
         new_block(i);
         exit_block();
         break;
      }
      default:
         break;
      }
   }
   block_node->visited = true;
}

// Create a new function parameter
void new_param(astree *param, symbol_table *fn_table,
      symbol *fn_symbol) {
   if (param == NULL) return;
   if (param->children.empty()) return;
   astree *param_name = param->children[0];
   if (param_name == NULL) return;
   symbol *param_sym = new_symbol(param_name);
   if (param_sym == NULL) return;
   param_sym->attributes.set(ATTR_variable);
   param_sym->attributes.set(ATTR_param);
   //printf("parameter: %s %s\n", param->lexinfo->c_str(),
   //   param_name->lexinfo->c_str());
   fn_symbol->parameters->push_back(param_sym);
   insert_symbol(fn_table, param_sym, (string*)param_name->lexinfo);
   param->visited = true;
}

void new_fn(astree *fn_node) {
   //printf("new function...\n");
   // get all the necessary children nodes -
   // fn type, fn name, fn params, fn block
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

   dump_sym(fn_symbol, (string *)name_node->lexinfo);

   // Enter the new block
   symbol_table *fn_table = enter_block();
   idents.push_back(fn_table);
   //printf("New Function: %s %s() \n",
   //   type_node->lexinfo->c_str(), name_node->lexinfo->c_str());

   // get parameters
   for (auto &param_type : params_node->children) {
      new_param(param_type, fn_table, fn_symbol);
   }

   // get function block
   new_block(fnblock_node);
   exit_block();
   insert_symbol(idents[blockstack.back()],
                 fn_symbol, (string*)name_node->lexinfo);
   fn_node->visited = true;
}


// for testing purposes only
void dump_tables() {
   printf("Idents:\n");
   int scope = 0;
   for (auto v: idents) {
      printf("%d\n", scope);
      for (auto i = v->cbegin(); i != v->cend(); i++) {
         printf("\t %s %s\n", i->first->c_str(),
         get_attr_string(i->second->attributes));
      }
      scope++;
   }
   printf("Types:\n");
   for (auto i = types.cbegin(); i != types.cend(); i++) {
      printf("%s\n", i->first->c_str());
      for (auto j = i->second->fields->cbegin();
            j != i->second->fields->cend(); j++) {
         printf("\t%s %s\n", j->first->c_str(),
         get_attr_string(j->second->attributes));
      }
   }
}

// Perform a depth-first traversal of the tree pointed to by root
// for now -- think of this as enter_program()
// somewhat similar to enter_block() should be ...?
void get_fn_names(astree *root) {
   if (root == NULL) return;
   if (root->children.empty()) {
      return;
   } else {
      for (auto &child : root->children) {
         if (child->visited) continue;
         switch(child->symbol) {
            case TOK_STRUCT: new_type(child); break;
            case TOK_FUNCTION: new_fn(child); break;
            case TOK_VARDECL: new_var(child); break;
            case TOK_BLOCK: {
               symbol_table *iblock = enter_block();
               idents.push_back(iblock);
               new_block(child);
               exit_block();
               break;
            }
         }
         get_fn_names(child);
      }
   }
}

// This is where the large switch statement will appear. But for now
// I will just print out the symbol of the node
void set_attributes(astree* n) {
   if (n == NULL) return;
   DEBUGF('z', "%s\n", get_yytname(n->symbol));
   switch (n->symbol) {
      case TOK_INTCON:                    // constants set ATTR_const
         n->attributes.set(ATTR_const);   // and fall through to set var
      case TOK_INT:
         n->attributes.set(ATTR_int);      break;
      case TOK_CHARCON:
         n->attributes.set(ATTR_const);
      case TOK_CHAR:
         n->attributes.set(ATTR_char);     break;
      case TOK_STRINGCON:
         n->attributes.set(ATTR_const);
      case TOK_STRING:
         n->attributes.set(ATTR_string);   break;
      case TOK_VOID:
         n->attributes.set(ATTR_void);     break;
      case TOK_BOOL:
         n->attributes.set(ATTR_bool);     break;
      case TOK_STRUCT:
         n->attributes.set(ATTR_struct);   break;
      case TOK_NULL:
         n->attributes.set(ATTR_const);    break;
      case TOK_FIELD:
         n->attributes.set(ATTR_field);    break;
      case TOK_FUNCTION:
         n->attributes.set(ATTR_function); break;
      case TOK_ARRAY:
         n->attributes.set(ATTR_array);    break;
      case TOK_VARDECL:
         n->attributes.set(ATTR_variable); break;
      case TOK_PROTOTYPE:
         n->attributes.set(ATTR_proto);    break;
      default: break;
   }
}

// Recursively set the attributes for a tree pointed to by root
void set_attributes_rec (astree *root) {
   if (root == NULL) return;
   if (root->children.empty()) {
      return;
   } else {
      for (auto &child : root->children) {
         set_attributes(child);
         set_attributes_rec(child);
      }
   }
}

// Return a string which is the concatenation of all of the attributes
// This is helpful for the modified printing necessary for the .ast file
// Potentially change the passed variable to an AST node so I can 
// conditionally concatenate the typeid of a struct
const char *get_attr_string(attr_bitset attributes) {
   string attrs = "";
   if (attributes.test(ATTR_void))     attrs += "void ";
   if (attributes.test(ATTR_bool))     attrs += "bool ";
   if (attributes.test(ATTR_char))     attrs += "char ";
   if (attributes.test(ATTR_int))      attrs += "int ";
   if (attributes.test(ATTR_null))     attrs += "null ";
   if (attributes.test(ATTR_string))   attrs += "string ";
   if (attributes.test(ATTR_struct))   attrs += "struct ";
   if (attributes.test(ATTR_array))    attrs += "array ";
   if (attributes.test(ATTR_function)) attrs += "function ";
   if (attributes.test(ATTR_proto))    attrs += "prototype ";
   if (attributes.test(ATTR_variable)) attrs += "variable ";
   if (attributes.test(ATTR_field))    attrs += "field ";
   if (attributes.test(ATTR_typeid))   attrs += "typeid ";
   if (attributes.test(ATTR_param))    attrs += "param ";
   if (attributes.test(ATTR_lval))     attrs += "lval ";
   if (attributes.test(ATTR_const))    attrs += "const ";
   if (attributes.test(ATTR_vreg))     attrs += "vreg ";
   if (attributes.test(ATTR_vaddr))    attrs += "vaddr ";
   return attrs.c_str();
}

void free_typechecker() {
   int size = idents.size();
   for (int i = 0; i < size; i++) {
      free (idents[i]);
   }
}
