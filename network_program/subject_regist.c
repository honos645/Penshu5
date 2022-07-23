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

int subject_regist(pthread_t selfId, PGconn *con, int soc, char *recvBuf, char *sendBuf){
  PGresult *res, *resSrc, *resDest;
  char sql[BUFSIZE], sqlSrc[BUFSIZE], sqlDest[BUFSIZE];
  int recvLen, sendLen;
  char user_level,subject_name[255],courses_available[5];
  int opening_year,subject_code,course_sharing_code,credit,classification,necessary,i;
  char comm[BUFSIZE], transferText[BUFSIZE];
  int resultRows, n;
  int resultBalanceSrc, resultBalanceDest;
  int idSrc, idDest, transferValue;
  // AccountBank accountSrc, accountDest;
  char *transactionNoSrc, *transactionNoDest;
  int point=25;
  char *sqlBegin="BEGIN", *sqlCommit="COMMIT", *sqlRollback="ROLLBACK";

  /*n = sscanf(recvBuf, "%s %d %d %d %s",
    comm, &idSrc, &idDest, &transferValue, transferText);*/
  n = sscanf(recvBuf, "%s %d %d %s %d %d %s %d %d", comm, &subject_code,&opening_year,subject_name,&course_sharing_code,&credit, courses_available,&classification, &necessary);

  if(n != 9){
    sprintf(sendBuf,"%s %d%s", ER_STAT, E_CODE_1, ENTER);
    return -1;
  }

  /*スキーマ検索パスを変更 */
  //sprintf(sql, "SET search_path to netbank");
  //PQexec(con, sql);


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

  /*
  for(i=0; i<resultRows; i++){
    subject_code = atoi(PQgetvalue(res, i, 0)); //0 番目のフィールド値を取得
    opening_year = atoi(PQgetvalue(res, i, 1)); //1 番目のフィールド値を取得
    subject_name = PQgetvalue(res, i, 2); //2 番目のフィールド値を取得
    course_sharing_code = atoi(PQgetvalue(res, i, 3)); //3 番目のフィールド値を取得
    credit = atoi(PQgetvalue(res, i, 4)); //4 番目のフィールド値を取得
    courses_available = PQgetvalue(res, i, 5); //5 番目のフィールド値を取得
    classification = atoi(PQgetvalue(res, i, 6)); //6 番目のフィールド値を取得
    printf("%d %d %s %d %d %s %d\n",  subject_code,opening_year,subject_name,course_sharing_code,credit, courses_available,classification);
    }*/
  /*プロトコル・レスポンスを送信*/
  send(soc, sendBuf, sendLen, 0);
  printf("[C_THREAD %ld] SEND=> %s", selfId, sendBuf);
  PQexec(con, sqlCommit);  //トランザクション終了（コミット）

  return 0;
}
