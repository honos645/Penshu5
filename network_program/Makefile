##
## cmss_server Makefile
##

CC	= gcc

LIBS	= -lpq -lpthread

SRCS	 = $(MAIN_SRC) $(USAMI) $(YANO) $(IKEIDO) $(KAMADA) #$(KITAZAWA)

MAIN_SRC	= setup_listen.c pmss_server_main.c main_controller.c

USAMI	= judge.c

# YANO	= count.c

IKEIDO	= login.c

KAMADA	= input_course.c

# KITAZAWA = gpa_create.c personal_grade.c

HDRS	= $(MAIN_HDR) $(HDR)

MAIN_HDR	= cmss.h

HDR	= common_func.c judge.h

OBJS	= $(SRCS:.c=.o)

PROGRAM	= pmss

all:	$(PROGRAM)

$(PROGRAM):	$(OBJS) $(HDRS)
		$(CC) $(OBJS) $(LIBS) -o $(PROGRAM)

clean:
		rm -f *.o *~ $(PROGRAM)

##