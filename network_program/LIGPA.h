/**
 *  LIGPA_func.h
 **/

#ifndef _INCLUDE_LIGPA_FUNC_
#define _INCLUDE_LIGPA_FUNC_


#define GPA "GPA" //GPA
#define ID "ID" //ID
#define EXAM_GPA "EXAM_GPA" //
#define EXAM_ID "EXAM_ID" //

extern int LIGPA(pthread_t __selfId, PGconn *__con, int __soc, char *__recvBuf, char *__sendBuf);
#endif
