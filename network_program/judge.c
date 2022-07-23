/**
 * @file judge.c
 * @author 67200072 宇佐見 (hm20007@student.miyaazki-u.ac.jp)
 * @brief 卒業判定，不足科目表示
 *        コマンド::卒研判定:rjudge 卒業判定:gjudge 修了判定:cjudge
 *        gcc judge_libpq.c -o judge -g -lpq
 * @version 1.0
 * @date 2022-07-05
 *
 * @copyright Copyright (c) 2022
 *
 */

#include"judge.h"

int judge_main(pthread_t __selfId, PGconn *__con, int __soc, UserInfo *__User, int __judgeFlag, char *__recvBuf) {
  char sql[BUFSIZE];
  char sendBuf[BUFSIZE], comm[BUFSIZE], transferText[BUFSIZE];
  char temp[BUFSIZE], commArg[BUFSIZE];
  int resultRows, cmdArgc, resultJudge, sendLen;

  cmdArgc = sscanf(__recvBuf, "%s %s", comm, commArg);

  if(!strcmp("rjudge", comm) && cmdArgc == 2) {
    /* 卒業研究着手判定 */
    switch (__User->user_level) {
      case STUDENT:
        if(!strcmp(__User->id, commArg)) {
          judge_personal( __selfId, __con, __soc, commArg, 0);
        }else {
          sendLen = sprintf(sendBuf, "学籍番号が違います%s", ENTER);
          printf("%s", sendBuf);
          send(__soc, sendBuf, sendLen, 0);
          // printf("学籍番号が違います\n");
        }
        break;
      case CLASSTEACHER:
      case SUBTEACHER:
        if(!strcmp(commArg, "00000000")) {
          judge_list(__selfId, __con, __soc, __User, 0);
        }else {
          judge_personal(__selfId, __con, __soc, commArg, 0);
        }
        break;
      case KYOMU:
      case STAFF:
      case DEPCHAIR:
        if(!strcmp(commArg, "00000000")) {
          judge_list(__selfId, __con, __soc, __User, 0);
        }else {
          judge_personal( __selfId, __con, __soc, commArg, 0);
        }
        break;
    }// ここまで卒業研究着手判定

  }else if(!strcmp("gjudge", comm) && cmdArgc == 2) {
    /* 卒業判定 */
    switch (__User->user_level) {
      case STUDENT:
        if(!strcmp(__User->id, commArg)) {
          judge_personal(__selfId, __con, __soc, commArg, 1);
        }else {
          sendLen = sprintf(sendBuf, "学籍番号が違います%s", ENTER);
          printf("%s", sendBuf);
          send(__soc, sendBuf, sendLen, 0);
          // printf("学籍番号が違います\n");
        }
        break;
      case CLASSTEACHER:
      case SUBTEACHER:
        if(!strcmp(commArg, "00000000")) {
          judge_list(__selfId, __con, __soc, __User, 1);
        }else {
          judge_personal(__selfId, __con, __soc, commArg, 1);
        }
        break;
      case KYOMU:
      case STAFF:
      case DEPCHAIR:
        if(!strcmp(commArg, "00000000")) {
          judge_list(__selfId, __con, __soc, __User, 1);
        }else {
          judge_personal(__selfId, __con, __soc, commArg, 1);
        }
        break;
    }// ここまで卒業判定

  }else if(!strcmp("cjudge", comm) && cmdArgc == 2) {
    /* 修了判定 */
    switch (__User->user_level) {
      case STUDENT:
        if(!strcmp(__User->id, commArg)) {
          judge_personal(__selfId, __con, __soc, commArg, 2);
        }else {
          sendLen = sprintf(sendBuf, "学籍番号が違います%s", ENTER);
          printf("%s", sendBuf);
          send(__soc, sendBuf, sendLen, 0);
          // printf("学籍番号が違います\n");
        }
        break;
      case CLASSTEACHER:
      case SUBTEACHER:
        if(!strcmp(commArg, "00000000")) {
          judge_list(__selfId, __con, __soc, __User, 2);
        }else {
          judge_personal(__selfId, __con, __soc, commArg, 2);
        }
        break;
      case KYOMU:
      case STAFF:
      case DEPCHAIR:
        if(!strcmp(commArg, "00000000")) {
          judge_list(__selfId, __con, __soc, __User, 2);
        }else {
          judge_personal(__selfId, __con, __soc, commArg, 2);
        }
        break;
    }// ここまで修了判定
  }else {
    sendLen = sprintf(sendBuf, "引数が違います%s", ENTER);
    printf("%s", sendBuf);
    // printf("コマンドが違います\n");
    send(__soc, sendBuf, sendLen, 0);
  }

  PQfinish(__con);

  return 0;
}// main End

