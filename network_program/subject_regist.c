/**
 * subject_regist.c
 *
 * [ARGUMENT]
 *  con       : PGConnオブジェクト
 *  recvBuf   : 受信用配列のポインタ
 *  sendBuf   : 送信用配列のポインタ
 * [RETURN]
 *  Success : 0
 *  Error   : -1
 **/

#include "cmss.h"
//全体のヘッダファイル#include "netbank.h"

int subject_regist(pthread_t selfId, PGconn *con, int soc, char *recvBuf, char *sendBuf, UserInfo *__User_Info){
  PGresult *res, *resSrc, *resDest;
  char sql[BUFSIZE], sqlSrc[BUFSIZE], sqlDest[BUFSIZE];
  int recvLen, sendLen;
  char user_level,subject_name[255],courses_available[5],tmp[255];
  int opening_year,subject_code,course_sharing_code,credit,classification,necessary,i;
  char comm[BUFSIZE], transferText[BUFSIZE];
  int resultRows, n;
  int resultBalanceSrc, resultBalanceDest;
  int idSrc, idDest, transferValue;
  // AccountBank accountSrc, accountDest;
  char *transactionNoSrc, *transactionNoDest;
  int point=25;
  char *sqlBegin="BEGIN", *sqlCommit="COMMIT", *sqlRollback="ROLLBACK";


  /*権限があるのかの確認*/
  if(__User_Info->user_level != 1 && __User_Info->user_level != 2){
    sprintf(sendBuf,"error:閲覧権限がありません%s%s",ENTER,DATA_END);
    send(soc, sendBuf, sendLen, 0);
    printf("[C_THREAD %ld] SEND=> %s\n", selfId, sendBuf);
    return 0;
  }
  sprintf(sendBuf,"科目コードを入力してください%s%s",ENTER,DATA_END);
  sendLen = strlen(sendBuf);
  send(soc, sendBuf, sendLen, 0);
  printf("[C_THREAD %ld] SEND=> %s\n", selfId, sendBuf);
  recvLen = receive_message(soc, recvBuf, BUFSIZE);
  if( recvLen < 1 )
    return -1;
  recvBuf[recvLen-1] = '\0'; // <LF>を消去
  printf("[C_THREAD %ld] RECV=> %s\n", selfId, recvBuf);
  strcpy(tmp,recvBuf);
  subject_code = atoi(tmp);

  sprintf(sendBuf,"開講年度を入力してください%s%s",ENTER,DATA_END);
  sendLen = strlen(sendBuf);
  send(soc, sendBuf, sendLen, 0);
  printf("[C_THREAD %ld] SEND=> %s\n", selfId, sendBuf);
  recvLen = receive_message(soc, recvBuf, BUFSIZE);
  if( recvLen < 1 )
    return -1;
  recvBuf[recvLen-1] = '\0'; // <LF>を消去
  printf("[C_THREAD %ld] RECV=> %s\n", selfId, recvBuf);

  strcpy(tmp,recvBuf);
  opening_year = atoi(tmp);

  sprintf(sendBuf,"科目名を入力してください%s%s",ENTER,DATA_END);
  sendLen = strlen(sendBuf);
  send(soc, sendBuf, sendLen, 0);
  printf("[C_THREAD %ld] SEND=> %s\n", selfId, sendBuf);
  recvLen = receive_message(soc, recvBuf, BUFSIZE);
  if( recvLen < 1 )
    return -1;
  recvBuf[recvLen-1] = '\0'; // <LF>を消去
  printf("[C_THREAD %ld] RECV=> %s\n", selfId, recvBuf);

  strcpy(subject_name,recvBuf);

  sprintf(sendBuf,"科目共有コードを入力してください%s%s",ENTER,DATA_END);
  sendLen = strlen(sendBuf);
  send(soc, sendBuf, sendLen, 0);
  printf("[C_THREAD %ld] SEND=> %s\n", selfId, sendBuf);
  recvLen = receive_message(soc, recvBuf, BUFSIZE);
  if( recvLen < 1 )
    return -1;
  recvBuf[recvLen-1] = '\0'; // <LF>を消去
  printf("[C_THREAD %ld] RECV=> %s\n", selfId, recvBuf);
  strcpy(tmp,recvBuf);
  course_sharing_code = atoi(tmp);

  sprintf(sendBuf,"単位数を入力してください%s%s",ENTER,DATA_END);
  sendLen = strlen(sendBuf);
  send(soc, sendBuf, sendLen, 0);
  printf("[C_THREAD %ld] SEND=> %s\n", selfId, sendBuf);
  recvLen = receive_message(soc, recvBuf, BUFSIZE);
  if( recvLen < 1 )
    return -1;
  recvBuf[recvLen-1] = '\0'; // <LF>を消去
  printf("[C_THREAD %ld] RECV=> %s\n", selfId, recvBuf);
  strcpy(tmp,recvBuf);
  credit = atoi(tmp);

  sprintf(sendBuf,"履修可能学科を入力してください%s%s",ENTER,DATA_END);
  sendLen = strlen(sendBuf);
  send(soc, sendBuf, sendLen, 0);
  printf("[C_THREAD %ld] SEND=> %s\n", selfId, sendBuf);
  recvLen = receive_message(soc, recvBuf, BUFSIZE);
  if( recvLen < 1 )
    return -1;
  recvBuf[recvLen-1] = '\0'; // <LF>を消去
  printf("[C_THREAD %ld] RECV=> %s\n", selfId, recvBuf);
  strcpy(courses_available,recvBuf);

  sprintf(sendBuf,"科目区分を入力してください%s%s",ENTER,DATA_END);
  sendLen = strlen(sendBuf);
  send(soc, sendBuf, sendLen, 0);
  printf("[C_THREAD %ld] SEND=> %s\n", selfId, sendBuf);
  recvLen = receive_message(soc, recvBuf, BUFSIZE);
  if( recvLen < 1 )
    return -1;
  recvBuf[recvLen-1] = '\0'; // <LF>を消去
  printf("[C_THREAD %ld] RECV=> %s\n", selfId, recvBuf);
  strcpy(tmp,recvBuf);
  classification = atoi(tmp);

  sprintf(sendBuf,"必修選択区分を入力してください%s%s",ENTER,DATA_END);
  sendLen = strlen(sendBuf);
  send(soc, sendBuf, sendLen, 0);
  printf("[C_THREAD %ld] SEND=> %s\n", selfId, sendBuf);
  recvLen = receive_message(soc, recvBuf, BUFSIZE);
  if( recvLen < 1 )
    return -1;
  recvBuf[recvLen-1] = '\0'; // <LF>を消去
  printf("[C_THREAD %ld] RECV=> %s\n", selfId, recvBuf);
  strcpy(tmp,recvBuf);
  necessary = atoi(tmp);


  /*n = sscanf(recvBuf, "%d %d %s %d %d %s %d %d", &subject_code,&opening_year,subject_name,&course_sharing_code,&credit, courses_available,&classification, &necessary);

  if(n != 8){
    sprintf(sendBuf,"%s %d%s", ER_STAT, E_CODE_1, ENTER);
    return -1;
    }*/


  /********* ここからIPOのプロセス処理 *********/

  PQexec(con, sqlBegin); //トランザクション開始
  /*権限があるのかの確認
  if(User_Info.user_level != '0' && User_info.user_level != '2'){*/
    //PQexec(con, sqlRollback);  //トランザクション終了（ロールバック）
    /* データベース接続を切断 */
    //printf("error:閲覧権限がありません。\n");
    //return 0;
  //}

  sprintf(sql, "INSERT INTO subject_detail(subject_code,opening_year,subject_name,course_sharing_code,credit,courses_available,classification,necessary) VALUES(%d,%d,'%s',%d,%d,'%s',%d, %d)",subject_code,opening_year,subject_name,course_sharing_code,credit,courses_available,classification,necessary);
  res = PQexec(con, sql);

  if(PQresultStatus(res) != PGRES_COMMAND_OK){
    printf("%s\n", PQresultErrorMessage(res));
    sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_2, ENTER);
    printf("%s", sendBuf);
    PQexec(con, sqlRollback);  //トランザクション終了（ロールバック）
    return -1;
  }
  /*プロトコルレスポンス*/
  resultRows = PQntuples(res);
  if(resultRows < 0){
    sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_3, ENTER);
    printf("%s", sendBuf);
    PQexec(con, sqlRollback);  //トランザクション終了（ロールバック）
    return -1;
  }
  sendLen = sprintf(sendBuf,"%s 科目登録が完了しました%s", OK_STAT, ENTER);


  /*プロトコル・レスポンスを送信*/
  send(soc, sendBuf, sendLen, 0);
  printf("[C_THREAD %ld] SEND=> %s", selfId, sendBuf);
  PQexec(con, sqlCommit);  //トランザクション終了（コミット）

  return 0;
}
