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

symbol_table idents;
symbol_table types;
symbol_stack fields;

symbol *new_symbol(astree *token) {
   symbol *sym = new symbol();
   sym->filenr = token->filenr;
   sym->linenr = token->linenr;
   sym->offset = token->offset;
   // anything else necessary?
   return sym;
}

// Enter a new type into the types symbol table
// Potential error in parser.y has put this fn on hold for the time
// being
void new_type(astree *struct_node) {
   // Check to see if the type already exists
   // if so -- do some stuff
   // otherwise we are free to create the new entry
   // into the types symbol table then
   // create the new fields symbol table for this type

   struct_node->attributes.set(ATTR_struct);
   symbol *s = new_symbol(struct_node);
   s->attributes.set(ATTR_struct);
   symbol_entry np = make_pair((string*)struct_node->lexinfo, s);
   types.insert(np);
   astree *typeid_node = struct_node->children[0];
   printf("typeid: %s\n", typeid_node->lexinfo->c_str());
   printf("all struct children:\n");
   for (auto &i : struct_node->children) {
      printf("%s\n", i->lexinfo->c_str());
      for (auto &j: i->children) printf("\t%s\n", j->lexinfo->c_str()); 
   }
}

void new_ident(astree *ident_node) {

}

void new_function (astree *fn_node) {
   
}

// This is where the large switch statement will appear. But for now
// I will just print out the symbol of the node
void visit_node(astree* n) {
   if (n == NULL) return;
   DEBUGF('z', "%s\n", get_yytname(n->symbol));
   switch (n->symbol) {
   case TOK_VOID:    n->attributes.set(ATTR_void);   break;
   case TOK_BOOL:    n->attributes.set(ATTR_bool);   break;
   case TOK_CHAR:    n->attributes.set(ATTR_char);   break;
   case TOK_INT :    n->attributes.set(ATTR_int);    break;
   case TOK_NULL:    n->attributes.set(ATTR_null);   break;
   case TOK_STRING:  n->attributes.set(ATTR_string); break;
   case TOK_STRUCT:     new_type(n); break;
   case TOK_FUNCTION:   new_function(n); break;
   default: break;
   }
}

// Perform a depth-first traversal of the tree pointed to by root
void depth_first(astree *root) {
   if (root == NULL) return;
   if (root->children.empty()) {
      return;
   } else {
      for (auto &child : root->children) {
         visit_node(child);
         depth_first(child);
      }
   }
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
