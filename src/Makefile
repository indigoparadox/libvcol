
# vim: ft=make noexpandtab

INCDIRS=
CFLAGS=-Wall -DDEBUG $(INCDIRS)
SOURCES=bstrglue.c goki.c hashmap.c vector.c mem.c bstrlib/bstrlib.c
OBJECTS=$(subst .c,.o,$(SOURCES))
OBJDIR=../obj/Debug-Linux

MD=mkdir -p -v
AR=ar

all: ../bin/Debug-Linux/libvcol.a

../bin/Debug-Linux/libvcol.a: $(OBJECTS)
	$(MD) ../bin/Debug-Linux
	$(AR) rcs $@ $(addprefix $(OBJDIR)/,$^)

%.o: %.c
	$(MD) $(OBJDIR)/$(dir $@)
	$(CC) -c -o $(OBJDIR)/$@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -rf ../obj; \
	rm -rf ../bin

