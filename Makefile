HDRS = $(*.h)

all: Makefile rpn rpn_tests

test: rpn_tests
	./rpn_tests

run: rpn
	./rpn

rpn: rpn_lib.c rpn.c $(HDRS)
	gcc rpn_lib.c rpn.c -o rpn

rpn_tests: rpn_lib.c tests.c $(HDRS)
	gcc rpn_lib.c tests.c `pkg-config --cflags --libs check` -o rpn_tests
	
clean:
	rm rpn rpn_tests
