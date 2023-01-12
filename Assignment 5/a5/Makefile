OPENMP?=-fopenmp
DEBUG?=-g
CC?=gcc
CFLAGS?=-Wextra -Wall -pedantic -std=c99 -fPIC
LDFLAGS?=-lm
LIBFLAGS?=-shared

all: heat-equation heat-equation.so

heat-equation: heat-equation.o debugbmp.o
	$(CC) -o $@ $^ $(LDFLAGS) $(OPENMP)

heat-equation.so: heat-equation.o debugbmp.o
	$(CC) -o $@ $^ $(LDFLAGS) $(OPENMP) $(LIBFLAGS)

# A general rule that tells us how to generate an .o file from a .c
# file.  This cuts down on the boilerplate.
%.o: %.c
	$(CC) -c $< $(CFLAGS) $(DEBUG) $(OPENMP)

clean:
	rm -rf heat-equation *.o *.dSYM *.pyc *.so

test: heat-equation
	./heat-equation 50 50 1000

steps_%.bmp: heat-equation
	./heat-equation 50 50 $* $@
