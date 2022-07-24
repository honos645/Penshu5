##
## cmss_server Makefile
##

CC	= gcc

LIBS	= -lpq -lpthread

SRCS	 = $(MAIN_SRC) $(USAMI) $(YANO) $(IKEIDO) $(KAMADA) $(AMAKI) $(TAIKI) $(KITAZAWA)

MAIN_SRC	= setup_listen.c pmss_server_main.c main_controller.c common_func.c

IKEIDO	= login.c

# KITAZAWA = gpa_create.c

TAIKI	= all_grade.c

# AMAKI	=subject_regist.c

KAMADA	= input_course.c

USAMI	= judge.c

# YANO	= count.c

HDRS	= $(MAIN_HDR) $(HDR)

MAIN_HDR	= cmss.h

HDR	=  judge_head.h all_grade_head.h #gpa_create_head.h

OBJS	= $(SRCS:.c=.o)

PROGRAM	= pmss

all:	$(PROGRAM)

$(PROGRAM):	$(OBJS) $(HDRS)
		$(CC) $(OBJS) $(LIBS) -o $(PROGRAM)

clean:
		rm -f *.o *~ $(PROGRAM)

##