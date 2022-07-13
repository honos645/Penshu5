/**
 * @file judge.c
 * @author 67200072 宇佐見 (hm20007@student.miyaazki-u.ac.jp)
 * @brief 卒業判定，不足科目表示
 *        コマンド::進級判定:pjudge 卒研判定:rjudge 卒業判定:gjudge 修了判定:cjudge
 *        gcc judge_libpq.c -o judge -g -lpq
 * @version 1.0
 * @date 2022-07-05
 *
 * @copyright Copyright (c) 2022
 *
 */

// TODO: 不可の時の不足科目を表示する機能
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <postgresql/libpq-fe.h>

#define BUFSIZE 1024
#define ENTER "\n" //<LF>

//*** レスポンスステータス ***//
#define OK_STAT    "+OK"     //成功
#define ER_STAT    "-ERR"    //失敗

//*** 判定 ***//
#define PASSING 0
#define FAILURE 1
#define PENDING 2

//*** ユーザステータス ***//
#define ADMIN 0
#define STUDENT 1
#define STAFF 2
#define KYOMU 3
#define TEACHER 4
#define CLASSTEACHER 5
#define SUBTEACHER 6
#define CLASSEMPLOY 7
#define EMPLOY 8
#define DEPCHAIR 9

typedef struct _UserData {
  char id[9];    // 学籍番号
  int level; // ユーザレベル
}UserData;

int judge_personal(PGconn *__con, char *__studentNum, int judgeFlag);
int judge_list(PGconn *__con, UserData __user, int __judgeFlag);

