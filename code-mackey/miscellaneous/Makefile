# $Id: Makefile,v 1.9 2014-10-03 18:47:56-07 - - $

#
# The parser in this directory is a dummy useable for asg2.
# It allows any sequence of tokens to be used in any order
# without actually doing a syntax check.  It is only useful
# as a test harness for the scanner assignment.
#

GPP       = g++ -g -O0 -Wall -Wextra -std=gnu++11
LISTING   = Listing.ps

all : printtokens addresses macros
	cid + Makefile

parser.c parser.h : parser.y
	cid + parser.y
	bison -dtv parser.y -o parser.c

printtokens : printtokens.c parser.c parser.h
	cid + printtokens.c
	${GPP} printtokens.c parser.c -o printtokens

% : %.c
	cid + $<
	${GPP} $< -o $@

lis : all printtokens.output addresses.output macros.output Makefile
	mkpspdf ${LISTING} parser.y parser.h \
	   printtokens.c printtokens.output \
	   addresses.c addresses.output \
	   macros.c macros.output \
	   Makefile

clean :
	- rm parser.o parser.h parser.c \
	     printtokens printtokens.o printtokens.output \
	     parser.output printtokens.output

ci :
	cid + make-kw-table.perl \
	    parser.y printtokens.c Makefile \

%.output : %
	./$< >$@

