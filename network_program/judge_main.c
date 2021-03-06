#include"judge.h"
/**
 * @brief 判定関数
 * 個人表示か一覧で表示かユーザレベルで振り分ける
 * @param __selfId スレッドID
 * @param __con データベース接続情報
 * @param __soc ソケットディスクリプタ
 * @param __User ユーザ情報構造体
 * @param __sendBuf コマンド、引数の格納された配列
 * @param __recvBuf エラー時エラー文格納配列
 * @param __judgeFlag 判定フラグ 0:卒業研究, 1:卒業, 2:修了
 * @return int 0:成功, -1:失敗
 */
int judge_main(pthread_t __selfId, PGconn *__con, int __soc, UserInfo *__User, char *__sendBuf, char *__recvBuf, int __judgeFlag) {
  char sql[BUFSIZE];
  char sendBuf[BUFSIZE], comm[BUFSIZE], transferText[BUFSIZE];
  char temp[BUFSIZE], commArg[BUFSIZE];
  int resultRows, cmdArgc, sendLen, errorFlag = 0;

  /*NOTE:コマンド解析*/
  cmdArgc = sscanf(__recvBuf, "%s %s", comm, commArg);

  //NOTE: 引数エラー1
  if(cmdArgc != 2) {
    sendLen = sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_5, ENTER);
    send(__soc, __sendBuf, sendLen, 0);
    printf("[C_THREAD %ld] SEND=> %s", __selfId, __sendBuf);
    return -1;
  }

  /*NOTE: ユーザレベル制限*/
  switch(__User->user_level) {
    case STUDENT: /*学生*/
      if(!strcmp(__User->id, commArg)){
        if(__judgeFlag == PROMOTION) {
          errorFlag = judge_personal(__selfId, __con, __soc, commArg, PROMOTION, __sendBuf);
        }else if(__judgeFlag == GRADUATION) {
          errorFlag = judge_personal(__selfId, __con, __soc, commArg, GRADUATION, __sendBuf);
        }else if(__judgeFlag == COMPLETION) {
          errorFlag = judge_personal(__selfId, __con, __soc, commArg, COMPLETION, __sendBuf);
        }
      }else {
        sendLen = sprintf(__sendBuf, "%s %d%s", ER_STAT, E_CODE_5, ENTER);
        printf("%s", __sendBuf);
        errorFlag = -1;
      }
      break;
    case CLERK:/*事務職員*/
      if(strlen(commArg) == 4){
        if(__judgeFlag == PROMOTION) {
          errorFlag = judge_list(__selfId, __con, __soc, __User, commArg, PROMOTION, __sendBuf);
        }else if(__judgeFlag == GRADUATION) {
          errorFlag = judge_list(__selfId, __con, __soc, __User, commArg, GRADUATION, __sendBuf);
        }else if(__judgeFlag == COMPLETION) {
          errorFlag = judge_list(__selfId, __con, __soc, __User, commArg, COMPLETION, __sendBuf);
        }
      }else if(!strcmp(commArg, "00000000")){
        if(__judgeFlag == PROMOTION) {
          errorFlag = judge_list(__selfId, __con, __soc, __User, commArg, PROMOTION, __sendBuf);
        }else if(__judgeFlag == GRADUATION) {
          errorFlag = judge_list(__selfId, __con, __soc, __User, commArg, GRADUATION, __sendBuf);
        }else if(__judgeFlag == COMPLETION) {
          errorFlag = judge_list(__selfId, __con, __soc, __User, commArg, COMPLETION, __sendBuf);
        }
      }else if(strlen(commArg) == 8) {
        if(__judgeFlag == PROMOTION) {
          errorFlag = judge_personal(__selfId, __con, __soc, commArg, PROMOTION, __sendBuf);
        }else if(__judgeFlag == GRADUATION) {
          errorFlag = judge_personal(__selfId, __con, __soc, commArg, GRADUATION, __sendBuf);
        }else if(__judgeFlag == COMPLETION) {
          errorFlag = judge_personal(__selfId, __con, __soc, commArg, COMPLETION, __sendBuf);
        }
      }
      break;
    case TEACH:/*教員、学科長、管理者*/
    case TEACH_HR:
    case TEACH_VHR:
    case TEACH_COM:
    case TEACH_W_HR:
    case TEACH_W:
    case CHAIR:
    case ADMIN:
      if(!strcmp(commArg, "00000000")){
        if(__judgeFlag == PROMOTION) {
          errorFlag = judge_list(__selfId, __con, __soc, __User, commArg, PROMOTION, __sendBuf);
        }else if(__judgeFlag == GRADUATION) {
          errorFlag = judge_list(__selfId, __con, __soc, __User, commArg, GRADUATION, __sendBuf);
        }else if(__judgeFlag == COMPLETION) {
          errorFlag = judge_list(__selfId, __con, __soc, __User, commArg, COMPLETION, __sendBuf);
        }
      }else {
        if(__judgeFlag == PROMOTION) {
          errorFlag = judge_personal(__selfId, __con, __soc, commArg, PROMOTION, __sendBuf);
        }else if(__judgeFlag == GRADUATION) {
          errorFlag = judge_personal(__selfId, __con, __soc, commArg, GRADUATION, __sendBuf);
        }else if(__judgeFlag == COMPLETION) {
          errorFlag = judge_personal(__selfId, __con, __soc, commArg, COMPLETION, __sendBuf);
        }
      }
      break;
  }

  return errorFlag;
}// main End

