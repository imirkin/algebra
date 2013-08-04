CXXFLAGS = -O2 -Wall -g -std=c++0x
LDFLAGS = -lstdc++

all: test chinese
test: test.o
chinese: chinese.o

%.o: elements.h types.h spaces.h

.PHONY = clean
clean:
	-rm -f *.o test chinese
