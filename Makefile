# -fsanitize=... documentation:
#   https://gcc.gnu.org/onlinedocs/gcc-11.4.0/gcc/Instrumentation-Options.html
# 	https://github.com/google/sanitizers/wiki/AddressSanitizerLeakSanitizer

CFLAGS=-Wall -Werror -g -fsanitize=address
TARGETS=clist_test

.PHONY=test scottyone


all: $(TARGETS)

clist_test : clist.c clist_test.c clist.h
	gcc $(CFLAGS) $^ -o $@

test: clist_test
	./clist_test

scottyone: clist_test
	scottycheck isse-05 clist.c clist_test.c clist.h

clean:
	rm -f $(TARGETS)
