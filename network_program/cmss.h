/**
 * atm.h
 **/

#ifndef _INCLUDE_ATM_
#define _INCLUDE_ATM_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include <postgresql/libpq-fe.h>   //libpqライブラリを使用するために必要

#define BUFSIZE 2048
#define PORT 10000     //ATMサーバのポート番号
#define ENTER "\n" //<LF>
#define DATA_END ".\n"

//*** プロトコルコマンド ***//
#define ACCOUNT_CREATE    "account_create"  //ログイン
#define GPA_CREATE    "gpa_create"  //個人GPA算出
#define PERSONAL_GRADE   "personal_grade"  //個人成績
#define ALL_GRADE   "allgrade"  //成績一覧
#define SUBJECT_REGIST "subject_regist" //科目登録
#define INPUT_COURSE "input_course" //進路入力
#define PJUDGE "promotion" //卒業着手判定
#define GJUDGE "graduation" //卒業判定
#define CJUDGE "completion" //修了判定
#define GRADUATE_COUNT "graduate_count" //卒業者・留年者

//*** レスポンスステータス ***//
#define OK_STAT    "+OK"     //成功
#define ER_STAT    "-ERR"    //失敗

//*** エラーコード ***//
#define E_CODE_1    100  //指定された口座IDの預金口座が存在しない
#define E_CODE_2    200  //指定された金額が適切でない
#define E_CODE_3    300  //貯金限度額を超えている
#define E_CODE_4    400  //借り入れ金額を下回っている
#define E_CODE_5    500  //リクエストコマンドの引数エラー
#define E_CODE_6    600  //リクエストコマンドが存在しない
#define E_CODE_7    700  //データベースエラー


//*** スレッド関数に渡す引数情報構造体 ***//
typedef struct _ThreadParamter {
  struct in_addr c_ipaddr;  //IPアドレス
  in_port_t      c_port;    //ポート番号
  int            soc;       //コネクション用ソケットディスクリプタ
  PGconn         *con;      //PGconnオブジェクト（データベース）
}ThreadParameter;

typedef struct _UserInfo {
  char id[10];
  char person_name[100];
  int user_level;
  char department[100];
  char major[100];
  int school_year;
}UserInfo;

extern int receive_message(int __s, char *__buf, int __maxlen);
extern void *main_controller(void *__arg);
extern int account_create(pthread_t __selfId, PGconn *__con, int __soc, UserInfo *__User_Info);
extern int gpa_create(pthread_t __selfId, PGconn *__con, int __soc, char *__recvBuf, char *__sendBuf, UserInfo *__User_Info);
extern int personal_grade(pthread_t __selfId, PGconn *__con, int __soc, char *__recvBuf, char *__sendBuf, UserInfo *__User_Info);
extern int all_grade(pthread_t selfId, PGconn *__con, int __soc, char *recvBuf, char *sendBuf, UserInfo *__User_Info);
extern int subject_regist(pthread_t selfId, PGconn *con, int __soc, char *recvBuf, char *sendBuf, UserInfo *__User_Info);
extern int input_course(pthread_t __selfId, PGconn *__con, int __soc, char *__sendBuf, UserInfo *__User_Info);
extern int judge_main(pthread_t __selfId, PGconn *__con, int __soc, UserInfo *__User, int __judgeFlag, char *__recvBuf);
extern int graduate_count(pthread_t __selfId, PGconn *__con, int __soc, char *__recvBuf, char *__sendBuf, UserInfo *__User_Info);
// extern int count(pthread_t selfId, PGconn *con, int soc, char *recvBuf, char *sendBuf);

#endif
