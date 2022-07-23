/**
 *    gcc -o work_main work_main->c -lpq
 **/
#include "cmss.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <postgresql/libpq-fe.h>

int input_course(pthread_t __selfId, PGconn *__con, int __soc, char *__sendBuf, UserInfo *__User_Info){
  PGresult *res;
  char id[10]; //生徒の学籍番号
  char proceed[100];
  char place[100];
  char route_id[10];
  char industry_id[10];
  char occupation_id[10];
  char connInfo[BUFSIZE];
  char sql[BUFSIZE];
  char recvBuf[BUFSIZE], sendBuf[BUFSIZE], comm[BUFSIZE],result[BUFSIZE];
  int sendLen,recvLen,i,j,n,point;
  int id_num = 8;
  char *sqlBegin="BEGIN", *sqlCommit="COMMIT", *sqlRollback="ROLLBACK";

  /* tableチェック */
  sprintf(sql, "SELECT route, route_industry, route_occupation_id, Employment_name, Employment_subject FROM work WHERE id = %s", __User_Info->id);
  printf("[%s]\n", sql);
  res = PQexec(__con, sql);
  if(PQresultStatus(res) != PGRES_TUPLES_OK){
    printf("%s\n", PQresultErrorMessage(res));
    sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_7, ENTER);
    printf("%s", sendBuf);
    return -1;
  }

  /*
  printf("あなたのユーザレベルを入力してください\n");
  scanf("%d", &__User_Info->user_level);
  printf("%d\n",__User_Info->user_level);

  for(i=0; i<8; i++){
    id[i] = __User_Info->id[i];
  }
  */

  if(__User_Info->user_level != 1){
    if(__User_Info->user_level != 0 && __User_Info->user_level != 7 && __User_Info->user_level != 8){
      printf("error:権限がありません。\n");
      return 0;
    }

    sprintf(sendBuf,"進路を変更したい学籍番号を入力してください\n");
    sendLen = strlen(sendBuf);
    send(__soc, sendBuf, sendLen, 0);
    printf("[C_THREAD %ld] SEND=> %s\n", __selfId, sendBuf);

    /* プロトコル・コマンド受信 */
    recvLen = receive_message(__soc, recvBuf, BUFSIZE);
    if( recvLen < 1 )
      return -1;
    recvBuf[recvLen-1] = '\0'; // <LF>を消去
    printf("[C_THREAD %ld] RECV=> %s\n", __selfId, recvBuf);

    /* プロトコル・コマンド解析 */
    //sscanf(recvBuf, "%s", comm);
    //scanf("%s", id);
    for(i=0; i<recvLen-1; i++){
        id[0] = recvBuf[0];
    }

    /* tableチェック */
    sprintf(sql, "SELECT * FROM users WHERE id = %s", id);
    printf("[%s]\n", sql);
    res = PQexec(__con, sql);
    if(PQresultStatus(res) != PGRES_TUPLES_OK){
      printf("%s\n", PQresultErrorMessage(res));
      sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_7, ENTER);
      printf("%s", sendBuf);
      return -1;
    }
  }

  if(__User_Info->user_level == 1){
    for(i=0;i<id_num; i++){
        id[i] = __User_Info->id[i];
      }
  }

  //printf("進学する場合は0を表示してください\n就職する場合は1を入力してください\n");
  //scanf("%d", &point);

  sprintf(sendBuf,"変更したい進学・就職先を入力してください\n");
    sendLen = strlen(sendBuf);
    send(__soc, sendBuf, sendLen, 0);
    printf("[C_THREAD %ld] SEND=> %s\n", __selfId, sendBuf);
  //scanf("%s", proceed);
  /* プロトコル・コマンド受信 */
    recvLen = receive_message(__soc, recvBuf, BUFSIZE);
    if( recvLen < 1 )
      return -1;
    recvBuf[recvLen-1] = '\0'; // <LF>を消去
    printf("[C_THREAD %ld] RECV=> %s\n", __selfId, recvBuf);
    for(i=0; i<recvLen-1; i++){
        proceed[0] = recvBuf[0];
    }

  sprintf(sendBuf,"変更したい進学・就職先所在地県名を入力してください\n");
    sendLen = strlen(sendBuf);
    send(__soc, sendBuf, sendLen, 0);
    printf("[C_THREAD %ld] SEND=> %s\n", __selfId, sendBuf);
  //scanf("%s", place);
  recvLen = receive_message(__soc, recvBuf, BUFSIZE);
    if( recvLen < 1 )
      return -1;
    recvBuf[recvLen-1] = '\0'; // <LF>を消去
    printf("[C_THREAD %ld] RECV=> %s\n", __selfId, recvBuf);
    for(i=0; i<recvLen-1; i++){
        place[0] = recvBuf[0];
    }

  if(__User_Info->user_level == '0' || __User_Info->user_level == '7' || __User_Info->user_level == '8'){
    sprintf(sendBuf,"進路区分、産業区分、職種を入力してください\n");
    sendLen = strlen(sendBuf);
    send(__soc, sendBuf, sendLen, 0);
    printf("[C_THREAD %ld] SEND=> %s\n", __selfId, sendBuf);

    sprintf(sendBuf,"入力しない場合、0を入力してください\n");
    sendLen = strlen(sendBuf);
    send(__soc, sendBuf, sendLen, 0);
    printf("[C_THREAD %ld] SEND=> %s\n", __selfId, sendBuf);

    sprintf(sendBuf,"進路区分:");
    sendLen = strlen(sendBuf);
    send(__soc, sendBuf, sendLen, 0);
    printf("[C_THREAD %ld] SEND=> %s\n", __selfId, sendBuf);
    //scanf("%s", route_id);
    recvLen = receive_message(__soc, recvBuf, BUFSIZE);
    if( recvLen < 1 )
      return -1;
    recvBuf[recvLen-1] = '\0'; // <LF>を消去
    printf("[C_THREAD %ld] RECV=> %s\n", __selfId, recvBuf);
    for(i=0; i<recvLen-1; i++){
        route_id[0] = recvBuf[0];
    }
    if(route_id[0] != '0' || route_id[0] != '\0'){
    /* tableチェック */
    sprintf(sql, "SELECT * FROM route WHERE route_id = %s", route_id);
    printf("[%s]\n", sql);
    res = PQexec(__con, sql);
    if(PQresultStatus(res) != PGRES_TUPLES_OK){
      printf("%s\n", PQresultErrorMessage(res));
      sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_7, ENTER);
      printf("%s", sendBuf);
      return -1;
    }
    }

    sprintf(sendBuf,"産業区分:");
    sendLen = strlen(sendBuf);
    send(__soc, sendBuf, sendLen, 0);
    printf("[C_THREAD %ld] SEND=> %s\n", __selfId, sendBuf);
    //scanf("%s", industry_id);
    recvLen = receive_message(__soc, recvBuf, BUFSIZE);
    if( recvLen < 1 )
      return -1;
    recvBuf[recvLen-1] = '\0'; // <LF>を消去
    printf("[C_THREAD %ld] RECV=> %s\n", __selfId, recvBuf);
    for(i=0; i<recvLen-1; i++){
        industry_id[0] = recvBuf[0];
    }
    if(industry_id[0] != '0' || industry_id[0] != '\0'){
    /* tableチェック */
    sprintf(sql, "SELECT * FROM route_industry WHERE industry_id = %s", industry_id);
    printf("[%s]\n", sql);
    res = PQexec(__con, sql);
    if(PQresultStatus(res) != PGRES_TUPLES_OK){
      printf("%s\n", PQresultErrorMessage(res));
      sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_7, ENTER);
      printf("%s", sendBuf);
      return -1;
    }
    }

    sprintf(sendBuf,"職種:");
    sendLen = strlen(sendBuf);
    send(__soc, sendBuf, sendLen, 0);
    printf("[C_THREAD %ld] SEND=> %s\n", __selfId, sendBuf);
    //scanf("%s", occupation_id);
    recvLen = receive_message(__soc, recvBuf, BUFSIZE);
    if( recvLen < 1 )
      return -1;
    recvBuf[recvLen-1] = '\0'; // <LF>を消去
    printf("[C_THREAD %ld] RECV=> %s\n", __selfId, recvBuf);
    for(i=0; i<recvLen-1; i++){
        occupation_id[0] = recvBuf[0];
    }
  }
  if(occupation_id[0] != '0' || occupation_id[0] != '\0'){
  /* tableチェック */
    sprintf(sql, "SELECT * FROM route_occupation WHERE occupation_id = %s", occupation_id);
    printf("[%s]\n", sql);
    res = PQexec(__con, sql);
    if(PQresultStatus(res) != PGRES_TUPLES_OK){
      printf("%s\n", PQresultErrorMessage(res));
      sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_7, ENTER);
      printf("%s", sendBuf);
      return -1;
    }
  }

  PQexec(__con, sqlBegin); //トランザクション開始*/

  /* 更新 */
  if(route_id[0] != '0' || route_id[0] != '\0'){
    sprintf(sql,"UPDATE work SET route_id=%s WHERE id = %s", route_id,id);
    printf("[%s]\n", sql);

    res = PQexec(__con, sql);

    if(PQresultStatus(res) != PGRES_COMMAND_OK){
      printf("%s", PQresultErrorMessage(res));
      sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_7, ENTER);
      printf("%s", sendBuf);
      PQexec(__con, sqlRollback);
      return -1;
    }

    if(atoi(PQcmdTuples(res)) != 1){
      sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_7, ENTER);
      printf("%s", sendBuf);
      PQexec(__con, sqlRollback);
      return -1;
    }
  }

  if(industry_id[0] != '0' || industry_id[0] != '\0'){
    sprintf(sql,"UPDATE work SET route_id=%s WHERE id = %s", industry_id,id);
    printf("[%s]\n", sql);

    res = PQexec(__con, sql);

    if(PQresultStatus(res) != PGRES_COMMAND_OK){
      printf("%s", PQresultErrorMessage(res));
      sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_7, ENTER);
      printf("%s", sendBuf);
      PQexec(__con, sqlRollback);
      return -1;
    }

    if(atoi(PQcmdTuples(res)) != 1){
      sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_7, ENTER);
      printf("%s", sendBuf);
      PQexec(__con, sqlRollback);
      return -1;
    }
  }

  if(occupation_id[0] != '0' || occupation_id[0] != '\0'){
    sprintf(sql,"UPDATE work SET route_id=%s WHERE id = %s", occupation_id,id);
    printf("[%s]\n", sql);

    res = PQexec(__con, sql);

    if(PQresultStatus(res) != PGRES_COMMAND_OK){
      printf("%s", PQresultErrorMessage(res));
      sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_7, ENTER);
      printf("%s", sendBuf);
      PQexec(__con, sqlRollback);
      return -1;
    }

    if(atoi(PQcmdTuples(res)) != 1){
      sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_7, ENTER);
      printf("%s", sendBuf);
      PQexec(__con, sqlRollback);
      return -1;
    }
  }


    sprintf(sql,"UPDATE work SET Employment_name=%s WHERE id = %s", proceed,id);
    printf("[%s]\n", sql);

    res = PQexec(__con, sql);

    if(PQresultStatus(res) != PGRES_COMMAND_OK){
      printf("%s", PQresultErrorMessage(res));
      sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_7, ENTER);
      printf("%s", sendBuf);
      PQexec(__con, sqlRollback);
      return -1;
    }

    if(atoi(PQcmdTuples(res)) != 1){
      sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_7, ENTER);
      printf("%s", sendBuf);
      PQexec(__con, sqlRollback);
      return -1;
    }



    sprintf(sql,"UPDATE work SET Employment_subject=%s WHERE id = %s", place,id);
    printf("[%s]\n", sql);

    res = PQexec(__con, sql);

    if(PQresultStatus(res) != PGRES_COMMAND_OK){
      printf("%s", PQresultErrorMessage(res));
      sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_7, ENTER);
      printf("%s", sendBuf);
      PQexec(__con, sqlRollback);
      return -1;
    }

    if(atoi(PQcmdTuples(res)) != 1){
      sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_7, ENTER);
      printf("%s", sendBuf);
      PQexec(__con, sqlRollback);
      return -1;
    }


  /* プロトコル・レスポンス */
  /*sprintf(sendBuf, "%s %s %s %s%s", OK_STAT
	 ,id
	 ,proceed
	 ,place,ENTER);
  printf("%s", sendBuf);*/

    //結果を配列に格納
    for(i=0; i<id_num; i++){
        result[i] = id[i];
    }

        result[i] = ' ';
        i++;
        n = strlen(proceed);
        for(j=0; j<n; j++){
            result[i] = proceed[n];
            i++;
        }


        result[i] = ' ';
        i++;
        n = strlen(place);
        for(j=0; j<n; j++){
            result[i] = place[n];
            i++;
        }

    if(route_id[0] != '0' || route_id[0] != '\0'){
        result[i] = ' ';
        i++;
        n = strlen(route_id);
        for(j=0; j<n; j++){
            result[i] = route_id[n];
            i++;
        }
    }
    if(industry_id[0] != '0' || industry_id[0] != '\0'){
        result[i] = ' ';
        i++;
        n = strlen(industry_id);
        for(j=0; j<n; j++){
            result[i] = industry_id[n];
            i++;
        }
    }
    if(occupation_id[0] != '0' || occupation_id[0] != '\0'){
        result[i] = ' ';
        i++;
        n = strlen(occupation_id);
        for(j=0; j<n; j++){
            result[i] = occupation_id[n];
            i++;
        }
    }

    //結果の送信
   sprintf(sendBuf,"%s %s%s", OK_STAT,result,ENTER);
    sendLen = strlen(sendBuf);
    send(__soc, sendBuf, sendLen, 0);
    printf("[C_THREAD %ld] SEND=> %s\n", __selfId, sendBuf);

  PQexec(__con, sqlCommit);  //トランザクション終了（コミット）

  /* データベース接続を切断 */
  //PQfinish(__con);

  return 0;
}
