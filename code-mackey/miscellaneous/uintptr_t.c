// $Id: uintptr_t.c,v 1.17 2011-11-09 19:57:03-08 - - $

// Illustrate the idea of using pointers as uintptr_t values
// for indexing into the symbol hash-stack table.

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
   char *string;
} *node;

int main (int argc, char **argv) {
   node *pseudo = calloc ((ssize_t) argc, sizeof (node));
   assert (pseudo != 0);
   for (int argi = 0; argi < argc; ++argi) {
      node new = malloc (sizeof (struct node));
      assert (new != NULL);
      new->string = strdup (argv[argi]);
      assert (new->string);
      pseudo[argi] = new;
   }
   for (int argi = 0; argi < argc; ++argi) {
      node ref = pseudo[argi];
      uintptr_t hashcode = (uintptr_t) ref;
      printf ("%16p %16" PRIxPTR " %6" PRIdPTR " %6" PRIdPTR
              " %6" PRIdPTR "\n",
              ref, hashcode, hashcode % 31, hashcode % 127,
              hashcode % 511);
   }
   return EXIT_SUCCESS;
}

//TEST// runprog -x uintptr_t.lis uintptr_t *
//TEST// mkpspdf uintptr_t.ps uintptr_t.c uintptr_t.c.log uintptr_t.lis

