CC?=gcc
CFLAGS?=-O3 -Wall -fPIC -ggdb
CCLD?=gcc
LDFLAGS?=-pie
LIBS?= 
PREFIX?=/usr
BINDIR?=$(PREFIX)/bin
EXENAME=./color-inverter
SRCDIR=src
OBJDIR=obj
SRCS=$(wildcard $(SRCDIR)/*.c)
OBJS=$(patsubst %.c,%.o,$(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SRCS)))

RM=rm
RM_FORCE_FLAG=-f
TOUCH=touch
ECHO=echo
INSTALL=install
EXEC=exec
EXEC_ARGS?='--help'

.PHONY: all
all: $(EXENAME)

$(EXENAME): $(OBJS)
	@$(ECHO) "CCLD	$(EXENAME)"
	@$(CCLD) $(LDFLAGS) -o $(EXENAME) $(OBJS) $(LIBS)

$(OBJS): $(SRCS)
	@$(ECHO) "CC	$@"
	@$(CC) -c $(CFLAGS) -o $@ $^

update:
	@$(ECHO) "TOUCH	$(SRCS)"
	@$(TOUCH) $(SRCS)

clean: $(OBJS) $(EXENAME)
	@$(ECHO) "RM	$(OBJS)"
	@$(RM) $(RM_FORCE_FLAG) $(OBJS)

install: $(EXENAME)
	@$(ECHO) "INSTALL	$(EXENAME)"
	@$(INSTALL) $(EXENAME) $(BINDIR) 

debug:
	@$(MAKE) update
	@CFLAGS="$(CFLAGS) -DDEBUG" $(MAKE) all

run: $(EXENAME)
	@$(ECHO) "EXEC	$(EXENAME)"
	@$(EXEC) $(EXENAME) $(EXEC_ARGS)
