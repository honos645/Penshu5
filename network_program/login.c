/**
*    gcc -o login login.c -lpq
**/
#include"cmss.h"

int account_create(pthread_t __selfId, PGconn *__con, int __soc, UserInfo *__User_Info){
  PGresult *res, *resSrc, *resDest;
  char sql[BUFSIZE];
  int resultRows = 0,resultFields = 0;
  char sendBuf[BUFSIZE], recvBuf[BUFSIZE];
  int sendLen, recvLen;

  /********* ここからIPOのプロセス処理 *********/

  char login_id[100];  //入力するログインID
  char pwd[100];  //入力するパスワード
  int count = 0;  //入力ミス数の計算

  //入力されたログインIDが学務情報システムのログインIDと一致するかチェックして存在しない場合、プログラム終了
  //入力された学籍番号とパスワードが学務情報システムと一致するかチェックして一致しない場合プログラム終了

  //入力が3回間違えたら終了する。
  while (count < 3){

    sprintf(sendBuf,"ログインIDを入力してください。%s%s", ENTER, DATA_END);
    sendLen = strlen(sendBuf);
    send( __soc,sendBuf,sendLen,0);
    printf("[C _THREAD %ld] SEND => %s\n",__selfId,sendBuf);

    recvLen = receive_message(__soc, recvBuf, BUFSIZE);
    if( recvLen < 1 ) break;
    recvBuf[recvLen-1] = '\0'; // <LF>を消去
    strcpy(login_id, recvBuf);
    printf("[C_THREAD %ld] RECV=> %s\n", __selfId, recvBuf);

    sprintf(sendBuf,"パスワードを入力してください。%s%s", ENTER, DATA_END);
    sendLen = strlen(sendBuf);
    send( __soc,sendBuf,sendLen,0);
    printf("[C _THREAD %ld] SEND => %s\n",__selfId,sendBuf);

    recvLen = receive_message(__soc, recvBuf, BUFSIZE);
    if( recvLen < 1 ) break;
    recvBuf[recvLen-1] = '\0'; // <LF>を消去
    strcpy(pwd, recvBuf);
    printf("[C_THREAD %ld] RECV=> %s\n", __selfId, recvBuf);

    /* SELECTのSQL文を作成 */
    sprintf(sql, "SELECT * FROM users where rogin_id = '%s' AND password = %d;",login_id, atoi(pwd));
    printf("%s\n",sql);
    /* SQLコマンド実行 */
    res = PQexec(__con, sql);

    /* SQLコマンド実行結果状態を確認 */
    if(PQresultStatus(res) != PGRES_TUPLES_OK){
      sprintf(sendBuf,"ログインできません。%s%s", ENTER, DATA_END);
      sendLen = strlen(sendBuf);
      send( __soc,sendBuf,sendLen,0);
      printf("[C _THREAD %ld] SEND => %s\n",__selfId,sendBuf);
      printf("%s\n", PQresultErrorMessage(res));
      exit(1);
    }

    //ユーザレベル、学籍番号、学部、学科（プログラム）、学年をユーザレベル毎の構造体に格納
    /* 結果の行数（レコード数）を取得 */
    resultRows = PQntuples(res);
    if(resultRows==0){
      sprintf(sendBuf,"ユーザが存在しません%s%s", ENTER, DATA_END);
      sendLen = strlen(sendBuf);
      send( __soc,sendBuf,sendLen,0);
      printf("[C _THREAD %ld] SEND => %s\n",__selfId,sendBuf);
      count ++;
    }
    /* 結果の列数（フィールド数）を取得 */
    resultFields = PQnfields(res);
    printf("Result of select Rows:%d, Fields:%d\n", resultRows, resultFields);

    if(resultRows == 1) break;
  }

  if(count >= 3) return 0;

    strcpy( __User_Info->id, PQgetvalue(res, 0, 2));
    strcpy( __User_Info->person_name, PQgetvalue(res, 0, 3));
    strcpy( __User_Info->major, PQgetvalue(res, 0, 10));
    strcpy( __User_Info->department, PQgetvalue(res, 0, 9));
    __User_Info->user_level = atoi(PQgetvalue(res, 0, 7));
    __User_Info->school_year = atoi(PQgetvalue(res, 0, 11));

    //ユーザー名、学籍番号、ユーザーレベルを表示
    sprintf(sendBuf, "ユーザー名：%s%s", __User_Info->person_name, ENTER);
  sendLen = strlen(sendBuf);
  send( __soc,sendBuf,sendLen,0);
  printf("[C _THREAD %ld] SEND => %s\n",__selfId,sendBuf);

  sprintf(sendBuf,"学籍番号：%s%s", __User_Info->id, ENTER);
  sendLen = strlen(sendBuf);
  send( __soc,sendBuf,sendLen,0);
  printf("[C _THREAD %ld] SEND => %s\n",__selfId,sendBuf);

  sprintf(sendBuf,"ユーザーレベル：%d%s", __User_Info->user_level, ENTER);
  sendLen = strlen(sendBuf);
  send( __soc,sendBuf,sendLen,0);
  printf("[C _THREAD %ld] SEND => %s\n",__selfId,sendBuf);


  sprintf(sendBuf,"でログインしました。\n%s", ENTER);
  sendLen = strlen(sendBuf);
  send( __soc,sendBuf,sendLen,0);
  printf("[C _THREAD %ld] SEND => %s\n",__selfId,sendBuf);


  return 1;

}
