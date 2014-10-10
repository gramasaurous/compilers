# $Id: Makefile,v 1.13 2014-10-09 15:49:32-07 - - $

GPP   = g++ -g -O0 -Wall -Wextra -std=gnu++11
GRIND = valgrind --leak-check=full --show-reachable=yes

all : teststring

teststring : main.o stringset.o
	${GPP} main.o stringset.o -o teststring

%.o : %.cpp
	${GPP} -c $<

ci :
	cid + Makefile stringset.h stringset.cpp main.cpp

spotless : clean
	- rm teststring Listing.ps Listing.pdf test?.out test?.err

clean :
	-rm stringset.o main.o

test : teststring
	${GRIND} teststring * * >test1.out 2>test1.err
	${GRIND} teststring foo foo foo foo bar bar bar foo qux baz \
	         >test2.out 2>test2.err

lis : test
	mkpspdf Listing.ps stringset.h stringset.cpp main.cpp \
	        Makefile test1.out test1.err test2.out test2.err

# Depencencies.
main.o: main.cpp stringset.h
stringset.o: stringset.cpp stringset.h