/**
 * @brief 学籍番号から判定をする関数
 *
 * @param __con 構造体PGconn
 * @param __studentNum 判定する学籍番号
 * @param __judgeFlag 卒業研究：0　卒業：1　修了：2
 * @return int 成功: 0, エラー: -1
 */
int judge_personal(pthread_t __selfId, PGconn *__con, int __soc, char *__studentNum, int __judgeFlag) {
  PGresult *res;
  int result, resultRows;
  char sql[BUFSIZE], temp[BUFSIZE];
  char judgeList[3][BUFSIZE] = {"research_judge", "graduation_judge", "completion_judge"};
  char creditTable[3][BUFSIZE] = {"advance", "graduate", ""};
  char resultList[3][BUFSIZE] = {"着手", "卒業", "修了"};
  int sendLen = 0;
  char sendBuf[BUFSIZE];

  /* 合否取得SQL作成 例:personalJudge.sql*/
  sprintf(sql, "SELECT grade_judge.id, users.person_name, grade_judge.%s FROM grade_judge INNER JOIN users ON grade_judge.id = users.id WHERE grade_judge.id = '%s';", judgeList[__judgeFlag], __studentNum);
  // printf("%s\n", sql);

  /* SQL実行 */
  res = PQexec(__con, sql);
  if(PQresultStatus(res) != PGRES_TUPLES_OK) {
    printf("%s\n", PQresultErrorMessage(res));
//TODO: エラー1
    sendLen = sprintf(sendBuf, "%s error_1%s", ER_STAT, ENTER);
    printf("%s", sendBuf);
    send(__soc, sendBuf, sendLen, 0);
    // printf("error_1\n");
    return -1;
  }

  resultRows = PQntuples(res);
  if(resultRows != 1) {
//TODO: エラー2
    sendLen = sprintf(sendBuf, "%s error_2%s", ER_STAT, ENTER);
    printf("%s", sendBuf);
    send(__soc, sendBuf, sendLen, 0);
    // printf("error_2\n");
    return -1;
  }

  result = atoi(PQgetvalue(res, 0, 2));
  if(result == 0) {
    sendLen = sprintf(sendBuf, "%s %s %s可能%s", OK_STAT, __studentNum, resultList[__judgeFlag], ENTER);
    printf("%s", sendBuf);
    send(__soc, sendBuf, sendLen, 0);
    // printf("%s %s %s可能%s", OK_STAT, __studentNum, resultList[__judgeFlag], ENTER);

  }else if(result == 1) {
    int flag = 0;
    if(__studentNum[3] < '2' || (__studentNum[3] == '2' && __studentNum[4] == '0')) {
      sprintf(temp, "%c%c%c0", __studentNum[0], __studentNum[1], __studentNum[2]);
    }else {
      sprintf(temp, "%c%c%c1", __studentNum[0], __studentNum[1], __studentNum[2]);
    }
    /* 単位数比較表SQL 例:personalJudge2.sql */
    sprintf(sql, "SELECT classification_name.name ,subject_classification_%s.classification, subject_classification_%s.required_credit, CASE WHEN credit.sum IS NULL THEN 0 ELSE credit.sum END, subject_classification_%s.absolute_option FROM subject_classification_%s LEFT OUTER JOIN(SELECT subject_detail.classification, SUM(subject_detail.credit) AS SUM FROM subject_detail LEFT OUTER JOIN(SELECT subject_grade.subject_code, subject_grade.opening_year, subject_grade.id, CONCAT(SUBSTRING(subject_grade.id, 1, 3), SUBSTRING(subject_grade.id, 5, 1)) AS available, subject_grade.grade_point FROM subject_grade WHERE subject_grade.id = '%s') AS grade ON subject_detail.subject_code = grade.subject_code AND subject_detail.opening_year = grade.opening_year AND subject_detail.courses_available = grade.available WHERE subject_detail.courses_available = '%s' AND (grade.grade_point >= 60 AND grade.grade_point <= 100) GROUP BY subject_detail.classification) AS credit ON subject_classification_%s.classification = credit.classification INNER JOIN classification_name on subject_classification_%s.classification = classification_name.code;", creditTable[__judgeFlag], creditTable[__judgeFlag], creditTable[__judgeFlag], creditTable[__judgeFlag], __studentNum, temp, creditTable[__judgeFlag], creditTable[__judgeFlag]);


    res = PQexec(__con, sql);
    resultRows = PQntuples(res);
    int creditList[resultRows][4];
    char listName[resultRows][BUFSIZE];
    for (int i = 0; i < resultRows; i++) {
      strcpy(listName[i], PQgetvalue(res, i, 0));
      for (int j = 1; j < 5; j++) {
        creditList[i][j - 1] = atoi(PQgetvalue(res, i, j));
      }
    }
    // printf("%s %s %s不可\n必要単位\n", OK_STAT, __studentNum, resultList[__judgeFlag]);
    sendLen = sprintf(sendBuf, "%s %s %s不可\n必要単位\n", OK_STAT, __studentNum, resultList[__judgeFlag]);
    printf("%s", sendBuf);
    send(__soc, sendBuf, sendLen, 0);

    for (int i = 0; i < sizeof(creditList) / sizeof(*creditList); i++) {
      if(creditList[i][0] % 10 != 0) continue;

      for (int j = i; creditList[j][0] < creditList[i][0] + 10; j++) {
        int needCredit = creditList[j][1] - creditList[j][2];
        creditList[i][2] += creditList[j][2];

        /* SQL例 needCreditSum.sql */
        sprintf(sql, "SELECT subject_detail.subject_name, subject_detail.necessary FROM subject_detail LEFT OUTER JOIN (SELECT subject_grade.subject_code, subject_grade.opening_year, subject_grade.id, CONCAT(SUBSTRING(subject_grade.id, 1, 3), SUBSTRING(subject_grade.id, 5, 1)) AS available, subject_grade.grade_point FROM subject_grade WHERE subject_grade.id = '%s' ) AS grade ON subject_detail.subject_code = grade.subject_code AND subject_detail.opening_year = grade.opening_year AND subject_detail.courses_available = grade.available WHERE subject_detail.courses_available = '%s' AND subject_detail.classification = %d AND grade.grade_point IS NULL GROUP BY subject_detail.subject_name, subject_detail.necessary ORDER BY subject_detail.necessary ASC;", __studentNum, "B110", creditList[j][0]);
        res = PQexec(__con, sql);
        resultRows = PQntuples(res);

        if(needCredit > 0) {

          if(creditList[j][3] == 0) {
            for (int k = 0; k < resultRows; k++) {
              if(atoi(PQgetvalue(res, k, 1)) == 0) {
                sendLen = sprintf(sendBuf, "%s\n", PQgetvalue(res, k, 0));
                printf("%s", sendBuf);
                send(__soc, sendBuf, sendLen, 0);
                // printf("%s\n", PQgetvalue(res, k , 0));
              }
            }

          }else if(creditList[j][3] == 1) {
            int selectFlag = 0;
            for (int k = 0; k < resultRows; k++) {
              if(atoi(PQgetvalue(res, k, 1)) == 0) {
                selectFlag = 1;
                sendLen = sprintf(sendBuf, "%s\n", PQgetvalue(res, k, 0));
                printf("%s", sendBuf);
                send(__soc, sendBuf, sendLen, 0);
                // printf("%s\n", PQgetvalue(res, k, 0));
              }
            }

            if(creditList[j][1] > 0) {
              selectFlag = 1;
              sendLen = sprintf(sendBuf, "**%sから%d単位必要**\n", listName[j], needCredit);
              printf("%s", sendBuf);
              send(__soc, sendBuf, sendLen, 0);
              // printf("**%sから%d単位必要**\n", listName[j], needCredit);
            }
            // if(selectFlag) printf("******\n");
          }
        }
      }
      // printf("******\n");
    }
  }else if (result == 2) {
    sendLen = sprintf(sendBuf, "%s %s %s保留%s", OK_STAT, __studentNum, resultList[__judgeFlag], ENTER);
    printf("%s", sendBuf);
    send(__soc, sendBuf, sendLen, 0);
    // printf("%s %s %s保留%s", OK_STAT, __studentNum, resultList[__judgeFlag], ENTER);
  }else if (result = -1) {
    sendLen = sprintf(sendBuf, "データ未設定%s", ENTER);
    printf("%s", sendBuf);
    send(__soc, sendBuf, sendLen, 0);
    // printf("データ未設定\n");
  }else {
    // TODO: エラー3
    sendLen = sprintf(sendBuf, "%s error_3%s", ER_STAT, ENTER);
    printf("%s", sendBuf);
    send(__soc, sendBuf, sendLen, 0);
    // printf("%s error_3%s", ER_STAT, ENTER);
    return -1;
  }

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
// TODO: SQLの改善
int judge_list(pthread_t __selfId, PGconn *__con, int __soc, UserInfo *__User, int __judgeFlag) {
  PGresult *res;
  char sql[BUFSIZE];
  int resultJudge, resultRows;
  char List[4][BUFSIZE] = {"promotion_judge", "research_judge", "graduation_judge", "completion_judge"};
  int sendLen = 0;
  char sendBuf[BUFSIZE];

  /* 合否取得SQL作成 */
  if(__User->user_level == CLASSTEACHER || __User->user_level == SUBTEACHER) {
    sprintf(sql, "SELECT grade_judge.id, users.person_name, CASE WHEN grade_judge.%s = 0 THEN '可' ELSE '不可' END AS judge FROM grade_judge INNER JOIN users ON grade_judge.id=users.id WHERE (users.school_year = (SELECT school_year FROM users WHERE id = '%s'));", List[__judgeFlag], __User->id);

  }else if(__User->user_level == STAFF || __User->user_level == DEPCHAIR || __User->user_level == ADMIN) {
    sprintf(sql, "SELECT grade_judge.id, users.person_name, CASE WHEN grade_judge.%s = 0 THEN '可' ELSE '不可' END AS judge FROM grade_judge INNER JOIN users ON grade_judge.id=users.id WHERE (grade_judge.id LIKE 'B%%' OR grade_judge.id LIKE 'M%%' OR grade_judge.id LIKE 'D%%');", List[__judgeFlag]);

  }else {
// TODO: エラー4
    sendLen = sprintf(sendBuf, "%s error_4%s", ER_STAT, ENTER);
    printf("%s", sendBuf);
    send(__soc, sendBuf, sendLen, 0);
    // printf("error_4\n");
    return -1;
  }

  /* SQL実行 */
  res = PQexec(__con, sql);
  if(PQresultStatus(res) != PGRES_TUPLES_OK) {
    printf("%s\n", PQresultErrorMessage(res));
//TODO: エラー5
    sendLen = sprintf(sendBuf, "%s error_5%s", ER_STAT, ENTER);
    printf("%s", sendBuf);
    send(__soc, sendBuf, sendLen, 0);
    // printf("error_5\n");
    return -1;
  }

  resultRows = PQntuples(res);
  if(resultRows < 0) {
//TODO: エラー6
    sendLen = sprintf(sendBuf, "%s error_6%s", ER_STAT, ENTER);
    printf("%s", sendBuf);
    send(__soc, sendBuf, sendLen, 0);
    // printf("error_6\n");
    return -1;
  }

  for (int i = 0; i < resultRows; i++) {
    sendLen = sprintf(sendBuf, "%s%s\t%s\t%s\n", sendBuf, PQgetvalue(res, i, 0), PQgetvalue(res, i, 1), PQgetvalue(res, i, 2));
    printf("%s", sendBuf);
    send(__soc, sendBuf, sendLen, 0);
  }

  return 0;
}