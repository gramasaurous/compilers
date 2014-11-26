// Graham Greving
// ggreving@ucsc.edu
// CMPS104a: asg4: typecheck.cpp

#include <string>

#include "typecheck.h"
#include "astree.h"
#include "lyutils.h"

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

// This is where the large switch statement will appear. But for now
// I will just print out the symbol of the node
void visit_node(astree* n) {
   if (n == NULL) return;
   DEBUGF('z', "%s\n", get_yytname(n->symbol));
   switch (n->symbol) {
   case TOK_VOID:    n->attributes.set(ATTR_void);   break;// this 72
   case TOK_BOOL:    n->attributes.set(ATTR_bool);   break;
   case TOK_CHAR:    n->attributes.set(ATTR_char);   break;
   case TOK_INT :    n->attributes.set(ATTR_int);    break;
   case TOK_NULL:    n->attributes.set(ATTR_null);   break;
   case TOK_STRING:  n->attributes.set(ATTR_string); break;
   case TOK_STRUCT: {
      n->attributes.set(ATTR_struct);
      break;
   }
   default: break;
   }
}

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
