# $Id: Makefile,v 1.3 2013-09-23 14:39:10-07 - - $

GPP   = g++ -g -O0 -Wall -Wextra -std=gnu++0x
GRIND = valgrind --leak-check=full --show-reachable=yes


all : teststring

teststring : main.o stringset.o
	${GPP} main.o stringset.o -o teststring

%.o : %.cpp
	${GPP} -c $<

ci :
	cid + Makefile stringset.h stringset.cpp main.cpp

spotless : clean
	- rm teststring Listing.ps Listing.pdf \

clean :
	-rm stringset.o main.o

test.out : teststring
	${GRIND} teststring * * * >test.out 2>&1

lis : test.out
	mkpspdf Listing.ps stringset.h stringset.cpp main.cpp \
	        Makefile test.out

# Depencencies.
main.o: main.cpp stringset.h
stringset.o: stringset.cpp stringset.h

