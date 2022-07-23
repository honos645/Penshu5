/**
 *  gcc LIGPA_main.c -o LIGPA_main -lpq
 **/

/**
 *  学籍番号順、席次順を入試区分別に表示
 *   [ARGUMENT]
 *    __selfId            : スレッド
 *    __con               : PGconnオブジェクト
 *    __soc               : ソケットディスクリプタ
 *    __department        : 表示する学部
 *    __major             : 表示する学科
 *    __school_year       : 表示する学年
 *    __sendBuf           : 送信用配列のポインタ
 *   [RETURN]
 *    Success   : 0
 *    Error     : -1
 **/

#include "PMSS.h"
#include "LIGPA.h"

int LIGPA(pthread_t __selfId, PGconn *__con, int __soc, char *__recvBuf, char *__sendBuf){
  PGresult *res;
  char sql[BUFSIZE];
  char comm[BUFSIZE];
  int resultRows, n, i, j, sendLen, recvLen;
  int exam;
  //int comm; //コマンド
  char department[BUFSIZE]; //学部
  char major[BUFSIZE]; //学科
  int school_year; //学年
  char sort[BUFSIZE]; //ソート手順
  char *id; //生徒の学籍番号
  char *name; //生徒の名前
  int gpa; //生徒のGPA
  int ranking; //生徒の席次
  int  exam_classification;
  char classification[][BUFSIZE] = {"推薦入試","総合選抜","前期日程","後期日程","編入試（推薦）","編入試（一般）","私費外国人"}; //入試区分の変換
  UserInfo __User_Info;

  /*プロトコル、コマンド引数取得*/
  n = sscanf(__recvBuf, "%s %s %s %d %s", comm, department, major, &school_year, sort);
  if( n != 5 ){
    sprintf(__recvBuf, "%s %d%s", ER_STAT, E_CODE_200, ENTER);
    return -1;
  }

  /*sprintf(sql, "SET search_path to netbank");
    PQexec(__con, sql);*/

  /*recvLen = recive_message(__soc, __recvBuf, BUFSIZE);
  if( recvLen < 1){
    break;
  }
  __recvBuf[recvLen-1] = '\0';

  n = sscanf(__recvBuf, "%d %d %d %s", &department, &major, &school_year, sort);*/

  __User_Info.level = '0';
  __User_Info.department = '0';
  __User_Info.major = '0';
  __User_Info.school_year = '1';
  
  if( __User_Info.level == '1'){
    printf("error:閲覧権限がありません。\n");
    sprintf(__sendBuf, "%s %d%s", ER_STAT, E_CODE_400, ENTER);
    return -1;
  }

  if( __User_Info.level == '4' || __User_Info.level == '5'){
    if( __User_Info.department != atoi(department) || __User_Info.major != atoi(major) || __User_Info.school_year == school_year ){
      printf("error:閲覧権限がありません。\n");
      sprintf(__sendBuf, "%s %d%s", ER_STAT, E_CODE_400, ENTER);
      return -1;
    }
  }

  if( strcmp(sort, GPA) == 0 || strcmp(sort, ID) == 0){
    if( strcmp(sort, ID) == 0 ){
      sprintf(sql, "SELECT b.id, a.person_name, b.gpa, b.grade_ranking FROM users AS a INNER JOIN grade_judge AS b ON a.id = b.id WHERE department = '%s' AND major = '%s' AND school_year = '%d' ORDER BY b.id", department, major, school_year );
    }else if( strcmp(sort, GPA) == 0 ){
      sprintf(sql, "SELECT b.id, a.person_name, b.gpa, b.grade_ranking FROM users AS a INNER JOIN grade_judge AS b ON a.id = b.id WHERE department = '%s' AND major = '%s' AND school_year = '%d' ORDER BY b.grade_ranking", department, major, school_year );
    }

    printf("[%s]\n", sql);
    res = PQexec(__con, sql);
    if(PQresultStatus(res) != PGRES_TUPLES_OK){
      printf("%s\n", PQresultErrorMessage(res));
      sprintf(__sendBuf, "%s %d%s", ER_STAT, E_CODE_100, ENTER);
      printf("%s", __sendBuf);
      return -1;
    }

    resultRows = PQntuples(res);
    if(resultRows < 0){
      sprintf(__sendBuf, "%s %d%s", ER_STAT, E_CODE_100, ENTER);
      printf("%s", __sendBuf);
      return -1;
    }

    sendLen = sprintf(__sendBuf, "%s %d%s", OK_STAT, resultRows, ENTER);
    send(__soc, __sendBuf, sendLen, 0);
    printf("[C_THREAD %ld] SEND=> %s", __selfId, __sendBuf);

    sendLen = sprintf(__sendBuf, "学籍番号 名前 GPA 席次%s", ENTER);
    send(__soc, __sendBuf, sendLen, 0);
    printf("[C_THREAD %ld] SEND=> %s", __selfId, __sendBuf);
  
    for(i=0; i<resultRows; i++){
      id = PQgetvalue(res, i, 0); //0 番目のフィールド値を取得
      name = PQgetvalue(res, i, 1); //1 番目のフィールド値を取得
      gpa = atoi(PQgetvalue(res, i, 2)); //2 番目のフィールド値を取得
      ranking = atoi(PQgetvalue(res, i, 3)); //3 番目のフィールド値を取得
      sendLen = sprintf(__sendBuf, "%s %s %d %d%s", id, name, gpa, ranking, ENTER);
      /*プロトコル、レスポンスを送信*/
      send(__soc, __sendBuf, sendLen, 0);
      printf("[C_THREAD %ld] SEND=> %s", __selfId, __sendBuf);
    }
  }else if( strcmp(sort, EXAM_GPA) == 0 || strcmp(sort, EXAM_ID) == 0 ){
    sendLen = sprintf(__sendBuf, "学籍番号 名前 GPA 席次 入試区分%s", ENTER);
    send(__soc, __sendBuf, sendLen, 0);
    for(i=0; i<8; i++){ //入試区分ごとに表示する
      if( strcmp(sort, EXAM_ID) == 0 ){ //入試区分別学籍番号順
	sprintf(sql, "SELECT b.id, a.person_name, b.gpa, b.grade_ranking, c.exam_classification FROM users AS a INNER JOIN grade_judge AS b ON a.id = b.id INNER JOIN entrance as c on a.id = c.id WHERE department = '%s' AND major = '%s' AND school_year = '%d' AND exam_classification = %d ORDER BY b.id", department, major, school_year, i);
      }else if( strcmp(sort, EXAM_GPA) == 0 ){ //入試区分別席次順
	sprintf(sql, "SELECT b.id, a.person_name, b.gpa, b.grade_ranking, c.exam_classification FROM users AS a INNER JOIN grade_judge AS b ON a.id = b.id INNER JOIN entrance as c on a.id = c.id WHERE department = '%s' AND major = '%s' AND school_year = '%d' AND exam_classification = '%d' ORDER BY b.grade_ranking", department, major, school_year, i);
      }
      //printf("[%s]\n", sql);
      res = PQexec(__con, sql);
      if(PQresultStatus(res) != PGRES_TUPLES_OK){
	printf("%s\n", PQresultErrorMessage(res));
	sprintf(__sendBuf, "%s %d%s", ER_STAT, E_CODE_100, ENTER);
	printf("%s", __sendBuf);
	return -1;
      }

      resultRows = PQntuples(res);
      if(resultRows < 0){
	sprintf(__sendBuf, "%s %d%s", ER_STAT, E_CODE_100, ENTER);
	printf("%s", __sendBuf);
	return -1;
      }

      /*sendLen = sprintf(__sendBuf, "%s %d%s", OK_STAT, resultRows, ENTER);
	send(__soc, __sendBuf, sendLen, 0);
	printf("[C_THREAD %ld] SEND=> %s", __selfId, __sendBuf);*/

      for(j=0; j<resultRows; j++){
	id = PQgetvalue(res, j, 0); //0 番目のフィールド値を取得
	name = PQgetvalue(res, j, 1); //1 番目のフィールド値を取得
	gpa = atoi(PQgetvalue(res, j, 2)); //2 番目のフィールド値を取得
	ranking = atoi(PQgetvalue(res, j, 3)); //3 番目のフィールド値を取得
	exam_classification = atoi(PQgetvalue(res, j, 4)); //4番目のフィールド値を取得
	  
	sendLen = sprintf(__sendBuf, "%s %s %d %d %s%s", id, name, gpa, ranking, classification[exam_classification], ENTER);
	/*プロトコル、レスポンスを送信*/
	send(__soc, __sendBuf, sendLen, 0);
      }
    }
  }else{
    sprintf(__sendBuf, "%s %d%s", ER_STAT, E_CODE_500, ENTER);
    printf("%s", __sendBuf);
    return -1;
  }
  
  return 0;
}
