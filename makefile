
CC = gcc

buildtype = release

CPPFLAGS += -D _GNU_SOURCE
CPPFLAGS += -I .

CFLAGS += -Wall -Werror

ifeq ($(buildtype), release)
CPPFLAGS += -D RELEASE
CPPFLAGS += -D FORTIFY_SOURCE=2

CFLAGS += -O2
CFLAGS += -flto
else
CPPFLAGS += -D DEBUGGING

CFLAGS += -g
CFLAGS += -Wno-unused-variable
CFLAGS += -Wno-unused-function
CFLAGS += -Wno-unused-but-set-variable
endif

default: gen/$(buildtype)/ldiff

install: ~/bin/ldiff

~/bin/ldiff: gen/release/ldiff
	install -D $< $@

ARGS += ./test-a.txt ./test-b.txt -f ./test-patternfile

#ARGS += ./examples/A.csv
#ARGS += ./examples/B.csv
#ARGS += -f ./examples/csv-patternfile

run: gen/$(buildtype)/ldiff
	$< $(ARGS)

valrun: gen/$(buildtype)/ldiff
	valgrind $< $(ARGS)

%/:
	mkdir -p $@

gen/srclist.mk: | gen/
	find -name '*.c' | sed 's/^/srcs += /' > $@

include gen/srclist.mk

objs = $(patsubst %.c,gen/$(buildtype)/%.o,$(srcs))
deps = $(patsubst %.c,gen/$(buildtype)/%.d,$(srcs))

gen/$(buildtype)/ldiff: $(objs)
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

gen/$(buildtype)/%.d: %.c | gen/$(buildtype)/%/
	$(CPP) $(CPPFLAGS) $< -MM -MT $@ -MF $@ || (gedit $<; false)

gen/$(buildtype)/%.o: %.c gen/$(buildtype)/%.d | gen/$(buildtype)/%/
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@ || (gedit $<; false)

clean:
	for l in $$(cat .gitignore); do rm -rvf $$l; done

ifneq "$(MAKECMDGOALS)" "clean"
include $(deps)
endif
















