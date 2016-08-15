CC=gcc
CFLAGS=-Werror -Wall -g
LIBS=-lpcap -pthread
SRCDIR=src
OBJDIR=obj
BINDIR=bin

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	[ -d $(OBJDIR) ] || mkdir $(OBJDIR)
	$(CC) $(CFLAGS) $< -c -o $@

all: $(OBJDIR)/stp.o
	[ -d $(SRCDIR) ] || mkdir $(SRCDIR)
	$(CC) $(CFLAGS) $< $(LIBS) -o $(BINDIR)/switch
