
CC = gcc

buildtype = release

CPPFLAGS += -D _GNU_SOURCE
CPPFLAGS += -I .

CFLAGS += -Wall -Werror
CFLAGS += -Wfatal-errors

ifeq ($(buildtype), release)
CPPFLAGS += -D RELEASE
CPPFLAGS += -D FORTIFY_SOURCE=2

CFLAGS += -O2
CFLAGS += -flto

LDFLAGS += -static
else ifeq ($(buildtype), testing)

CFLAGS += -g
CFLAGS += -Wno-unused-variable
CFLAGS += -Wno-unused-function
CFLAGS += -Wno-unused-but-set-variable

else
CPPFLAGS += -D DEBUGGING

CFLAGS += -g
CFLAGS += -Wno-unused-variable
CFLAGS += -Wno-unused-function
CFLAGS += -Wno-unused-but-set-variable
endif

buildprefix = $(buildtype)

default: gen/$(buildprefix)/lexdiff

#ARGS += -v
ARGS += --dotfile

ARGS += ./test-a.txt ./test-b.txt -f ./test-patternfile

#ARGS += ./examples/A.csv
#ARGS += ./examples/B.csv
#ARGS += -f ./examples/csv-patternfile

run: gen/$(buildprefix)/lexdiff
	$< $(ARGS)

valrun: gen/$(buildprefix)/lexdiff
	valgrind $< $(ARGS)

valrun-stop: gen/$(buildprefix)/lexdiff
	valgrind --gen-suppressions=yes -- $< $(ARGS)

valrun-leak: gen/$(buildprefix)/lexdiff
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -- $< $(ARGS)

.PRECIOUS: %/

%/:
	mkdir -p $@

gen/srclist.mk: | gen/
	find -name '*.c' | sed 's@^./@srcs += @' > $@

include gen/srclist.mk

objs = $(patsubst %.c,gen/$(buildprefix)/%.o,$(srcs))
deps = $(patsubst %.c,gen/$(buildprefix)/%.d,$(srcs))

gen/$(buildprefix)/lexdiff: $(objs)
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

gen/$(buildprefix)/%.d: %.c | gen/$(buildprefix)/%/
	$(CPP) $(CPPFLAGS) $< -MM -MT $@ -MF $@ || (gedit $<; false)

gen/$(buildprefix)/%.o: %.c gen/$(buildprefix)/%.d
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@ || (gedit $<; false)

clean:
	for l in $$(cat .gitignore); do rm -rvf $$l; done

ifneq "$(MAKECMDGOALS)" "clean"
include $(deps)
endif
















