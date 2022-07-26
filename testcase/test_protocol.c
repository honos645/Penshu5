/**
 *   test_protocol.c
 **/

#include "test_protocol.h"

#define FG_BLACK   "\x1b[30m"  //文字色を黒
#define FG_RED     "\x1b[31m"  //文字色を赤
#define FG_GREEN   "\x1b[32m"  //文字色を緑
#define FG_YELLOW  "\x1b[33m"  //文字色を黄色
#define FG_BLUE    "\x1b[34m"  //文字色を青
#define FG_MAGENTA "\x1b[35m"  //文字色をマゼンタ
#define FG_CYAN    "\x1b[36m"  //文字色をシアン
#define FG_WHITE   "\x1b[37m"  //文字色を白

#define ESC_DEFAULT "\x1b[0m"  //デフォルトに戻す

#define  clear          printf("\x1b[2J");  //画面をクリア
#define  clear_r        printf("\x1b[0K");  //カーソルから右端までの行をクリア
#define  location(x,y)  printf("\x1b[%d;%dH", x, y) //カーソルの位置を画面(x,y)に移動
#define  cursor_left(x) printf("\x1b[%dD", x)    //カーソルの位置をx文字分、左に移動

#define  fg(c)          printf("%s",c); //文字色を指定
#define  esc_default    printf("%s", ESC_DEFAULT); //デフォルトに戻す

#define CLIENT_MAX 10
#define TEST_MAX 100
#define TESTCASE_DATA "testcase001.txt"

#define PC     "PC"    // Protocol command
#define ER     "ER"    // Expected results
#define RR     "RR"    // Real results

/* テスト・プロトコルコマンド格納用構造体 */
typedef struct _testCommand{
  int  commandCount;                //テスト・プロトコルコマンドの件数
  int  commandClientNo[TEST_MAX];   //テスト・プロトコルコマンドを実行するクライアント識別
  char *commandData[TEST_MAX];      //テスト・プロトコルコマンド
}TestCommand;

/* レスポンス期待結果格納用構造体 */
typedef struct _testResponse{
  int  responseCount;
  char *responseData[RECORD_MAX];
}TestResponse;

extern int setup_connect( char *__target, u_short __port);
extern int recv_data(int s, char recv_buf[], int buf_size);
extern int record_division(char *block, char *records[]);

int read_testcase(TestCommand *testCommand, TestResponse *expectResponse, char *testcase);

int main(int argc, char *argv[]){
  TestCommand  testCommand;
  TestResponse expectResponse[TEST_MAX];
  int soc[CLIENT_MAX], s;
  char sendBuf[BUFSIZE];
  char recvBuf[BUFSIZE_MAX];
  int  sendLen, recvLen;
  u_short port;
  char *records[RECORD_MAX];  //データレコードのポインタ配列
  int recordCount;  //データレコードの件数
  int i, j, k,flag;
  int clientCount;


  /* 画面クリア */
  clear;
  location(0,0);

  if(argc != 4){
    printf("Usage: %s ipaddr port testcase-file\n", argv[0]);
    exit(1);
  }

  port = atoi(argv[2]);

  printf("\n********************* << TEST_PROTOCOL START >> ***********************\n\n");
  
  /* テストケースの読込 */
  clientCount = read_testcase(&testCommand, expectResponse, argv[3]);
  if(clientCount < 0){
    printf("ERROR: Reading testcase data [%s] failed.\n", argv[3]);
    exit(1);
  }

  printf("SUCCESS: Reading testcase data [%s] completed.\n", argv[3]);  

  printf("Client count: %d\n", clientCount);

  for(i=0; i<clientCount; i++){
    soc[i] = setup_connect(argv[1], port);
    if(soc[i] < 0) exit(1);
  }
 
  printf("\n");
  fg(FG_CYAN);
  printf("++ %s: Protocol command\n", PC);
  fg(FG_RED);
  printf("++ %s: Real results\n", RR);
  fg(FG_GREEN);
  printf("++ %s: Expected results\n", ER);
  esc_default;
  printf("\n");

  for(i=0; i<testCommand.commandCount; i++){
    printf("\n======================================================================\n");
    fg(FG_CYAN);
    printf("<%s %2d %d>%s", PC, i+1, testCommand.commandClientNo[i], testCommand.commandData[i]);
    esc_default;
    /*if(i == 0){
    recvLen = recv_data(s, recvBuf, BUFSIZE_MAX); //データ終了デリミタまでサーバからデータ受信
    recordCount = record_division(recvBuf, records); //レコードに分割
    for(j=0; j<recordCount; j++){
      fg(FG_RED);
      printf("[%s %2d]%s\n", RR, j, records[j]); //実際の結果
      esc_default;
    }
    printf("----------------------------------------------------------------------\n");
    }*/
    sendLen = strlen(testCommand.commandData[i]);
    s = soc[testCommand.commandClientNo[i]];
    send(s, testCommand.commandData[i], sendLen, 0); //テスト・プロトコル・コマンド送信

    recvLen = recv_data(s, recvBuf, BUFSIZE_MAX); //データ終了デリミタまでサーバからデータ受信
    recordCount = record_division(recvBuf, records); //レコードに分割

    for(j=0; j<recordCount; j++){
      fg(FG_RED);
      printf("[%s %2d]%s\n", RR, j, records[j]); //実際の結果
      esc_default;
    }
    printf("----------------------------------------------------------------------\n");
    
    for(k=0; k<expectResponse[i].responseCount; k++){
      fg(FG_GREEN);
      printf("[%s %2d]%s", ER, k, expectResponse[i].responseData[k]);  //期待する結果
      esc_default;
    }

    sleep(3);
  }


  printf("\n********************** << TEST_PROTOCOL END >> ***********************\n\n");

  for(i=0; i<clientCount; i++){
    close(soc[i]);
  }
  return 0;

}

