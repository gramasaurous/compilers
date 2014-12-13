// Graham Greving
// ggreving@ucsc.edu
// CMPS104a: asg5: icgen.cpp

#include <string>
#include <unordered_map>
#include <vector>

#include "icgen.h"
#include "astree.h"
#include "lyutils.h"

extern FILE *oil_file;

void visit(astree *node) {
	if (node == NULL) return;
	switch(node->symbol) {
		fprintf(oil_file, "%s\n", get_yytname(node->symbol));
	}
}

void gen_ic (astree *root) {
   if (root == NULL) return;
   if (root->children.empty()) {
      return;
   } else {
      for (auto &child : root->children) {
      	visit(child);
      }
   }
}