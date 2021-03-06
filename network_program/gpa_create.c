
//gpa_create

#include "cmss.h"
#include "gpa_create.h"

int gpa_create(pthread_t __selfId, PGconn *con, int __soc, char * __recvBuf,UserInfo *__User_Info){

  char recvBuf[BUFSIZE], sendBuf[BUFSIZE], comm[BUFSIZE], transferText[BUFSIZE];
  int resultRows, n, sendLen;

  PGresult *res;
  char q_id[256];

  char sql[BUFSIZE], sql_stu_name[BUFSIZE];
  int sql_stu_level, sql_stu_year, sql_stu_retakem, sql_stu_department, sql_stu_major;

  int i, j , tmp, sum = 0, bottom = 0;
  /********* ここからIPOのプロセス処理 *********/


  /* コマンド入力（入力）*/
  n = sscanf(__recvBuf, "%s %s", comm, q_id);

  if(n != 2){
    sendLen = sprintf(sendBuf,"%s %d%s", ER_STAT, E_CODE_5, ENTER);
    printf("%s", sendBuf);
    // send(__soc, sendBuf, sendLen, 0);
    return 0;
  }

  //get studentinfo
  sprintf(sql, "SELECT id, person_name, user_level, department, major, school_year FROM users WHERE id = '%s';", q_id);
  res = PQexec(con, sql);
  if(PQresultStatus(res) != PGRES_TUPLES_OK){
    printf("%s\n", PQresultErrorMessage(res));
    sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_2, ENTER);
    // printf("%s", sendBuf);
    send(__soc, sendBuf, sendLen, 0);
    return 0;
  }

  strcpy(sql_stu_name, PQgetvalue(res, 0, 1));
  sql_stu_level = atoi(PQgetvalue(res, 0, 2));
  sql_stu_department = atoi(PQgetvalue(res, 0, 3));
  sql_stu_major = atoi(PQgetvalue(res, 0, 4));
  sql_stu_year = atoi(PQgetvalue(res, 0, 5));
  printf("%s %d %d %d %d\n", sql_stu_name, sql_stu_level, sql_stu_department, sql_stu_major, sql_stu_year);

  //check userlevel
  if(__User_Info->user_level != ADMIN || __User_Info->user_level != CLERK || __User_Info->user_level != TEACH_COM || __User_Info->user_level != CHAIR){
    if(__User_Info->user_level == STUDENT && strcmp(__User_Info->id, q_id)!=0){
      sendLen = sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_4, ENTER);
      // printf("%s", sendBuf);
      send(__soc, sendBuf, sendLen, 0);
      printf("[C_THREAD %ld] SEND=> %s", __selfId, sendBuf);
      return 0;
    }else if(__User_Info->user_level == TEACH_HR || __User_Info->user_level == TEACH_VHR
         && !(__User_Info->school_year == sql_stu_year
         && atoi(__User_Info->department) == sql_stu_department
         && atoi(__User_Info->major) == sql_stu_major
         && __User_Info->school_year == sql_stu_year)){
      sendLen = sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_4, ENTER);
      // printf("%s", sendBuf);
      send(__soc, sendBuf, sendLen, 0);
      printf("[C_THREAD %ld] SEND=> %s", __selfId, sendBuf);
      return 0;
    }
  }

  //Do SQL
  sprintf(sql,"SELECT sg.id, sg.subject_code, sg.opening_year, sd.subject_name, sd.classification ,sd.necessary , sd.credit, sg.grade_point "
    "FROM subject_grade AS sg "
    "INNER JOIN subject_detail AS sd "
    "ON sg.subject_code = sd.subject_code "
    "AND sg.opening_year = sd.opening_year WHERE sg.id = '%s' ;", q_id);

  //recieve
  res = PQexec(con, sql);
  if(PQresultStatus(res) != PGRES_TUPLES_OK){
    printf("%s\n", PQresultErrorMessage(res));
    sendLen = sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_1, ENTER);
    printf("%s", sendBuf);
    send(__soc, sendBuf, sendLen, 0);
    printf("[C_THREAD %ld] SEND=> %s", __selfId, sendBuf);
    return 0;
  }
  resultRows = PQntuples(res);
  if(resultRows == 0){
    sendLen = sprintf(sendBuf, "%s 講義が登録されていません %s", ER_STAT, ENTER);
    send(__soc, sendBuf, sendLen, 0);
    printf("[C_THREAD %ld] SEND=> %s", __selfId, sendBuf);
    return 0;
  }

  double gpa[NUM_SUB][TERMS];
  char stu_ent_year[BUFSIZE];
  double gp[NUM_SUB][TERMS];
  int credits[NUM_SUB][TERMS];
  int term;
  for(i=0; i<NUM_SUB; i++){
    for(j=0; j<TERMS; j++){
      gp[i][j] = 0;
      gpa[i][j] = 0;
      credits[i][j] = 0;
    }
  }

  sprintf(stu_ent_year, "20%c%c0", q_id[3], q_id[4]);
  //テスト用
  //printf("%s %s %s %s %s %s %s %s\n", PQgetvalue(res, 0, 1), PQgetvalue(res, 0, 1), PQgetvalue(res, 0, 2), PQgetvalue(res, 0, 3), PQgetvalue(res, 0, 4), PQgetvalue(res, 0, 5), PQgetvalue(res, 0, 6), PQgetvalue(res, 0, 7));


  for(i=0; i<resultRows; i++){
    tmp =   atoi(PQgetvalue(res, i, 2)) - atoi(stu_ent_year);
    if(tmp % 2 == 1) tmp += 2;
    term = tmp / 10 * 2 + tmp % 10;

    //全科目
    int grade = atoi(PQgetvalue(res, i, 7));
    if(grade == 666) continue;
    if(grade < 60 || grade > 100) continue;
    if(grade > 100) grade = 0;
    gp[ALL][term] += (grade - 54.5) / 10 * atoi(PQgetvalue(res, i, 6));
    credits[ALL][term] += atoi(PQgetvalue(res, i, 6));

    //テスト用
    //printf("gp: %1.2lf term: %d grade: %d credit: %d\n", (grade - 54.5) / 10 * atoi(PQgetvalue(res, i, 6)), term, grade, credits[ALL][term]);

    int subclass = atoi(PQgetvalue(res, i, 4));
    int nes = atoi(PQgetvalue(res, i, 5));
    //各区分
    if(subclass > 10 && subclass < 54){
      /* 基礎教育 */
      gp[BASEEDU][term] += (grade - 54.5) / 10 * atoi(PQgetvalue(res, i, 6));
      credits[BASEEDU][term] += atoi(PQgetvalue(res, i, 6));
    }else if(subclass > 60 && subclass < 72){
      /* 専門科目 */
      gp[TECHNIC][term] += (grade - 54.5) / 10 * atoi(PQgetvalue(res, i, 6));
      credits[TECHNIC][term] += atoi(PQgetvalue(res, i, 6));
    }else if(subclass > 50 && subclass < 60){
      /* 工学基礎 */
      gp[BASEEGN][term] += (grade - 54.5) / 10 * atoi(PQgetvalue(res, i, 6));
      credits[BASEEGN][term] += atoi(PQgetvalue(res, i, 6));
    }else if(subclass == 61){
      /* 専門必修 */
      gp[TECHNES][term] += (grade - 54.5) / 10 * atoi(PQgetvalue(res, i, 6));
      credits[TECHNES][term] += atoi(PQgetvalue(res, i, 6));
    }else if(subclass == 62){
      /* 専門選択 */
      gp[TECHSEL][term] += (grade - 54.5) / 10 * atoi(PQgetvalue(res, i, 6));
      credits[TECHSEL][term] += atoi(PQgetvalue(res, i, 6));
    }
  }

  char termel[256], subclass[256];
  // calc GPA
  for (i = 0; i < NUM_SUB; i++)
    {
      for (j = 0; j < TERMS; j++)
  {
    if(credits[i][j] == 0) continue;
    printf("%d %d gpa: %1.2lf credits: %d\n", i, j, gp[i][j], credits[i][j]);
    gpa[i][j] = gp[i][j] / credits[i][j];
    if (j % 2 == 0)
      {
        sprintf(termel, "前期");
      }
    else
      {
        sprintf(termel, "後期");
      }
    if (i == ALL)
      {
        sprintf(subclass, "全科目合計");
      }
    else if (i == BASEEDU)
      {
        sprintf(subclass, "基礎教育科目");
      }
    else if (i == TECHNIC)
      {
        sprintf(subclass, "専門科目");
      }
    else if (i == BASEEGN)
      {
        sprintf(subclass, "工学基礎科目");
      }
    else if (i == TECHNES)
      {
        sprintf(subclass, "専門必修科目");
      }
    else if (i == TECHSEL)
      {
        sprintf(subclass, "専門選択科目");
      }

    sendLen = sprintf(sendBuf, "%d %s %s: %1.2lf\n", (j / 2 + atoi(stu_ent_year))/10, termel, subclass, gpa[i][j]);
    printf("%s", sendBuf);
    send(__soc, sendBuf, sendLen, 0);
    printf("[C_THREAD %ld] SEND=> %s", __selfId, sendBuf);
  }
    }

  return 0;
}
