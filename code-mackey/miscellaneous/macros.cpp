// $Id: macros.cpp,v 1.6 2014-08-14 17:06:32-07 - - $

#include <cassert>
#include <iomanip>
#include <iostream>
using namespace std;

#define PRINT(ITEM) cout << #ITEM << " = \"" << ITEM << "\"" << endl;

int main (int argc, char **argv) {
   (void) argc; // : unused parameter 'argc' [-Wunused-parameter]
   PRINT (argv[0]);
   PRINT (__FILE__);
   PRINT (__LINE__);
   PRINT (__func__);
   PRINT (__DATE__);
   PRINT (__TIME__);
   PRINT (__STDC__);
   #ifdef __STDC_VERSION__
   PRINT (__STDC_VERSION__);
   #endif
   #ifdef __cplusplus
   PRINT (__cplusplus);
   #endif
   return 0;
}

//TEST// ./macros >macros.lis 2>&1
//TEST// mkpspdf macros.ps macros.cpp* macros.lis

