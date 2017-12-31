it:	all

PROGS	:=	scgi2argv
CFLAGS	+=	-Wall -pedantic -s -Os

all:	$(PROGS)
clean:
	rm -f $(PROGS)
