dlopen-statics: libdlstat.so
dlopen-statics: CFLAGS += -DLIBDLSTAT_PATH='"$(shell pwd)/libdlstat.so"'
dlopen-statics: LDLIBS += -ldl
libdlstat.so: LDLIBS :=
libdlstat.so: CFLAGS += -fPIC
libdlstat.so: dlstat.c
	$(CC) -shared -o $@ $+ $(CFLAGS) $(LDFLAGS) $(LDLIBS)
