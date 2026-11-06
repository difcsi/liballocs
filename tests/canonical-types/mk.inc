canonical-types: libcanon.so
canonical-types: LDLIBS += -Wl,-rpath,$(shell pwd) libcanon.so
libcanon.so: LDLIBS :=
libcanon.so: CFLAGS += -fPIC
libcanon.so: canon.c nosym.o
	$(CC) -shared -o $@ $+ $(CFLAGS) $(LDFLAGS) $(LDLIBS)
# nosym.o's static loses its symbol, so only DWARF describes it (an extrasym)
nosym.o: nosym.c
	$(CC) -c -o $@ $< $(CFLAGS) && objcopy --strip-symbol=canon_nosym $@