int main(int argc, char **argv) {
  PGconn   *con;  // PGConnオブジェクト
  PGresult *res;
  char *dbHost  = "kite.cs.miyazaki-u.ac.jp";   // DBServerのホスト名
  char *dbPort  = "5432";                       // DBServerのポート番号
  char *dbName  = "db06";                       // データベース名
  char *dbUser  = "dbuser06";                   // データベース・ユーザ
  char *dbPwd   = "dbpass06";                   // データベース・ユーザのパスワード

  char connInfo[BUFSIZE];
  char sql[BUFSIZE];
  char recvBuf[BUFSIZE], sendBuf[BUFSIZE], comm[BUFSIZE], transferText[BUFSIZE];
  char temp[BUFSIZE], commArg[BUFSIZE];
  int resultRows, cmdArgc, resultJudge;
  UserData userData = {"A0000000", 0};

  /* データベース接続パラメータ設定 */
  sprintf(connInfo, "host=%s port=%s dbname=%s user=%s password=%s",
                      dbHost, dbPort, dbName, dbUser, dbPwd);

  /* データベース接続 */
  con = PQconnectdb(connInfo);

  /* 接続状態確認 */
  if(PQstatus(con) == CONNECTION_BAD) {
    printf("Connection to database '%s:%s %s' failed.\n", dbHost, dbPort, dbName);
    printf("%s", PQerrorMessage(con));
    PQfinish(con);
    exit(1);
  }

  /* スキーマ変更 */
  sprintf(sql, "SET search_path to cmss");
  res = PQexec(con, sql);

  // コマンド
  printf("ユーザ登録\n学籍番号 ユーザレベル\n");
  fgets(recvBuf, BUFSIZE, stdin);
  cmdArgc = sscanf(recvBuf, "%s %d", userData.id, &userData.level);

  /***** IPOプロセス処理 *****/
  printf("[判定]\n");
  printf("コマンドを入力: ");
  fgets(recvBuf, BUFSIZE, stdin);
  cmdArgc = sscanf(recvBuf, "%s %s", comm, commArg);

  if(!strcmp("pjudge", comm) && cmdArgc == 2){
    /* 進級判定 */
    switch (userData.level) {
      case STUDENT:
        if(!strcmp(userData.id, commArg)) {
          judge_personal(con, commArg, 0);
        }else {
          printf("学籍番号が違います\n");
        }
        break;
      case CLASSTEACHER:
      case SUBTEACHER:
        if(!strcmp(commArg, "00000000")) {
          judge_list(con, userData, 0);
        }else {
          judge_personal(con, commArg, 0);
        }
        break;
      case KYOMU:
      case STAFF:
      case DEPCHAIR:
        if(!strcmp(commArg, "00000000")) {
          judge_list(con, userData, 0);
        }else {
          judge_personal(con, commArg, 0);
        }
        break;
    }// ここまで進級判定

  }else if(!strcmp("rjudge", comm) && cmdArgc == 2) {
    /* 卒業研究着手判定 */
    switch (userData.level) {
      case STUDENT:
        if(!strcmp(userData.id, commArg)) {
          judge_personal(con, commArg, 1);
        }else {
          printf("学籍番号が違います\n");
        }
        break;
      case CLASSTEACHER:
      case SUBTEACHER:
        if(!strcmp(commArg, "00000000")) {
          judge_list(con, userData, 1);
        }else {
          judge_personal(con, commArg, 1);
        }
        break;
      case KYOMU:
      case STAFF:
      case DEPCHAIR:
        if(!strcmp(commArg, "00000000")) {
          judge_list(con, userData, 1);
        }else {
          judge_personal(con, commArg, 1);
        }
        break;
    }// ここまで卒業研究着手判定

  }else if(!strcmp("gjudge", comm) && cmdArgc == 2) {
    /* 卒業判定 */
    switch (userData.level) {
      case STUDENT:
        if(!strcmp(userData.id, commArg)) {
          judge_personal(con, commArg, 2);
        }else {
          printf("学籍番号が違います\n");
        }
        break;
      case CLASSTEACHER:
      case SUBTEACHER:
        if(!strcmp(commArg, "00000000")) {
          judge_list(con, userData, 2);
        }else {
          judge_personal(con, commArg, 2);
        }
        break;
      case KYOMU:
      case STAFF:
      case DEPCHAIR:
        if(!strcmp(commArg, "00000000")) {
          judge_list(con, userData, 2);
        }else {
          judge_personal(con, commArg, 2);
        }
        break;
    }// ここまで卒業判定

  }else if(!strcmp("cjudge", comm) && cmdArgc == 2) {
    /* 修了判定 */
    switch (userData.level) {
      case STUDENT:
        if(!strcmp(userData.id, commArg)) {
          judge_personal(con, commArg, 3);
        }else {
          printf("学籍番号が違います\n");
        }
        break;
      case CLASSTEACHER:
      case SUBTEACHER:
        if(!strcmp(commArg, "00000000")) {
          judge_list(con, userData, 3);
        }else {
          judge_personal(con, commArg, 3);
        }
        break;
      case KYOMU:
      case STAFF:
      case DEPCHAIR:
        if(!strcmp(commArg, "00000000")) {
          judge_list(con, userData, 3);
        }else {
          judge_personal(con, commArg, 3);
        }
        break;
    }// ここまで修了判定
  }

  PQfinish(con);

  return 0;
}// main End

/**
 * @brief 学籍番号から判定をする関数
 *
 * @param __con 構造体PGconn
 * @param __studentNum 判定する学籍番号
 * @param __judgeFlag 進級：０　卒業研究：１　卒業：２　修了：３
 * @return int 成功: 0, エラー: -1
 */
