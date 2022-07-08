##
## PMSS (Promotion Management Support System) Makefile
## autor: 67200072 usami
##

CC	= gcc

CFLAG	= -lpq -g -lpthread

TARGET	= PMSS

SRCS	= 

HDRS	=

OBJS	= $(SRCS:.c=.o)

$(TARGET): $(OBJS)
		$(CC) -o $(TARGET) $(OBJS) $(CFLAG)

$(OBJS): $(SRCS) $(HDRS)
		$(CC) -c $(SRCS)

all:
		clean $(OBJS) $(TARGET)

clean:
		rm -f $(OBJS) $(TARGET) *.d *~