//=================================
//  read_testcase
//
//=================================
int read_testcase(TestCommand *testCommand, TestResponse *expectResponse, char *testcase){
  FILE *fp;
  char tmpBuf[BUFSIZE];
  char *command, *response;
  char key;
  int  nc, nr, nres;
  int  clientNo, clientMax=-1;
  
  if(( fp = fopen(testcase, "r")) == NULL){
    fprintf(stderr,"ERROR: Testcase data [%s] was not found.\n", testcase);
    return -1;
  }

  nc = nr = 0;
  nres = 0;
  while(1){
    memset(tmpBuf, 0, BUFSIZE);
    if(fgets(tmpBuf, BUFSIZE, fp) == NULL) break;
    //printf("%s\n", tmpBuf);

    key = tmpBuf[0];
    switch(key){
    case '%':   //コメント
      break;

    case '>':  //プロトコル・コマンド
      if(!('0' <= tmpBuf[1]  && tmpBuf[1] <= '9') || tmpBuf[2] != ':' ){
	fprintf(stderr,"ERROR: Testcase data format is incorrect.\n");
	fclose(fp);
	return -1;
      }
      clientNo = tmpBuf[1] - '0';
      if(clientNo > clientMax)  clientMax = clientNo;
      
      if(nc != 0){
	expectResponse[nr].responseCount = nres;
	nr++;
      }
      command = (char *)calloc(BUFSIZE, sizeof(char));
      if(command == NULL){
	fprintf(stderr,"ERROR: Memory allocation failed.\n");
	fclose(fp);
	return -1;
      }
      sprintf(command,"%s", &tmpBuf[3]);
      testCommand->commandData[nc] = command;       //プロトコルコマンド
      testCommand->commandClientNo[nc] = clientNo;  //クライアント識別
      nc++;
      nres = 0;
      break;

    case '<': //レスポンス・データ
      response = (char *)calloc(BUFSIZE, sizeof(char));
      if(response == NULL){
	fprintf(stderr,"ERROR: Memory allocation failed.\n");
	return -1;
      }
      sprintf(response, "%s", &tmpBuf[1]);
      expectResponse[nr].responseData[nres++] = response;
      break;
    default:
      if(tmpBuf[0] != '\n'){
	fclose(fp);
	fprintf(stderr,"ERROR: Testcase data format is incorrect.\n");
	return -1;
      }
    }
  }

  if(nc != nr){
    expectResponse[nr].responseCount = nres;
    nr++;
  }
  
  testCommand->commandCount = nc;

  fclose(fp);
  
  return clientMax+1;
}
