#ifndef __JUDGE_
#define __JUDGE_

#include"cmss.h"
//*** コマンドフラグ ***//
#define PROMOTION 0
#define GRADUATION 1
#define COMPLETION 2

//*** 判定 ***//
#define PASSING 0
#define FAILURE 1
#define PENDING 2
#define NODATA -1

extern int judge_personal(pthread_t __selfId, PGconn *__con, int __soc, char *__studentNum, int __judgeFlag, char *__errorBuf);
extern int judge_list(pthread_t __selfId, PGconn *__con, int __soc, UserInfo *__User, char *__recvBuf, int __judgeFlag, char *__errorBuf);

#endif