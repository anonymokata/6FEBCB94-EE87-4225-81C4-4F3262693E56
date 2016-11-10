all: rpn

test:
	rpn -test

rpn: Makefile rpn.c
	gcc rpn.c test.c -o rpn

clean:
	rm rpn
