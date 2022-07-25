
#include "cmss.h"

int graduate_count(pthread_t __selfId, PGconn *__con, int __soc, char *__recvBuf, char *__sendBuf, UserInfo *__User_Info){
  PGresult *res;
  char sql[BUFSIZE];
  int recvLen, sendLen;
  char comm[BUFSIZE];
  int resultRows, n;
  int point=25;
  char *sqlBegin="BEGIN", *sqlCommit="COMMIT", *sqlRollback="ROLLBACK";

  /* __User_Info->user_level = 2;
  if(__User_Info->user_level != 2){
    printf("権限がありません。\n");
    return 0;
    }*/
  /* SELECTのSQL文を作成 */
  //PQexec(__con, sqlBegin); //トランザクション開始
  sprintf(sql, "SELECT * FROM grade_judge where graduation_judge=0;");
  res = PQexec(__con, sql);
  if(PQresultStatus(res) != PGRES_TUPLES_OK){
    printf("%s", PQresultErrorMessage(res));
    PQfinish(__con);
    exit(1);
  }
  resultRows = PQntuples(res);
  sendLen=sprintf(__sendBuf,"卒業者数：%d\n",resultRows);
  send(__soc,__sendBuf,sendLen,0);
  printf("[C_THREAD %ld] SEND=> %s", __selfId, __sendBuf);

  sprintf(sql, "SELECT * FROM grade_judge where graduation_judge=2;");
  res = PQexec(__con, sql);
  if(PQresultStatus(res) != PGRES_TUPLES_OK){
    printf("%s", PQresultErrorMessage(res));
    PQfinish(__con);
    exit(1);
  }
  resultRows = PQntuples(res);
  sendLen=sprintf(__sendBuf,"学部保留者数：%d\n",resultRows);
  send(__soc,__sendBuf,sendLen,0);
  printf("[C_THREAD %ld] SEND=> %s", __selfId, __sendBuf);

  sprintf(sql, "SELECT * FROM grade_judge where graduation_judge=1 or research_judge=1;");
  res = PQexec(__con, sql);
  if(PQresultStatus(res) != PGRES_TUPLES_OK){
    printf("%s", PQresultErrorMessage(res));
    PQfinish(__con);
    exit(1);
  }
  resultRows = PQntuples(res);
  sendLen=sprintf(__sendBuf,"学部留年者数：%d\n",resultRows);
  send(__soc,__sendBuf,sendLen,0);
  printf("[C_THREAD %ld] SEND=> %s", __selfId, __sendBuf);

  sprintf(sql, "SELECT * FROM grade_judge where completion_judge=0;");
  res = PQexec(__con, sql);
  if(PQresultStatus(res) != PGRES_TUPLES_OK){
    printf("%s", PQresultErrorMessage(res));
    PQfinish(__con);
    exit(1);
  }
  resultRows = PQntuples(res);
  sendLen=sprintf(__sendBuf,"修了者数：%d\n",resultRows);
  send(__soc,__sendBuf,sendLen,0);
  printf("[C_THREAD %ld] SEND=> %s", __selfId, __sendBuf);

  sprintf(sql, "SELECT * FROM grade_judge where completion_judge=2;");
  res = PQexec(__con, sql);
  if(PQresultStatus(res) != PGRES_TUPLES_OK){
    printf("%s", PQresultErrorMessage(res));
    PQfinish(__con);
    exit(1);
  }
  resultRows = PQntuples(res);
  sendLen=sprintf(__sendBuf,"修士保留者数：%d\n",resultRows);
  send(__soc,__sendBuf,sendLen,0);
  printf("[C_THREAD %ld] SEND=> %s", __selfId, __sendBuf);

  sprintf(sql, "SELECT * FROM grade_judge where completion_judge=1;");
  res = PQexec(__con, sql);
  if(PQresultStatus(res) != PGRES_TUPLES_OK){
    printf("%s", PQresultErrorMessage(res));
    PQfinish(__con);
    exit(1);
  }
  resultRows = PQntuples(res);
  sendLen=sprintf(__sendBuf,"修士留年者数：%d\n",resultRows);
  send(__soc,__sendBuf,sendLen,0);
  printf("[C_THREAD %ld] SEND=> %s", __selfId, __sendBuf);
  //PQexec(__con, sqlCommit);  //トランザクション終了（コミット）

  return 0;
}
