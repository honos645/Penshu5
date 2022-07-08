/**
 *    gcc -o transfer_main transfer_main.c -lpq
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
#define E_CODE_2    200  //
#define E_CODE_3    300  //貯金限度額を超えている
#define E_CODE_4    400  //借り入れ金額を下回っている
#define E_CODE_5    500  //リクエストコマンドの引数エラー
#define E_CODE_6    600  //リクエストコマンドが存在しない
#define E_CODE_7    700  //データベースエラー

typedef struct _AccountBank {
  int  account_id;
  char account_name[100];
  int  balance;
  int  max_balance;
  int  min_balance;
  int  account_point;
}AccountBank;

typedef struct _UserInfo {
  int  id;
  char person_name;
  char user_level;
  char roll;
  char department;
  char major;
}UserInfo;

int main(int argc, char *argv[]){
  PGconn   *con;	//PGConnオブジェクト
  PGresult *res, *resSrc, *resDest;	
  char	*dbHost	= "kite.cs.miyazaki-u.ac.jp";	//DBServerのホスト名
  char	*dbPort	= "10000";	 //DBServerのポート番号
  char	*dbName	= "db38";	 //データベース名
  char	*dbLogin= "dbuser38";	 //データベース・ユーザ
  char	*dbPwd	= "dbpass38";	 //データベース・ユーザのパスワード
  char user_level, user_department, user_major;
  int user_school_year;
  int sort;
  int id
  char	connInfo[BUFSIZE];
  char	sql[BUFSIZE], sqlSrc[BUFSIZE], sqlDest[BUFSIZE];
  char recvBuf[BUFSIZE], sendBuf[BUFSIZE], comm[BUFSIZE], transferText[BUFSIZE];
  int resultRows, n;
  int resultBalanceSrc, resultBalanceDest;
  int idSrc, idDest, transferValue;
  AccountBank accountSrc, accountDest;
  char *transactionNoSrc, *transactionNoDest;
  int point=25;
  char *sqlBegin="BEGIN", *sqlCommit="COMMIT", *sqlRollback="ROLLBACK";*/
  
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
  comm, &idSrc, &idDest, &transferValue, transferText);*/

  /*if(n != 5){
    sprintf(sendBuf,"%s %d%s", ER_STAT, E_CODE_5, ENTER);
    printf("%s", sendBuf);
    return -1;
    }*/

  printf("あなたのユーザレベル・学部・学科・学年を入力してください");
  scanf("%s %s %s %d",user_level, user_department, user_major, user_school_year);
  printf("GPAを閲覧したい学部・学科・学年を入力してください");
  scanf("%s %s %d", UserInfo->department, UserInfo->major, UserInfo->search_school_year);

  if( user_level == 1){
    printf("error:閲覧権限がありません。\n");
    return 0;
  }

  if( user_level == 4 || user_level == 5){
    if( UserInfo.department != department || UserInfo.major != major || UserInfo.school_year == school_year ){
      printf("error:閲覧権限がありません。\n");     
      return 0;
    }
  }

  printf("学籍番号順に表示する場合は0を表示してください\n次順に表示する場合は1を入力してください\n");
  scnaf("%d",&sort);

  if( sort == 0 ){
    sprintf(sql, "SELECT b.id, a.person_name, b.gpa, b.ranking FROM users AS a INNER JOIN grade_judge AS b ON a.id = b.id WHERE depertment = %c AND major = %c AND school_year.id = %d AND ORDER BY b.id", UserInfo.department, UserInfo.major, UserInfo.school_year );
  }else if( sort == 1 ){
    sprintf(sql, "SELECT b.id, a.person_name, b.gpa, b.ranking FROM users AS a INNER JOIN grade_judge AS b ON a.id = b.id WHERE depertment = %c AND major = %c AND school_year.id = %d AND ORDER BY b.ranking", UserInfo.department, UserInfo.major, UserInfo.school_year );
  }

  res = PQexec(con, sql);
  if(PQresultStatus(res) != PGRES_TUPLES_OK){
    printf("%s\n", PQresultErrorMessage(res));
    sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_2, ENTER);
    printf("%s", sendBuf);
    return -1;
  }

  resultRows = PQntuples(res);
  if(resultRows < 0){
    sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_3, ENTER);
    printf("%s", sendBuf);
    return -1;
  }
  
  for(
  id = PQgetvalue(res, 1, 0); //0 番目のフィールド値を取得
  person_name = PQgetvalue(res, 1, 1); //1 番目のフィールド値を取得
  gpa = PQgetvalue(res, 1, 2); //2 番目のフィールド値を取得
  ranking = PQgetvalue(res, 1, 3); //3 番目のフィールド値を取得
  printf("%s %s %d %d¥n", id, name, address, atoi(age));

  /*スキーマ検索パスを変更 */
  /*sprintf(sql, "SET search_path to netbank");
    PQexec(con, sql);*/
  
  /* 振込元、振込先口座チェック */
  /*sprintf(sql, "SELECT account_id, account_name, balance, min_balance, account_point FROM bank_account_t   WHERE account_id =%d", idSrc);
  printf("[%s]\n", sql);
  res = PQexec(con, sql);
  if(PQresultStatus(res) != PGRES_TUPLES_OK){
    printf("%s\n", PQresultErrorMessage(res));
    sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_7, ENTER);
    printf("%s", sendBuf);
    return -1;
  }

  resultRows = PQntuples(res);
  if(resultRows != 1){
    sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_1, ENTER);
    printf("%s", sendBuf);
    return -1;
  }
  accountSrc.account_id = atoi(PQgetvalue(res, 0, 0));
  strcpy(accountSrc.account_name, PQgetvalue(res, 0, 1));
  accountSrc.balance = atoi(PQgetvalue(res, 0, 2));
  accountSrc.min_balance = atoi(PQgetvalue(res, 0, 3));
  accountSrc.account_point = atoi(PQgetvalue(res, 0, 4));

  sprintf(sql, "SELECT account_id, account_name, balance, max_balance, account_point FROM bank_account_t   WHERE account_id =%d", idDest);
  printf("[%s]\n", sql);
  res = PQexec(con, sql);
  if(PQresultStatus(res) != PGRES_TUPLES_OK){
    printf("%s\n", PQresultErrorMessage(res));
    sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_7, ENTER);
    printf("%s", sendBuf);
    return -1;
  }

  resultRows = PQntuples(res);
  if(resultRows != 1){
    sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_1, ENTER);
    printf("%s", sendBuf);
    return -1;
  }
  accountDest.account_id = atoi(PQgetvalue(res, 0, 0));
  strcpy(accountDest.account_name, PQgetvalue(res, 0, 1));
  accountDest.balance = atoi(PQgetvalue(res, 0, 2));
  accountDest.max_balance = atoi(PQgetvalue(res, 0, 3));
  accountDest.account_point = atoi(PQgetvalue(res, 0, 4));

  resultBalanceSrc = accountSrc.balance - transferValue;
  resultBalanceDest = accountDest.balance + transferValue;*/
  
  /* 振込元・振込先の限度額をチェック */
  /*if(resultBalanceSrc < accountSrc.min_balance){
    sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_4, ENTER);
    printf("%s", sendBuf);
    return -1;
  }

  if(resultBalanceDest > accountDest.max_balance){
    sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_3, ENTER);
    printf("%s", sendBuf);
    return -1;
  }

  PQexec(con, sqlBegin); //トランザクション開始*/
  
  /* 振込元・振込先預金口座を更新 */
  /*sprintf(sqlSrc,"UPDATE bank_account_t SET balance=balance-%d WHERE account_id=%d", transferValue,idSrc);
  sprintf(sqlDest,"UPDATE bank_account_t SET balance=balance+%d WHERE account_id=%d", transferValue, idDest);
  printf("[%s]\n", sqlSrc);
  printf("[%s]\n", sqlDest);
  
  resSrc  = PQexec(con, sqlSrc);
  resDest = PQexec(con, sqlDest);

  if(PQresultStatus(resSrc) != PGRES_COMMAND_OK || PQresultStatus(resDest) != PGRES_COMMAND_OK){
    printf("%s", PQresultErrorMessage(resSrc));
    printf("%s", PQresultErrorMessage(resDest));
    sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_7, ENTER);
    printf("%s", sendBuf);
    PQexec(con, sqlRollback);    
    return -1;
  }

  
  if(atoi(PQcmdTuples(resSrc)) != 1 || atoi(PQcmdTuples(resDest)) != 1){
    sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_7, ENTER);
    printf("%s", sendBuf);
    PQexec(con, sqlRollback);
    return -1;
  }

  accountSrc.balance  = resultBalanceSrc;
  accountDest.balance = resultBalanceDest;*/
  
  /* 振込元・振込先の預金口座取引を記録 */

  /* シーケンスを使用した主キー生成 (TR00001) */
  /*sprintf(sql, "SELECT 'TR'||LPAD(nextval('transaction_no_seq')::text,5,'0')");
  printf("[%s]\n", sql);
  res = PQexec(con, sql);
  transactionNoSrc = PQgetvalue(res, 0, 0);
  
  
  sprintf(sqlSrc, "INSERT INTO transaction_detail_t (transaction_no, account_id, transaction_class,transaction_summary,transaction_amount,balance,transaction_date) VALUES('%s', %d, 400, '振込 (%s)', %d, %d, now())",
	  transactionNoSrc,
	  accountSrc.account_id,
	  accountDest.account_name,
	  transferValue,
	  accountSrc.balance);
  printf("[%s]\n",sqlSrc);
  
  resSrc = PQexec(con, sqlSrc);

  if(PQresultStatus(resSrc) != PGRES_COMMAND_OK){
    printf("%s", PQresultErrorMessage(resSrc));
    sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_7, ENTER);
    printf("%s", sendBuf);
    PQexec(con, sqlRollback);
    return -1;
    }*/

  /* シーケンスを使用した主キー生成 (TR00001) */
  /*sprintf(sql, "SELECT 'TR'||LPAD(nextval('transaction_no_seq')::text,5,'0')");
  printf("[%s]\n", sql);
  res = PQexec(con, sql);
  transactionNoDest = PQgetvalue(res, 0, 0);


  sprintf(sqlDest, "INSERT INTO transaction_detail_t (transaction_no, account_id, transaction_class,transaction_summary,transaction_amount,balance,transaction_date) VALUES('%s', %d, 401, '振込 (%s)', %d, %d, now())",
	  transactionNoDest,
	  accountDest.account_id,
	  accountSrc.account_name,
	  transferValue,
	  accountDest.balance);
  printf("[%s]\n", sqlDest);
  
  resDest = PQexec(con, sqlDest);

  if(PQresultStatus(resDest) != PGRES_COMMAND_OK){
    printf("%s", PQresultErrorMessage(resDest));
    sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_7, ENTER);
    printf("%s", sendBuf);
    PQexec(con, sqlRollback);
    return -1;
    }*/

  /* 振込処理によるポイント加算 */

  /*accountSrc.account_point += point;
  
  sprintf(sqlSrc, "UPDATE bank_account_t SET account_point=%d WHERE account_id = %d",
	  accountSrc.account_point,
	  accountSrc.account_id);
  printf("[%s]\n", sqlSrc);
  
  resSrc = PQexec(con, sqlSrc);

  if(PQresultStatus(resSrc) != PGRES_COMMAND_OK){
    printf("%s", PQresultErrorMessage(resSrc));
    sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_7, ENTER);
    printf("%s", sendBuf);
    PQexec(con, sqlRollback);
    return -1;
    }*/

  /* プロトコル・レスポンス */
  /*sprintf(sendBuf, "%s %d %s %d %d%s", OK_STAT
	 ,accountSrc.account_id
	 ,accountSrc.account_name
	 ,point
	 ,accountSrc.account_point,ENTER);
  printf("%s", sendBuf);
  PQexec(con, sqlCommit);*/  //トランザクション終了（コミット）
  
  /* データベース接続を切断 */
  PQfinish(con);

  return 0;
}
