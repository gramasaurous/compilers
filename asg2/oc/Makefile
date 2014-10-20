# Graham Greving
# ggreving@ucsc.edu
# asg4: Makefile

MKFILE      = Makefile
DEPFILE     = ${MKFILE}.dep
NOINCL      = git clean spotless
NEEDINCL    = ${filter ${NOINCL}, ${MAKECMDGOALS}}
GMAKE       = ${MAKE} --no-print-directory
COMPILECPP  = g++ -g -O0 -Wall -Wextra -std=gnu++11
MAKEDEPCPP  = g++ -MM -std=gnu++11

MODULES     = auxlib stringset
CPPHEADER   = ${MODULES:=.h}
CPPSOURCE   = ${MODULES:=.cpp} main.cpp
OTHERS      = ${MKFILE} README
ALLSOURCES  = ${CPPHEADER} ${TEMPLATES} ${CPPSOURCE} ${OTHERS}
EXECBIN     = oc
OBJECTS     = ${CPPSOURCE:.cpp=.o}
LINKLIBS    =

HOST 		= $(findstring ucsc,$(shell hostname))
CLASS       = cmps104a-wm.f14
PROJECT     = asg1
TEST_DIR    = ../../grading/${PROJECT}_test

all : ${EXECBIN} 
ifeq ($(HOST),ucsc)
	- checksource ${ALLSOURCES}
endif

${EXECBIN} : ${OBJECTS}
	${COMPILECPP} -o $@ ${OBJECTS} ${LINKLIBS}

%.o : %.cpp
	${COMPILECPP} -c $<

clean :
	- rm ${OBJECTS} ${DEPFILE}

spotless : clean
	- rm ${EXECBIN}

submit : ${ALLSOURCES}
ifeq ($(HOST),ucsc)
	- checksource ${ALLSOURCES}
	submit ${CLASS} ${PROJECT} ${ALLSOURCES}
endif

git :
	#git commit -a -m "gmake auto commit: `LC_TIME=C date`"
	git commit -a

dep : ${CPPSOURCE} ${CPPHEADER} ${GENFILES}
	@ echo "# ${DEPFILE} created `LC_TIME=C date`" >${DEPFILE}
	${MAKEDEPCPP} ${CPPSOURCE} >>${DEPFILE}

${DEPFILE} :
	@ touch ${DEPFILE}
	${GMAKE} dep

again :
	${GMAKE} spotless dep all

testsubmit :
	- rm ${TEST_DIR}/*
	cp ${ALLSOURCES} ${TEST_DIR}/
	cp ../../grading/mk* ${TEST_DIR}/

ifeq (${NEEDINCL}, )
include ${DEPFILE}
endif

