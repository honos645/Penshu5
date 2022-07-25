#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmss.h"
#include "personal_grade.h"
#include <postgresql/libpq-fe.h>
/* int personal_grade(pthread_t __selfId, PGconn *con, int __soc, char * __recvBuf,UserInfo __User_Info); */

int personal_grade(pthread_t __selfId, PGconn *con, int __soc, char * __recvBuf,UserInfo *__User_Info){

  PGresult *res;
  char recvBuf[BUFSIZE], sendBuf[BUFSIZE], comm[BUFSIZE], transferText[BUFSIZE];
  int resultRows, n, sendLen;

  char q_id[BUFSIZE];
  char subject_name[BUFSIZE][BUFSIZE], rating[BUFSIZE][BUFSIZE];
  int credit[BUFSIZE], grade_point[BUFSIZE], retake[BUFSIZE];

  char sql[BUFSIZE], sql_stu_name[BUFSIZE], sql_stu_department[BUFSIZE], sql_stu_major[BUFSIZE];
  int sql_stu_level, sql_stu_year;

  int i, tmp, sum = 0, bottom = 0;
  double gpa;

  /********* ここからIPOのプロセス処理 *********/

  /* コマンド入力（入力）*/
  n = sscanf(__recvBuf, "%s %s", comm, q_id);

  if(n != 2){
    sendLen = sprintf(sendBuf,"%s %d%s", ER_STAT, E_CODE_5, ENTER);
    printf("%s", sendBuf);
    send(__soc, sendBuf, sendLen, 0);
        printf("[C_THREAD %ld] SEND=> %s", __selfId, sendBuf);
    return -1;
  }

  /*スキーマ検索パスを変更 */
  //sprintf(sql, "SET search_path to cmss");
  //PQexec(con, sql);

  //get studentinfo
  sprintf(sql, "SELECT id, person_name, user_level, department, major, school_year FROM users WHERE id = %s;", q_id);
  res = PQexec(con, sql);
  if(PQresultStatus(res) != PGRES_TUPLES_OK){
    printf("%s\n", PQresultErrorMessage(res));
    sendLen = sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_1, ENTER);
    printf("%s", sendBuf);
    send(__soc, sendBuf, sendLen, 0);
    printf("[C_THREAD %ld] SEND=> %s", __selfId, sendBuf);
    return -1;
  }
  strcpy(sql_stu_name, PQgetvalue(res, 0, 1));
  sql_stu_level = atoi(PQgetvalue(res, 0, 2));
  strcpy(sql_stu_department, PQgetvalue(res, 0, 3));
  strcpy(sql_stu_major, PQgetvalue(res, 0, 4));
  sql_stu_year = atoi(PQgetvalue(res, 0, 5));

  //check userlevel
  if(__User_Info->user_level != ADMIN || __User_Info->user_level != CLERK || __User_Info->user_level != TEACH_COM || __User_Info->user_level != CHAIR){
    if(__User_Info->user_level == STUDENT && strcmp(__User_Info->id, q_id)!=0){
      sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_4, ENTER);
      printf("%s", sendBuf);
      send(__soc, sendBuf, BUFSIZE, 0);
      printf("[C_THREAD %ld] SEND=> %s", __selfId, sendBuf);
      return -1;
    }else if(!(__User_Info->user_level == TEACH_HR || __User_Info->user_level == TEACH_VHR
	       && __User_Info->school_year == sql_stu_year
	       && strcmp(__User_Info->department, sql_stu_department) == 0
	       && __User_Info->major == sql_stu_major
	       && __User_Info->school_year == sql_stu_year)){
      sendLen = sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_4, ENTER);
      printf("%s", sendBuf);
      send(__soc, sendBuf, sendLen, 0);
      printf("[C_THREAD %ld] SEND=> %s", __selfId, sendBuf);
      return -1;
    }
  }

  //Do SQL
  sprintf(sql,"SELECT sg.id, sg.subject_code, sg.opening_year, sd.subject_name, sd.credit, sg.grade_point, sg.retake "
	  "FROM subject_grade AS sg "
	  "INNER JOIN subject_detail AS sd "
	  "ON sg.subject_code = sd.subject_code "
	  "AND sg.opening_year = sd.opening_year WHERE id = %s ;", q_id);

  //recieve
  res = PQexec(con, sql);
  if(PQresultStatus(res) != PGRES_TUPLES_OK){
    printf("%s\n", PQresultErrorMessage(res));
    sendLen = sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_4, ENTER);
    printf("%s", sendBuf);
    send(__soc, sendBuf, sendLen, 0);
    printf("[C_THREAD %ld] SEND=> %s", __selfId, sendBuf);
    return -1;
  }

  resultRows = PQntuples(res);
  for(i=0; i<resultRows; i++){
    strcpy(subject_name[i],PQgetvalue(res, i, 3));
    credit[i] = atoi(PQgetvalue(res, i, 4));
    grade_point[i] = atoi(PQgetvalue(res, i, 5));
    retake[i] = atoi(PQgetvalue(res, i, 6));

    if(grade_point[i] < 60){
      strcpy(rating[i], "否");
    }else if(grade_point[i] == 60 && retake[i] == 1){
      strcpy(rating[i], "可(再)");
    }else if(grade_point[i] >= 60 && grade_point[i] < 70){
      strcpy(rating[i], "可");
    }else if(grade_point[i] >= 70 && grade_point[i] < 80){
      strcpy(rating[i], "良");
    }else if(grade_point[i] >= 80 && grade_point[i] < 90){
      strcpy(rating[i], "優");
    }else if(grade_point[i] >= 90 && grade_point[i] <= 100){
      strcpy(rating[i], "秀");
    }else if(grade_point[i] == ATTEND_LESS){
      strcpy(rating[i], "出席不足");
    }else if(grade_point[i] == NOT_TAKEN){
      strcpy(rating[i], "未受験");
    }else{
      strcpy(rating[i], "履修中");
    }
  }

  // calc GPA
  for (i = 0; i < resultRows; i++)
    {
      if (grade_point[i] >= 0 && grade_point[i] < 60)
	{
	  tmp = 0;
	}
      else if (grade_point[i] >= 60 && grade_point[i] <= 100)
	{
	  tmp = (grade_point[i] - 54.5) / 10;
	}
      else
	{
	  continue;
	}

      sum += tmp * credit[i];
      bottom += credit[i];
    }
  gpa = sum / bottom;

  //display
  printf("%sさんの履修状況\n",sql_stu_name);
  for(i=0; i<resultRows; i++){
    sendLen = sprintf(sendBuf, "%s %s\n",subject_name[i],rating[i]);
    printf("%s", sendBuf);
    send(__soc, sendBuf, sendLen, 0);
    printf("[C_THREAD %ld] SEND=> %s", __selfId, sendBuf);
  }
  sendLen = sprintf(sendBuf, "GPA : %lf", gpa);
  printf("%s", sendBuf);
  send(__soc, sendBuf, sendLen, 0);
  printf("[C_THREAD %ld] SEND=> %s", __selfId, sendBuf);

  return 0;
}
