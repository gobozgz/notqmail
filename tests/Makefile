# Tests require
# 1. <https://pkg-config.freedesktop.org>
#    (or <https://github.com/pkgconf/pkgconf>)
# 2. <https://libcheck.github.io/check/>

SHELL=/bin/sh

default: it

.PHONY: clean default it test

TESTBINS = unittest_str unittest_stralloc

CHECK_INCLUDES := `pkg-config --cflags check`
CHECK_LIBS := `pkg-config --libs check`

clean:
	rm -f $(TESTBINS) *.o

it: $(TESTBINS)

test: it
	@for tbin in $(TESTBINS); do \
		./$$tbin || exit 1 ; \
	done

unittest_str: \
../load unittest_str.o ../str.a
	../load unittest_str ../str.a \
	$(CHECK_LIBS)

unittest_str.o: \
../compile unittest_str.c ../str.h
	../compile unittest_str.c -I.. \
	$(CHECK_INCLUDES)

unittest_stralloc: \
../load unittest_stralloc.o ../stralloc.a ../str.a ../error.a
	../load unittest_stralloc ../stralloc.a ../str.a ../error.a \
	$(CHECK_LIBS)

unittest_stralloc.o: \
../compile unittest_stralloc.c ../alloc.h ../stralloc.h
	../compile unittest_stralloc.c -I.. \
	$(CHECK_INCLUDES)
