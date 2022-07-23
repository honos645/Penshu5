// #include "count.h"
#include"csmm.h"

// typedef struct _UserInfo {
//     char id[BUFSIZE];
//     int user_level;
//   }UserInfo;

int count(pthread_t selfId, PGconn *con, int soc, char *recvBuf, char *sendBuf){
  PGresult *res;
  char sql[BUFSIZE];
  int recvLen, sendLen;
  char comm[BUFSIZE];
  int resultRows, n;
  int point=25;
  char *sqlBegin="BEGIN", *sqlCommit="COMMIT", *sqlRollback="ROLLBACK";


    if(UserInfo.user_level != 2){
      printf("権限がありません。\n");
      return 0;
    }
  /* SELECTのSQL文を作成 */
  PQexec(con, sqlBegin); //トランザクション開始
  sprintf(sql, "SELECT * FROM grade_judge where graduation_judge=0;");
  res = PQexec(con, sql);
  if(PQresultStatus(res) != PGRES_TUPLES_OK){
    printf("%s", PQresultErrorMessage(res));
    PQfinish(con);
    exit(1);
  }
  resultRows = PQntuples(res);
  sendLen=sprintf(sendBuf,"卒業者数：%d\n",resultRows);
  send(soc,sendBuf,sendLen,0);
  printf("[C_THREAD %ld] SEND=> %s", selfId, sendBuf);

  sprintf(sql, "SELECT * FROM grade_judge where graduation_judge=2;");
  res = PQexec(con, sql);
  if(PQresultStatus(res) != PGRES_TUPLES_OK){
    printf("%s", PQresultErrorMessage(res));
    PQfinish(con);
    exit(1);
  }
  resultRows = PQntuples(res);
  sendLen=sprintf(sendBuf,"学部保留者数：%d\n",resultRows);
  send(soc,sendBuf,sendLen,0);
  printf("[C_THREAD %ld] SEND=> %s", selfId, sendBuf);

  sprintf(sql, "SELECT * FROM grade_judge where graduation_judge=1 or research_judge=1;");
  res = PQexec(con, sql);
  if(PQresultStatus(res) != PGRES_TUPLES_OK){
    printf("%s", PQresultErrorMessage(res));
    PQfinish(con);
    exit(1);
  }
  resultRows = PQntuples(res);
  sendLen=sprintf(sendBuf,"学部留年者数：%d\n",resultRows);
  send(soc,sendBuf,sendLen,0);
  printf("[C_THREAD %ld] SEND=> %s", selfId, sendBuf);

  sprintf(sql, "SELECT * FROM grade_judge where completion_judge=0;");
  res = PQexec(con, sql);
  if(PQresultStatus(res) != PGRES_TUPLES_OK){
    printf("%s", PQresultErrorMessage(res));
    PQfinish(con);
    exit(1);
  }
  resultRows = PQntuples(res);
  sendLen=sprintf(sendBuf,"修了者数：%d\n",resultRows);
  send(soc,sendBuf,sendLen,0);
  printf("[C_THREAD %ld] SEND=> %s", selfId, sendBuf);

  sprintf(sql, "SELECT * FROM grade_judge where completion_judge=2;");
  res = PQexec(con, sql);
  if(PQresultStatus(res) != PGRES_TUPLES_OK){
    printf("%s", PQresultErrorMessage(res));
    PQfinish(con);
    exit(1);
  }
  resultRows = PQntuples(res);
  sendLen=sprintf(sendBuf,"修士保留者数：%d\n",resultRows);
  send(soc,sendBuf,sendLen,0);
  printf("[C_THREAD %ld] SEND=> %s", selfId, sendBuf);

  sprintf(sql, "SELECT * FROM grade_judge where completion_judge=1;");
  res = PQexec(con, sql);
  if(PQresultStatus(res) != PGRES_TUPLES_OK){
    printf("%s", PQresultErrorMessage(res));
    PQfinish(con);
    exit(1);
  }
  resultRows = PQntuples(res);
  sendLen=sprintf(sendBuf,"修士留年者数：%d\n",resultRows);
  send(soc,sendBuf,sendLen,0);
  printf("[C_THREAD %ld] SEND=> %s", selfId, sendBuf);
  PQexec(con, sqlCommit);  //トランザクション終了（コミット）

  return 0;
}
