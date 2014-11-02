# Graham Greving
# ggreving@ucsc.edu
# CMPS104a: asg2: Makefile

MKFILE      = Makefile
DEPFILE     = ${MKFILE}.dep
NOINCL      = git clean spotless
NEEDINCL    = ${filter ${NOINCL}, ${MAKECMDGOALS}}
GMAKE       = ${MAKE} --no-print-directory
COMPILECPP  = g++ -g -O0 -Wall -Wextra -std=gnu++11
MAKEDEPCPP  = g++ -MM -std=gnu++11

## PARSER AND SCANNER ##
LSOURCES	= scanner.l
YSOURCES	= parser.y
CLGEN		= yylex.cpp
HYGEN 		= yyparse.h
CYGEN 		= yyparse.cpp
LREPORT 	= yylex.output
YREPORT 	= yyparse.output

MODULES     = auxlib stringset lyutils
CPPHEADER   = ${MODULES:=.h}
CPPSOURCE   = ${MODULES:=.cpp} main.cpp
OTHERS      = ${MKFILE} README
GEN 		= ${CYGEN} ${HYGEN} ${CLGEN}
ALLSOURCES  = ${CPPHEADER} ${CPPSOURCE} ${OTHERS} ${LSOURCES} ${YSOURCES}
EXECBIN     = oc
OBJECTS     = ${CPPSOURCE:.cpp=.o} ${CLGEN:.cpp=.o} ${CYGEN:.cpp=.o}
LINKLIBS    = 

HOST 		= $(findstring ucsc,$(shell hostname))
CLASS       = cmps104a-wm.f14
PROJECT     = asg2
TEST_DIR    = ../../grading/${PROJECT}_test

all: ${EXECBIN} 
ifeq ($(HOST),ucsc)
	- checksource ${ALLSOURCES}
endif

${EXECBIN}: ${CLGEN} ${CYGEN} ${OBJECTS}
	${COMPILECPP} -o $@ ${OBJECTS} ${LINKLIBS}

%.o: %.cpp
	${COMPILECPP} -c $<

${CLGEN}: ${LSOURCES}
	flex --outfile=${CLGEN} ${LSOURCES} 2>${LREPORT}
	- grep -v '^ ' ${LREPORT}

${CYGEN} ${HYGEN}: ${YSOURCES}
	bison --defines=${HYGEN} --output=${CYGEN} ${YSOURCES}

clean:
	- rm ${OBJECTS} ${DEPFILE}

spotless: clean
	- rm ${GEN}
	- rm *.output
	- rm ${EXECBIN}
	- rm -rf ${TEST_DIR}

submit: ${ALLSOURCES} spotless
ifeq ($(HOST),ucsc)
	- checksource ${ALLSOURCES}
	submit ${CLASS} ${PROJECT} ${ALLSOURCES}
endif

git: spotless
	#git commit -a -m "gmake auto commit: `LC_TIME=C date`"
	git commit -a

dep: ${CPPSOURCE} ${CPPHEADER} ${GENFILES}
	@ echo "# ${DEPFILE} created `LC_TIME=C date`" >${DEPFILE}
	${MAKEDEPCPP} ${CPPSOURCE} >>${DEPFILE}

${DEPFILE}:
	@ touch ${DEPFILE}
	${GMAKE} dep

again:
	${GMAKE} spotless dep all

testsubmit:
	- rm ${TEST_DIR}/*
	cp ${ALLSOURCES} ${TEST_DIR}/
	cp ../../grading/mk* ${TEST_DIR}/

ifeq (${NEEDINCL}, )
include ${DEPFILE}
endif

