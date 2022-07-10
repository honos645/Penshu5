/**
 *    gcc -o work_main work_main.c -lpq
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <postgresql/libpq-fe.h>

#define BUFSIZE 1024
#define ENTER "\n" //<LF>

//*** レスポンスステータス ***//
#define OK_STAT    "+OK"     //成功
#define ER_STAT    "-ERR"    //失敗

//*** エラーコード ***//
#define E_CODE_1    100  //役職が条件を満していない
#define E_CODE_2    200  //SELECT文でエラー
#define E_CODE_3    300  //SELECT文で持って来たデータの行数が１より小さい
#define E_CODE_4    400  //借り入れ金額を下回っている
#define E_CODE_5    500  //リクエストコマンドの引数エラー
#define E_CODE_6    600  //リクエストコマンドが存在しない
#define E_CODE_7    700  //データベースエラー


typedef struct _UserInfo {
  char rogin_id;
  int password;
  int  id;
  char person_name;
  char adress_code;
  char email_adress;
  char birthday_date;
  char user_level;
  char roll;
  char department;
  char major;
  int school_year;
}UserInfo;

int main(int argc, char *argv[]){
  PGconn   *con;	//PGConnオブジェクト
  PGresult *res, *resSrc, *resDest;	
  char	*dbHost	= "kite.cs.miyazaki-u.ac.jp";	//DBServerのホスト名
  char	*dbPort	= "5432";	 //DBServerのポート番号
  char	*dbName	= "db14";	 //データベース名
  char	*dbLogin= "dbuser14";	 //データベース・ユーザ
  char	*dbPwd	= "dbpass14";	 //データベース・ユーザのパスワード
  char id; //生徒の学籍番号
  char proceed;
  char place;
  char route_id;
  char industry_id;
  char occupation_id;
  UserInfo User_Info;
  char connInfo[BUFSIZE];
  char sql[BUFSIZE];
  char recvBuf[BUFSIZE], sendBuf[BUFSIZE], comm[BUFSIZE], transferText[BUFSIZE];
  int n,point;
  char *sqlBegin="BEGIN", *sqlCommit="COMMIT", *sqlRollback="ROLLBACK";
  
  /* データベース接続パラメータ設定 */
  sprintf(connInfo, "host=%s port=%s dbname=%s user=%s password=%s",
	  dbHost, dbPort, dbName, dbLogin, dbPwd);

  /* データベース接続 */
  con = PQconnectdb(connInfo);

  /* 接続状態を確認 */
  if(PQstatus(con) == CONNECTION_BAD){
    printf("Connection to database '%s:%s %s' failed.\n", dbHost, dbPort, dbName);
    printf("%s", PQerrorMessage(con));
    PQfinish(con);
    exit(1);
  }
  printf("[Connected to database %s:%s %s]\n", dbHost, dbPort, dbName);

  /********* ここからIPOのプロセス処理 *********/

  /* 振込要求情報を受信（入力）*/
  /*fgets(recvBuf, BUFSIZE, stdin);
  n = sscanf(recvBuf, "%s %d %d %d %s",
  comm, &, &idDest, &transferValue, transferText);*/

  /*if(n != 5){
    sprintf(sendBuf,"%s %d%s", ER_STAT, E_CODE_5, ENTER);
    printf("%s", sendBuf);
    return -1;
    }*/

  /*スキーマ検索パスを変更 */
  /*sprintf(sql, "SET search_path to netbank");
    PQexec(con, sql);*/

  /* tableチェック */

  /*sprintf(sql, "SELECT route, route_industry, route_occupation_id, Employment_name, Employment_subject FROM work WHERE id = %d", id);
  printf("[%s]\n", sql);
  res = PQexec(con, sql);
  if(PQresultStatus(res) != PGRES_TUPLES_OK){
    printf("%s\n", PQresultErrorMessage(res));
    sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_7, ENTER);
    printf("%s", sendBuf);
    return -1;
  }*/

  printf("あなたのユーザレベルを入力してください\n");
  scanf("%s", &User_Info.user_level);
  
  id = User_Info.id;

  if(User_Info.user_level != '0' || User_Info.user_level != '1'){
    if(User_Info.user_level != '7' || User_Info.user_level != '8'){
      printf("error:閲覧権限がありません。\n");
      return 0;
    }
    printf("進路を変更したい学籍番号を入力してください\n");
    scanf("%s", &id);
    /* tableチェック */
    sprintf(sql, "SELECT person_name FROM users WHERE id = %d", id);
    printf("[%s]\n", sql);
    res = PQexec(con, sql);
    if(PQresultStatus(res) != PGRES_TUPLES_OK){
      printf("%s\n", PQresultErrorMessage(res));
      sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_7, ENTER);
      printf("%s", sendBuf);
      return -1;
    }
  }
  
  //printf("進学する場合は0を表示してください\n就職する場合は1を入力してください\n");
  //scanf("%d", &point);

  printf("変更したい進学・就職先を入力してください\n");
  scanf("%s", &proceed);
  
  printf("変更したい進学・就職先所在地県名を入力してください\n");
  scanf("%s", &place);

  if(User_Info.user_level == '7' || User_Info.user_level == '8'){
    printf("進路区分、産業区分、職種を入力してください\n入力しない場合、0を入力してください");
    scanf("%s %s %s", route_id,industry_id,occupation_id);
  }

  PQexec(con, sqlBegin); //トランザクション開始*/

  /* 更新 */
  if(route_id != NULL || route_id != '0'){
    sprintf(sql,"UPDATE work SET route_id=%s WHERE id = %d", route_id,id);
    printf("[%s]\n", sql);
  
    res = PQexec(con, sql);

    if(PQresultStatus(res) != PGRES_COMMAND_OK){
      printf("%s", PQresultErrorMessage(res));
      sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_7, ENTER);
      printf("%s", sendBuf);
      PQexec(con, sqlRollback);
      return -1;
    }

    if(atoi(PQcmdTuples(res)) != 1){
      sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_7, ENTER);
      printf("%s", sendBuf);
      PQexec(con, sqlRollback);
      return -1;
    }
  }

  if(industry_id != NULL || industry_id != '0'){
    sprintf(sql,"UPDATE work SET route_id=%s WHERE id = %d", industry_id,id);
    printf("[%s]\n", sql);
  
    res = PQexec(con, sql);

    if(PQresultStatus(res) != PGRES_COMMAND_OK){
      printf("%s", PQresultErrorMessage(res));
      sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_7, ENTER);
      printf("%s", sendBuf);
      PQexec(con, sqlRollback);
      return -1;
    }

    if(atoi(PQcmdTuples(res)) != 1){
      sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_7, ENTER);
      printf("%s", sendBuf);
      PQexec(con, sqlRollback);
      return -1;
    }
  }

  if(occupation_id != NULL || occupation_id != '0'){
    sprintf(sql,"UPDATE work SET route_id=%s WHERE id = %d", occupation_id,id);
    printf("[%s]\n", sql);
  
    res = PQexec(con, sql);

    if(PQresultStatus(res) != PGRES_COMMAND_OK){
      printf("%s", PQresultErrorMessage(res));
      sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_7, ENTER);
      printf("%s", sendBuf);
      PQexec(con, sqlRollback);
      return -1;
    }

    if(atoi(PQcmdTuples(res)) != 1){
      sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_7, ENTER);
      printf("%s", sendBuf);
      PQexec(con, sqlRollback);
      return -1;
    }
  }

  if(proceed != NULL){
    sprintf(sql,"UPDATE work SET Employment_name=%s WHERE id = %d", proceed,id);
    printf("[%s]\n", sql);
  
    res = PQexec(con, sql);

    if(PQresultStatus(res) != PGRES_COMMAND_OK){
      printf("%s", PQresultErrorMessage(res));
      sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_7, ENTER);
      printf("%s", sendBuf);
      PQexec(con, sqlRollback);
      return -1;
    }

    if(atoi(PQcmdTuples(res)) != 1){
      sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_7, ENTER);
      printf("%s", sendBuf);
      PQexec(con, sqlRollback);
      return -1;
    }
  }

  if(place != NULL){
    sprintf(sql,"UPDATE work SET Employment_subject=%s WHERE id = %d", place,id);
    printf("[%s]\n", sql);
  
    res = PQexec(con, sql);

    if(PQresultStatus(res) != PGRES_COMMAND_OK){
      printf("%s", PQresultErrorMessage(res));
      sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_7, ENTER);
      printf("%s", sendBuf);
      PQexec(con, sqlRollback);
      return -1;
    }

    if(atoi(PQcmdTuples(res)) != 1){
      sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_7, ENTER);
      printf("%s", sendBuf);
      PQexec(con, sqlRollback);
      return -1;
    }
  }

  /* プロトコル・レスポンス */
  sprintf(sendBuf, "%s %s %s %s%s", OK_STAT
	 ,id
	 ,proceed
	 ,place,ENTER);
  printf("%s", sendBuf);

  PQexec(con, sqlCommit);  //トランザクション終了（コミット）

  /* データベース接続を切断 */
  PQfinish(con);

  return 0;
}
