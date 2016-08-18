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
	[ -d $(BINDIR) ] || mkdir $(BINDIR)
	$(CC) $(CFLAGS) $< $(LIBS) -o $(BINDIR)/switch

.PHONY:
clean:
	rm -r bin obj
