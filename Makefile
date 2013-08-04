CXXFLAGS = -O2 -Wall -g -std=c++0x
LDFLAGS = -lstdc++

all: test chinese
test: test.o
chinese: chinese.o

.PHONY = clean depend
clean:
	-rm -f *.o test chinese
depend:
	makedepend -Y *.cc
# DO NOT DELETE

chinese.o: elements.h modn.h math.h
test.o: elements.h basic.h matrix.h modn.h math.h monomial.h polynomial.h
test.o: rational.h trace.h word.h
