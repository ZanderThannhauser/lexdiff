
CC = gcc

CPPFLAGS += -D _GNU_SOURCE
CPPFLAGS += -I .

CFLAGS += -Wall -Werror -Wfatal-errors

LDLIBS += -lgmp

buildtype ?= release

ifeq ($(buildtype), release)
CPPFLAGS += -D RELEASE

CFLAGS += -O2
CFLAGS += -flto

LDFLAGS += -flto=auto
LDFLAGS += -static

else ifeq ($(buildtype), test)
CPPFLAGS += -D TESTING

CFLAGS += -g
CFLAGS += -Wno-unused-variable
CFLAGS += -Wno-unused-function
CFLAGS += -Wno-unused-but-set-variable
CFLAGS += -Wno-unused-label

else ifeq ($(buildtype), debug)
CPPFLAGS += -D DEBUGGING
CPPFLAGS += -D ZEBU_DEBUG

CFLAGS += -g
CFLAGS += -Wno-unused-variable
CFLAGS += -Wno-unused-function
CFLAGS += -Wno-unused-but-set-variable
CFLAGS += -Wno-unused-label

else
$(error "invalid buildtype!");
endif

on_error ?= do_nothing
ifeq ($(on_error), do_nothing)
ON_ERROR =
else ifeq ($(on_error), open_editor)
ON_ERROR += || ($$EDITOR $<; false)
else
$(error "invalid on_error option!");
endif

buildprefix = bin/$(buildtype)-build
depprefix   = dep/$(buildtype)-build

default: $(buildprefix)/lexdiff

ARGS += -v
#ARGS += -h

ARGS += --pretty-print

#ARGS += --dotout /tmp/tokenizer.dot

#ARGS += ./examples/csv/csv.ldiff ./examples/csv/master.csv ./examples/csv/compare.csv

#ARGS += ./examples/json/json.ldiff ./examples/json/master.json ./examples/json/compare.json

ARGS += ./examples/sandbox/sandbox.ldiff ./examples/sandbox/master.txt ./examples/sandbox/compare.txt

run: $(buildprefix)/lexdiff
	$< $(ARGS)

valrun: $(buildprefix)/lexdiff
	valgrind $< $(ARGS)

valrun-stop: $(buildprefix)/lexdiff
	valgrind --gen-suppressions=yes -- $< ${ARGS}

valrun-leak: $(buildprefix)/lexdiff
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1 -- $< ${ARGS}

tracerun: $(buildprefix)/lexdiff
	strace $< $(ARGS)

install: $(buildprefix)/lexdiff
	@ mkdir -vp ~/bin/
	@ cp -vau $(buildprefix)/lexdiff ~/bin/lexdiff

.PRECIOUS: %/

%/:
	@ mkdir -p $@

srclist.mk:
	@ echo "searching for source files..."
	@ find -name '*.c' -! -path '*/-*' | sort -V | sed 's/^/srcs += /' > $@

ifneq "$(MAKECMDGOALS)" "clean"
include srclist.mk
endif

objs := $(patsubst %.c,$(buildprefix)/%.o,$(srcs))

deps := $(patsubst %.c,$(depprefix)/%.d,$(srcs))

bin/escape: ./-escape.c | bin/
	@ echo "compiling $<"
	@ gcc -Wall -Werror ./$< -o $@

cmdln/usage_message.c cmdln/usage_message.h: bin/escape cmdln/usage_message.txt
	@ echo "escaping $*"
	@ $^ -v usage_message -o ./cmdln/usage_message.c

parse/zebu.c parse/zebu.h: parse/zebu.zb
	zebu -v -m --template=fileio -i $< -o parse/zebu

$(buildprefix)/%.o $(depprefix)/%.d: %.cpp | $(buildprefix)/%/ $(depprefix)/%/
	@ echo "compiling $<"
	@ $(CC) -c $(CPPFLAGS) $(CFLAGS) $< -MMD -o $(buildprefix)/$*.o -MF $(depprefix)/$*.d $(ON_ERROR)

$(buildprefix)/%.o $(depprefix)/%.d: %.c | $(buildprefix)/%/ $(depprefix)/%/
	@ echo "compiling $<"
	@ $(CC) -c $(CPPFLAGS) $(CFLAGS) $< -MMD -o $(buildprefix)/$*.o -MF $(depprefix)/$*.d $(ON_ERROR)

$(buildprefix)/lexdiff: $(objs)
	@ echo "linking $@"
	@ $(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

clean:
	for l in $$(cat .gitignore); do rm -rvf $$l; done

ifneq "$(MAKECMDGOALS)" "clean"
include $(deps)
endif
















