
#include "csmm.h"

/**
 * controller
 * メイン処理 (スレッド関数)
 *   [ARGUMENT]
 *      __arg  : スレッド引数構造体(ThreadParameter型)
 *  [RETURN]å
 *     NONE
 **/
void *main_controller(void *__arg){
  ThreadParameter *threadParam = (ThreadParameter *)__arg; //スレッド引数構造体
  char recvBuf[BUFSIZE], sendBuf[BUFSIZE];
  int  recvLen, sendLen;
  pthread_t selfId;                      //自分自身のスレッドID
  char comm[BUFSIZE];    //プロトコル・コマンド
  int flag, cnt;
  UserInfo *User_Info;

  selfId = pthread_self(); //自分自身のスレッドIDを取得
  printf("[C_THREAD %ld] NETBANK CONTROLLER START (%d)\n", selfId, threadParam->soc);

  /* ログイン */
  flag = account_create(selfId, threadParam->con, threadParam->soc, User_Info);

  while(flag){
    /* プロトコル・コマンド受信 */
    recvLen = receive_message(threadParam->soc, recvBuf, BUFSIZE);
    if( recvLen < 1 )
      break;
    recvBuf[recvLen-1] = '\0'; // <LF>を消去
    printf("[C_THREAD %ld] RECV=> %s\n", selfId, recvBuf);

    /* プロトコル・コマンド解析 */
    cnt=sscanf(recvBuf, "%s", comm);
    /* コマンドに対応するモジュールを呼び出す */
    if(strcmp(comm, GPA_CREATE) == 0){
      /* 個人ＧＰＡ算出 */
      // gpa_create(selfId, threadParam->con, threadParam->soc, recvBuf, User_Info);

    }else if(strcmp(comm, PERSONAL_GRADE)==0){
      /* 個人成績閲覧 */
      // personal_grade(selfId, threadParam->con, threadParam->soc, recvBuf, User_Info);

    }else if(strcmp(comm, ALL_GRADE) == 0){
      /* 成績一覧 */
      all_grade(selfId, threadParam->con, threadParam->soc, recvBuf, sendBuf, User_Info);

    }else if(strcmp(comm, SUBJECT_REGIST) == 0){
      /* 科目登録 */
      subject_regist(selfId, threadParam->con, threadParam->soc, recvBuf, sendBuf, User_Info);

    }else if(strcmp(comm, INPUT_COURSE) == 0){
      /* 進路入力 */
      input_course(selfId, threadParam->con, threadParam->soc, sendBuf, User_Info);

    }else if(strcmp(comm, PJUDGE) == 0){
      /* 卒業着手判定*/
      judge_main(selfId, threadParam->con, threadParam->soc, User_Info, 0, recvBuf);

    }else if(strcmp(comm, GJUDGE) == 0){
      /* 卒業判定 */
      judge_main(selfId, threadParam->con, threadParam->soc, User_Info, 1, recvBuf);

    }else if(strcmp(comm, CJUDGE) == 0){
      /* 終了判定 */
      judge_main(selfId, threadParam->con, threadParam->soc, User_Info, 2, recvBuf);

    }else if(strcmp(comm, GRADUATE_COUNT) == 0){
      /* 卒業者・留年者 */
      graduate_count(selfId, threadParam->con, threadParam->soc, recvBuf, sendBuf, User_Info);

    }else{
      sprintf(sendBuf, "%s %d%s", ER_STAT, E_CODE_3, ENTER);
    }

    if(flag < 0){
      /* エラーレスポンスを送信 */
      sendLen = strlen(sendBuf);
      send(threadParam->soc, sendBuf, sendLen, 0);
      printf("[C_THREAD %ld] SEND=> %s", selfId, sendBuf);
    }

    /* レスポンスの終了を現す「.\n]を送信 */
    sendLen = sprintf(sendBuf, "%s", ENTER);
    send(threadParam->soc, sendBuf, sendLen, 0);
    printf("[C_THREAD %ld] SEND=> %s\n", selfId, sendBuf);
  }//END while()

  printf("[C_THREAD %ld] NETBANK CONTROLLER END (%d)\n\n", selfId, threadParam->soc);

  /* データベース接続を切断 */
  PQfinish(threadParam->con);
  /* ソケットを切断 */
  close(threadParam->soc);
  /* スレッド引数構造の領域解放 */
  free(threadParam);
}//END
