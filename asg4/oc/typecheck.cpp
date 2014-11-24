// Graham Greving
// ggreving@ucsc.edu
// CMPS104a: asg4: typecheck.cpp

#include <string>

#include "typecheck.h"

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

