HDRS = $(*.h)
CFLAGS = -Wall -Wextra -std=c99 -Werror
all: meta rpn rpn_tests meta

meta: Makefile $(HDRS)

test: meta rpn_tests
	./rpn_tests

run: rpn meta
	./rpn

rpn: meta rpn_lib.c rpn.c
	gcc $(CFLAGS) rpn_lib.c rpn.c -g -o rpn

rpn_tests: meta rpn_lib.c tests.c
	gcc $(CFLAGS) rpn_lib.c tests.c `pkg-config --cflags --libs check` -g -o rpn_tests
	
clean:
	rm -f rpn rpn_tests