int judge_personal(PGconn * __con, char *__studentNum, int __judgeFlag) {
  PGresult *res;
  char sql[BUFSIZE];
  int resultJudge, resultRows;
  char judgeList[4][BUFSIZE] = {"promotion_judge", "research_judge", "graduation_judge", "completion_judge"};
  char sendBuf[BUFSIZE];

  /* 合否取得SQL作成 */
  sprintf(sql, "SELECT grade_judge.id, users.person_name, CASE WHEN grade_judge.%s = 0 THEN '可' ELSE '不可' END AS judge FROM grade_judge INNER JOIN users ON grade_judge.id = users.id WHERE grade_judge.id = '%s';", judgeList[__judgeFlag], __studentNum);

  /* SQL実行 */
  res = PQexec(__con, sql);
  if(PQresultStatus(res) != PGRES_TUPLES_OK) {
    printf("%s\n", PQresultErrorMessage(res));
//TODO: エラー
    printf("error_1\n");
    return -1;
  }

  resultRows = PQntuples(res);
  if(resultRows != 1) {
//TODO: エラー
    printf("%d error_2\n", resultRows);
    return -1;
  }

  // 送信用文作成
  sprintf(sendBuf, "%s\t%s\t%s\n", PQgetvalue(res, 0, 0), PQgetvalue(res, 0, 1), PQgetvalue(res, 0, 2));

// NOTE: 分野別取得単位数を表示-未検証
  if(strcmp(PQgetvalue(res, 0, 2), "可") {
    char TMP = "B110";
    sprintf(sql, "SELECT grade_list.classification_name AS name, grade_list.classification, sum(CASE WHEN grade_list.grade_point >= 60 AND grade_list.grade_point <= 100 THEN grade_list.credit ELSE 0 END) AS total FROM (SELECT subject_grade.id ,subject_detail.subject_code ,subject_detail.course_sharing_code ,subject_detail.subject_name ,subject_detail.credit ,subject_detail.classification ,classification_name.classification_name ,subject_detail.courses_available ,subject_grade.grade_point FROM (subject_detail INNER JOIN classification_name ON subject_detail.classification = classification_name.classification_code) LEFT OUTER JOIN subject_grade ON subject_detail.subject_code = subject_grade.subject_code AND subject_detail.opening_year = subject_grade.opening_year WHERE subject_detail.courses_available = '%s' AND (subject_grade.id = '%s' OR subject_grade.id IS NULL)) AS grade_list GROUP BY grade_list.classification, grade_list.classification_name;", TMP, __studentNum);
  }

  printf("%s", sendBuf);

  return 0;
}// judge_personal End

/**
 * @brief 判定結果の一覧表示
 *
 * @param __con 構造体PGconn
 * @param __user 学籍番号とユーザレベルの構造体
 * @param __judgeFlag 進級：０　卒業研究：１　卒業：２　修了：３
 * @return int 成功: 0, エラー: -1
 */
int judge_list(PGconn *__con, UserData __user, int __judgeFlag) {
  PGresult *res;
  char sql[BUFSIZE];
  int resultJudge, resultRows;
  char List[4][BUFSIZE] = {"promotion_judge", "research_judge", "graduation_judge", "completion_judge"};
  char sendBuf[BUFSIZE];

  /* 合否取得SQL作成 */
  if(__user.level == CLASSTEACHER || __user.level == SUBTEACHER) {
    sprintf(sql, "SELECT grade_judge.id, users.person_name, CASE WHEN grade_judge.%s = 0 THEN '可' ELSE '不可' END AS judge FROM grade_judge INNER JOIN users ON grade_judge.id=users.id WHERE (users.school_year = (SELECT school_year FROM users WHERE id = '%s'));", List[__judgeFlag], __user.id);

  }else if(__user.level == STAFF || __user.level == DEPCHAIR || __user.level == ADMIN) {
    sprintf(sql, "SELECT grade_judge.id, users.person_name, CASE WHEN grade_judge.%s = 0 THEN '可' ELSE '不可' END AS judge FROM grade_judge INNER JOIN users ON grade_judge.id=users.id WHERE (grade_judge.id LIKE 'B%%' OR grade_judge.id LIKE 'M%%' OR grade_judge.id LIKE 'D%%');", List[__judgeFlag]);

  }else {
// TODO: エラー
    printf("error_3\n");
  }

  /* SQL実行 */
  res = PQexec(__con, sql);
  if(PQresultStatus(res) != PGRES_TUPLES_OK) {
    printf("%s\n", PQresultErrorMessage(res));
//TODO: エラー
    printf("error_4\n");
    return -1;
  }

  resultRows = PQntuples(res);
  if(resultRows < 0) {
//TODO: エラー
    printf("%d error_5\n", resultRows);
    return -1;
  }

  for (int i = 0; i < resultRows; i++) {
    sprintf(sendBuf, "%s%s\t%s\t%s\n", sendBuf, PQgetvalue(res, i, 0), PQgetvalue(res, i, 1), PQgetvalue(res, i, 2));
  }

  printf("%s", sendBuf);

  return 0;
}