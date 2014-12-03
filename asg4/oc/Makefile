# Graham Greving
# ggreving@ucsc.edu
# CMPS104a: asg4: Makefile

MKFILE    = Makefile
DEPSFILE  = ${MKFILE}.deps
NOINCLUDE = git submit testsubmit clean spotless
NEEDINCL  = ${filter ${NOINCLUDE}, ${MAKECMDGOALS}}
VALGRIND  = valgrind --leak-check=full --show-reachable=yes

#
# Definitions of list of files:
#
MODULES    = typecheck astree lyutils auxlib stringset
HSOURCES   = ${MODULES:=.h}
CSOURCES   = ${MODULES:=.cpp} main.cpp 
LSOURCES  = scanner.l
YSOURCES  = parser.y
ETCSRC    = README ${MKFILE} ${DEPSFILE}
ETCSUB    = README ${MKFILE}
CLGEN     = yylex.cpp
HYGEN     = yyparse.h
CYGEN     = yyparse.cpp
CGENS     = ${CLGEN} ${CYGEN}
ALLGENS   = ${HYGEN} ${CGENS}
EXECBIN   = oc
ALLCSRC   = ${CSOURCES} ${CGENS}
OBJECTS   = ${ALLCSRC:.cpp=.o}
LREPORT   = yylex.output
YREPORT   = yyparse.output
REPORTS   = ${LREPORT} ${YREPORT}
ALLSRC    = ${ETCSRC} ${YSOURCES} ${LSOURCES} ${CSOURCES} ${HSOURCES}
SUBSRC    = ${ETCSUB} ${LSOURCES} ${YSOURCES} ${CSOURCES} ${HSOURCES}

#
# Definitions of the compiler and compilation options:
#
GCC       = g++ -g -O0 -Wall -Wextra -std=gnu++11 -fdiagnostics-color
MKDEPS    = g++ -MM -std=gnu++11

#
# Definitions of various other things
#
HOST        = $(findstring ucsc,$(shell hostname))
CLASS       = cmps104a-wm.f14
PROJECT     = asg4
TESTDIR    = ../../grading/${PROJECT}_test/

#
# The first target is always ``all'', and hence the default,
# and builds the executable images
#
all: ${EXECBIN}
ifeq ($(HOST),ucsc)
	- checksource ${SUBSRC}
endif
#
# Build the executable image from the object files.
#
${EXECBIN}: ${CLGEN} ${CYGE{} ${OBJECTS}
	${GCC} -o ${EXECBIN} ${OBJECTS}

#
# Build an object file form a C source file.
#
%.o: %.cpp
	${GCC} -c $<

#
# Build the scanner.
#
${CLGEN}: ${LSOURCES}
	flex --outfile=${CLGEN} ${LSOURCES} 2>${LREPORT}
	- grep -v '^  ' ${LREPORT}

#
# Build the parser.
#
${CYGEN} ${HYGEN}: ${YSOURCES}
	bison --defines=${HYGEN} --output=${CYGEN} ${YSOURCES}

#
# Clean and spotless remove generated files.
#
clean:
	- rm ${OBJECTS} ${ALLGENS} ${REPORTS} ${DEPSFILE}

spotless: clean
	- rm ${EXECBIN}
	- rm -r ${TESTDIR}
	- rm *.str
	- rm *.tok
	- rm *.ast
	- rm *.sym

#
# Build the dependencies file using the C preprocessor
#
deps: ${ALLCSRC}
	@ echo "# ${DEPSFILE} created `date` by ${MAKE}" >${DEPSFILE}
	${MKDEPS} ${ALLCSRC} >>${DEPSFILE}

${DEPSFILE}:
	@ touch ${DEPSFILE}
	${MAKE} --no-print-directory deps

#
# Test Submission (dry run)
#
testsubmit: ${SUBSRC}
	- rm -r ${TESTDIR}*
	- rmdir ${TESTDIR}
	- mkdir ${TESTDIR}
	cp ${SUBSRC} ${TESTDIR}
	cp ../../grading/mk* ${TESTDIR}

#
# Commit to git repo
#
git:
	git commit -a

#
# Real Submission (will fail if not on unix.ucsc.edu)
#
submit: ${SUBSRC} spotless
ifeq ($(HOST),ucsc)
	- checksource ${SUBSRC}
	- git commit -m "gmake submission commit: `LC_TIME=C date`"
	submit ${CLASS} ${PROJECT} ${SUBSRC}
endif

#
# Everything
#
again :
	gmake --no-print-directory spotless deps all
	
ifeq "${NEEDINCL}" ""
include ${DEPSFILE}
endif

