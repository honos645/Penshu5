##
## test_protocol Makefile
##
CC		= gcc
LIBS		= 
SRCS		= test_protocol.c setup_connect.c receive_util.c
HDRS            = test_protocol.h
OBJS		= $(SRCS:.c=.o)
PROGRAM		= test_protocol

all:		$(PROGRAM)

$(PROGRAM):	$(OBJS) $(HDRS) 
		$(CC) $(OBJS) $(LIBS) -o $(PROGRAM)

clean:
		rm -f *.o *~ $(PROGRAM)

###
