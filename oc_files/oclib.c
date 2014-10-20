// $Id: oclib.c,v 1.45 2012-11-16 21:10:41-08 - - $

#include <ctype.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __OCLIB_C__
#include "oclib.oh"

ubyte **oc_argv;

void ____assert_fail (char *expr, char *file, int line) {
   fflush (NULL);
   fprintf (stderr, "%s: %s:%d: assert (%s) failed.\n",
            basename ((char *) oc_argv[0]), file, line, expr);
   fflush (NULL);
   abort();
}

void *xcalloc (int nelem, int size) {
   void *result = calloc (nelem, size);
   assert (result != NULL);
   return result;
}

void __ocmain (void);
int main (int argc, char **argv) {
   argc = argc; // warning: unused parameter 'argc'
   oc_argv = (ubyte **) argv;
   __ocmain();
   return EXIT_SUCCESS;
}


ubyte *scan (int (*skipover) (int), int (*stopat) (int)) {
   int byte;
   do {
      byte = getchar();
      if (byte == EOF) return NULL;
   } while (skipover (byte));
   ubyte buffer[0x1000];
   ubyte *end = buffer;
   do {
      *end++ = byte;
      assert (end < buffer + sizeof buffer);
      *end = '\0';
      byte = getchar();
   }while (byte != EOF && ! stopat (byte));
   ubyte *result = (ubyte *) strdup ((char *) buffer);
   assert (result != NULL);
   return result;
}

int isfalse (int byte)   { return 0 & byte; } 
int isnl (int byte)      { return byte == '\n'; }
void __putb (ubyte byte) { printf ("%s", byte ? "true" : "false"); } 
void __putc (ubyte byte) { printf ("%c", byte); } 
void __puti (int val)    { printf ("%d", val); }
void __puts (ubyte *str) { printf ("%s", str); }
void __endl (void)       { printf ("%c", '\n'); fflush (NULL); }
int __getc (void)        { return getchar(); } 
ubyte *__getw (void)     { return scan (isspace, isspace); }
ubyte *__getln (void)    { return scan (isfalse, isnl); } 
ubyte **__getargv (void) { return oc_argv; }
void __exit (int status) { exit (status); }

