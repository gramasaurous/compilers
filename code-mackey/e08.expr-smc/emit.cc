
#include <stdio.h>
#include <assert.h>

#include "astree.h"
#include "emit.h"
#include "lyutils.h"
#include "auxlib.h"

void emit (astree*);

void emit_insn (const char* opcode, const char* operand, astree* tree) {
   printf ("%-10s%-10s%-20s; %s %ld.%ld\n", "",
            opcode, operand, scanner_filename (tree->filenr)->c_str(),
            tree->linenr, tree->offset);
}

void postorder (astree* tree) {
   assert (tree != NULL);
   for (size_t child = 0; child < tree->children.size(); ++child) {
      emit (tree->children.at(child));
   }
}

void postorder_emit_stmts (astree* tree) {
   postorder (tree);
}

void postorder_emit_oper (astree* tree, const char* opcode) {
   postorder (tree);
   emit_insn (opcode, "", tree);
}

void postorder_emit_semi (astree* tree) {
   postorder (tree);
   emit_insn ("", "", tree);
}

void emit_push (astree* tree, const char* opcode) {
   emit_insn (opcode, tree->lexinfo->c_str(), tree);
}

void emit_assign (astree* tree) {
   assert (tree->children.size() == 2);
   astree* left = tree->children.at(0);
   emit (tree->children.at(1));
   if (left->symbol != IDENT) {
      eprintf ("%:%s: %d: left operand of `=' is not an identifier\n",
               scanner_filename (left->filenr)->c_str(), left->linenr);
   }else{
      emit_insn ("popvar", left->lexinfo->c_str(), left);
   }
}


void emit (astree* tree) {
   switch (tree->symbol) {
      case ROOT  : postorder_emit_stmts (tree);       break;
      case ';'   : postorder_emit_semi (tree);        break;
      case '='   : emit_assign (tree);                break;
      case '+'   : postorder_emit_oper (tree, "add"); break;
      case '-'   : postorder_emit_oper (tree, "sub"); break;
      case '*'   : postorder_emit_oper (tree, "mul"); break;
      case '/'   : postorder_emit_oper (tree, "div"); break;
      case '^'   : postorder_emit_oper (tree, "pow"); break;
      case POS   : postorder_emit_oper (tree, "pos"); break;
      case NEG   : postorder_emit_oper (tree, "neg"); break;
      case IDENT : emit_push (tree, "pushvar");       break;
      case NUMBER: emit_push (tree, "pushnum");       break;
      default    : assert (false);                    break;
   }
}

void emit_sm_code (astree* tree) {
   printf ("\n");
   if (tree) emit (tree);
}

RCSC("$Id: emit.cc,v 1.3 2013-09-20 17:52:13-07 - - $")

