CPP       = g++
BIN       = stealth
SRCDIR    = src

all:
	@(cd ${SRCDIR}; make )

clean: 
	@rm -rf ${BIN} ${SRCDIR}/*.o *.core *.log *.pid *~ .*.swp

debug:
	@(cd ${SRCDIR}; make debug )
