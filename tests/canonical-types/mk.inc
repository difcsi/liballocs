canonical-types: libcanon.so
canonical-types: LDLIBS += -Wl,-rpath,$(shell pwd) libcanon.so
libcanon.so: LDLIBS :=
libcanon.so: CFLAGS += -fPIC
libcanon.so: canon.c
	$(CC) -shared -o $@ $+ $(CFLAGS) $(LDFLAGS) $(LDLIBS)