/**
 * @brief 個人判定
 *
 * @param __selfId スレッドID
 * @param __con データベース接続情報
 * @param __soc ソケットディスクリプタ
 * @param __studentNum 学籍番号
 * @param __judgeFlag 判定フラグ 0:卒業研究, 1:卒業, 2:修了
 * @param __errorBuf エラー時エラー文格納配列
 * @return int 0:成功, -1:失敗
 */
int judge_personal(pthread_t __selfId, PGconn *__con, int __soc, char *__studentNum, int __judgeFlag, char *__errorBuf) {
  PGresult *res;
  int result, resultRows;
  char sql[BUFSIZE], temp[BUFSIZE];
  char judgeList[3][BUFSIZE] = {"research_judge", "graduation_judge", "completion_judge"};
  char creditTable[3][BUFSIZE] = {"advance", "graduate", ""};
  char resultList[3][BUFSIZE] = {"着手", "卒業", "修了"};
  int sendLen = 0;
  char sendBuf[BUFSIZE];

  /* 合否取得SQL作成 例:personalJudge.sql*/
  sprintf(sql,  "SELECT grade_judge.id, users.person_name, grade_judge.%s "
                "FROM grade_judge INNER JOIN users ON grade_judge.id = users.id WHERE grade_judge.id = '%s';"
                , judgeList[__judgeFlag], __studentNum);

  /* SQL実行 */
  res = PQexec(__con, sql);
  if(PQresultStatus(res) != PGRES_TUPLES_OK) {
    printf("%s\n", PQresultErrorMessage(res));
    sendLen = sprintf(__errorBuf, "%s %d%s", ER_STAT, E_CODE_7, ENTER);
    return -1;
  }

  resultRows = PQntuples(res);
  if(resultRows != 1) {
//NOTE: エラー2
    sendLen = sprintf(__errorBuf, "%s %d%s", ER_STAT, E_CODE_7, ENTER);
    return -1;
  }

  /*NOTE: 判定結果を表示*/
  result = atoi(PQgetvalue(res, 0, 2));
  if(result == PASSING) {
    /*可*/
    sendLen = sprintf(sendBuf, "%s %s %s可能%s", OK_STAT, __studentNum, resultList[__judgeFlag], ENTER);
    send(__soc, sendBuf, sendLen, 0);
    printf("[C_THREAD %ld] SEND=> %s", __selfId, sendBuf);

  }else if(result == FAILURE) {
    /*不可*/
    int flag = 0;
    if(__studentNum[3] < '2' || (__studentNum[3] == '2' && __studentNum[4] == '0')) {
      sprintf(temp, "%c%c%c0", __studentNum[0], __studentNum[1], __studentNum[2]);
    }else {
      sprintf(temp, "%c%c%c1", __studentNum[0], __studentNum[1], __studentNum[2]);
    }
    /* 単位数比較表SQL 例:personalJudge2.sql */
    sprintf(sql, "SELECT classification_name.name ,subject_classification_%s.classification, subject_classification_%s.required_credit, CASE WHEN credit.sum IS NULL THEN 0 ELSE credit.sum END, subject_classification_%s.absolute_option FROM subject_classification_%s "
                  "LEFT OUTER JOIN(SELECT sd.classification, SUM(sd.credit) AS SUM FROM subject_detail AS sd LEFT OUTER JOIN(SELECT subject_grade.subject_code, subject_grade.opening_year, subject_grade.id, CONCAT(SUBSTRING(subject_grade.id, 1, 3), SUBSTRING(subject_grade.id, 5, 1)) AS available,"
                  "subject_grade.grade_point FROM subject_grade WHERE subject_grade.id = '%s') AS grade ON sd.subject_code = grade.subject_code AND sd.opening_year = grade.opening_year AND sd.courses_available = grade.available "
                  "WHERE sd.courses_available = '%s' AND (grade.grade_point >= 60 AND grade.grade_point <= 100) GROUP BY sd.classification) AS credit ON subject_classification_%s.classification = credit.classification INNER JOIN classification_name on subject_classification_%s.classification = classification_name.code;"
                  , creditTable[__judgeFlag], creditTable[__judgeFlag], creditTable[__judgeFlag], creditTable[__judgeFlag], __studentNum, temp, creditTable[__judgeFlag], creditTable[__judgeFlag]);

    /*SQL実行*/
    res = PQexec(__con, sql);
    if(PQresultStatus(res) != PGRES_TUPLES_OK) {
      printf("%s\n", PQresultErrorMessage(res));
      sendLen = sprintf(__errorBuf, "%s %d%s", ER_STAT, E_CODE_7, ENTER);
      return -1;
    }
    resultRows = PQntuples(res);

    int creditList[resultRows][4];
    char listName[resultRows][BUFSIZE];

    /*NOTE: 不足単位数を配列に格納*/
    for (int i = 0; i < resultRows; i++) {
      strcpy(listName[i], PQgetvalue(res, i, 0));
      for (int j = 1; j < 5; j++) {
        creditList[i][j - 1] = atoi(PQgetvalue(res, i, j));
      }
    }

    sendLen = sprintf(sendBuf, "%s %s %s不可%s", OK_STAT, __studentNum, resultList[__judgeFlag], ENTER);
    send(__soc, sendBuf, sendLen, 0);
    printf("[C_THREAD %ld] SEND=> %s", __selfId, sendBuf);
    sendLen = sprintf(sendBuf, "必要単位%s", ENTER);
    send(__soc, sendBuf, sendLen, 0);
    printf("[C_THREAD %ld] SEND=> %s", __selfId, sendBuf);


    for (int i = 0; i < sizeof(creditList) / sizeof(*creditList); i++) {
      if(creditList[i][0] % 10 != 0) continue;
      for (int j = i; creditList[j][0] < creditList[i][0] + 10; j++) {
        int needCredit = creditList[j][1] - creditList[j][2];
        creditList[i][2] += creditList[j][2];

        /* SQL例 needCreditSum.sql */
        sprintf(sql,  "SELECT subject_detail.subject_name, subject_detail.necessary FROM subject_detail"
                      " LEFT OUTER JOIN (SELECT subject_grade.subject_code, subject_grade.opening_year, subject_grade.id, CONCAT(SUBSTRING(subject_grade.id, 1, 3), SUBSTRING(subject_grade.id, 5, 1)) AS available, subject_grade.grade_point FROM subject_grade WHERE subject_grade.id = '%s' ) AS grade "
                      "ON subject_detail.subject_code = grade.subject_code AND subject_detail.opening_year = grade.opening_year AND subject_detail.courses_available = grade.available "
                      "WHERE subject_detail.courses_available = '%s' AND subject_detail.classification = %d AND grade.grade_point IS NULL GROUP BY subject_detail.subject_name, subject_detail.necessary ORDER BY subject_detail.necessary ASC;"
                      , __studentNum, "B110", creditList[j][0]);

        /*SQL 実行*/
        res = PQexec(__con, sql);
        if(PQresultStatus(res) != PGRES_TUPLES_OK) {
          printf("%s\n", PQresultErrorMessage(res));
          sendLen = sprintf(__errorBuf, "%s %d%s", ER_STAT, E_CODE_7, ENTER);
          return -1;
        }
        resultRows = PQntuples(res);

        if(needCredit > 0) {
          if(creditList[j][3] == 0) {
            /*NOTE: 必修科目の講義名表示*/
            for (int k = 0; k < resultRows; k++) {
              if(atoi(PQgetvalue(res, k, 1)) == 0) {
                if(__judgeFlag == PROMOTION && (!strcmp(PQgetvalue(res, k, 0), "卒業研究") || !strcmp(PQgetvalue(res, k, 0), "情報工学セミナー"))) continue;
                sendLen = sprintf(sendBuf, "%s%s", PQgetvalue(res, k, 0), ENTER);
                send(__soc, sendBuf, sendLen, 0);
                printf("[C_THREAD %ld] SEND=> %s", __selfId, sendBuf);
              }
            }
          }else if(creditList[j][3] == 1) {
            int selectFlag = 0;
            /*NOTE: 選択科目の講義名表示*/
            for (int k = 0; k < resultRows; k++) {
              if(atoi(PQgetvalue(res, k, 1)) == 0) {
                selectFlag = 1;
                sendLen = sprintf(sendBuf, "%s%s", PQgetvalue(res, k, 0), ENTER);
                send(__soc, sendBuf, sendLen, 0);
                printf("[C_THREAD %ld] SEND=> %s", __selfId, sendBuf);
              }
            }
            if(creditList[j][1] > 0) {
              selectFlag = 1;
              sendLen = sprintf(sendBuf, "**%sから%d単位必要**%s", listName[j], needCredit, ENTER);
              send(__soc, sendBuf, sendLen, 0);
              printf("[C_THREAD %ld] SEND=> %s", __selfId, sendBuf);
            }
          }
        }
      }
    }
  }else if (result == PENDING) {
    /*保留*/
    sendLen = sprintf(sendBuf, "%s %s %s保留%s", OK_STAT, __studentNum, resultList[__judgeFlag], ENTER);
    send(__soc, sendBuf, sendLen, 0);
    printf("[C_THREAD %ld] SEND=> %s%s", __selfId, sendBuf, ENTER);
  }else {
    /*エラー処理*/
    sendLen = sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_7, ENTER);
    return -1;
  }

  return 0;
}// judge_personal End

