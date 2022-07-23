#ifndef __JUDGE_
#define __JUDGE_

#include"cmss.h"

//*** 判定 ***//
#define PASSING 0
#define FAILURE 1
#define PENDING 2

//*** ユーザステータス ***//
#define ADMIN 0
#define STUDENT 1
#define STAFF 2
#define KYOMU 3
#define TEACHER 4
#define CLASSTEACHER 5
#define SUBTEACHER 6
#define CLASSEMPLOY 7
#define EMPLOY 8
#define DEPCHAIR 9

extern int judge_personal(pthread_t __selfId, PGconn *__con, int __soc, char *__studentNum, int __judgeFlag);
extern int judge_list(pthread_t __selfId, PGconn *__con, int __soc, UserInfo *__User, int __judgeFlag);

#endif