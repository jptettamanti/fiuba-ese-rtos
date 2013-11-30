all: test verify
	true

test: test.c fixedptc.h
	gcc -o test -O3 -Wall test.c

verify: verify.c fixedptc.h
	gcc -o verify -O3 -Wall -DFIXEDPT_BITS=32 -lm verify.c