/**
 * @brief 不足単位の講義を一覧で表示
 *
 * @param __selfId スレッドID
 * @param __con データベース接続情報
 * @param __soc ソケットディスクリプタ
 * @param __User ユーザ情報構造体
 * @param __recvBuf 不明多分使ってない
 * @param __judgeFlag 判定フラグ 0:卒業研究, 1:卒業, 2:修了
 * @param __errorBuf エラー時エラー文格納配列
 * @return int 0:成功, -1:失敗
 */
int judge_list(pthread_t __selfId, PGconn *__con, int __soc, UserInfo *__User, char *__recvBuf, int __judgeFlag, char *__errorBuf) {
  PGresult *res;
  char sql[BUFSIZE];
  int resultJudge, resultRows;
  char List[4][BUFSIZE] = {"promotion_judge", "research_judge", "graduation_judge", "completion_judge"};
  int sendLen = 0;
  char sendBuf[BUFSIZE];

  /* 合否取得SQL作成 */
  if(__User->user_level == TEACH_HR || __User->user_level == TEACH_VHR || __User->user_level == TEACH_COM) {
    sprintf(sql,  "SELECT grade_judge.id, users.person_name, CASE WHEN grade_judge.%s = 0 THEN '可' ELSE '不可' END AS judge "
                  "FROM grade_judge INNER JOIN users ON grade_judge.id=users.id "
                  "WHERE (users.school_year = (SELECT school_year FROM users WHERE id = '%s'));"
                  , List[__judgeFlag], __User->id);

  }else if(__User->user_level == CHAIR || __User->user_level == ADMIN) {
    sprintf(sql,  "SELECT grade_judge.id, users.person_name, CASE WHEN grade_judge.%s = 0 THEN '可' ELSE '不可' END AS judge "
                  "FROM grade_judge INNER JOIN users ON grade_judge.id=users.id "
                  "WHERE (grade_judge.id LIKE 'B%%' OR grade_judge.id LIKE 'M%%' OR grade_judge.id LIKE 'D%%');"
                  , List[__judgeFlag]);

  }else if(__User->user_level == CLERK) {
    if(strlen(__recvBuf) == 4) {
      sprintf(sql,  "SELECT grade_judge.id, users.person_name, CASE WHEN grade_judge.%s = 0 THEN '可' ELSE '不可' END AS judge "
                    "FROM grade_judge INNER JOIN users ON grade_judge.id=users.id "
                    "WHERE grade_judge.id LIKE 'B%c%c%s%%' OR grade_judge.id LIKE 'M%c%c%s%%' OR grade_judge.id LIKE 'D%c%c%s%%';", List[__judgeFlag], __User->id[1], __User->id[2], __recvBuf + 2, __User->id[1], __User->id[2], __recvBuf + 2, __User->id[1], __User->id[2], __recvBuf + 2);
    }else {
      sprintf(sql,  "SELECT grade_judge.id, users.person_name, CASE WHEN grade_judge.%s = 0 THEN '可' ELSE '不可' END AS judge "
                    "FROM grade_judge INNER JOIN users ON grade_judge.id=users.id "
                    "WHERE (grade_judge.id LIKE 'B%%' OR grade_judge.id LIKE 'M%%' OR grade_judge.id LIKE 'D%%');"
                    , List[__judgeFlag]);
    }

  }else {
    // NOTE: エラー3
    sendLen = sprintf(__errorBuf, "%s %d%s", ER_STAT, E_CODE_4, ENTER);
    return -1;
  }

  /* SQL実行 */
  res = PQexec(__con, sql);
  if(PQresultStatus(res) != PGRES_TUPLES_OK) {
    printf("%s\n", PQresultErrorMessage(res));
    //NOTE: エラー4
    sendLen = sprintf(__errorBuf, "%s %d%s", ER_STAT, E_CODE_7, ENTER);
    return -1;
  }

  resultRows = PQntuples(res);
  if(resultRows < 1) {
    //NOTE: エラー5
    sendLen = sprintf(__errorBuf, "%s %d%s", ER_STAT, E_CODE_7, ENTER);
    return -1;
  }

  /*NOTE: 一覧で表示*/
  for (int i = 0; i < resultRows; i++) {
    sendLen = sprintf(sendBuf, "%s\t%s\t%s%s", PQgetvalue(res, i, 0), PQgetvalue(res, i, 1), PQgetvalue(res, i, 2), ENTER);
    send(__soc, sendBuf, sendLen, 0);
    printf("[C_THREAD %ld] SEND=> %s%s", __selfId, sendBuf, ENTER);
  }

  return 0;
}