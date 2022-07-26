/*
 * receive.c
 */

#include "test_protocol.h"

//=========================================================
// recv_data
// データ終了デリミタ(".\n")まで受信を繰り返す
//   [PARAMETER]
//     int    s         : ソケットディスクリプタ
//     char  *recvBuf   : 受信データを格納するバッファのポインタ
//     int    bufSize   : 受信データを格納するバッファサイズ
//   [RETURN]
//     success : 受信データのトータルサイズ
//     error   : -1 
//=========================================================
int recv_data(int s, char *recvBuf, int bufSize){
  char tmpBuf[BUFSIZE];
  int recvLen, recvTotal=0;
  
  memset(recvBuf, 0, bufSize);  //受信データ格納用バッファを初期化
  do{
    memset(tmpBuf, 0, BUFSIZE);                   //受信用のバッファを初期化
    recvLen = recv(s, tmpBuf, BUFSIZE-1, 0);      //サーバからデータ受信
    if(recvTotal == 0) 
      sprintf(recvBuf, "%s", tmpBuf);
    else if(recvTotal + recvLen < bufSize)
      sprintf(recvBuf, "%s%s", recvBuf, tmpBuf);  //受信データを結合
    else
      return -1;
    recvTotal += recvLen;                        //トータルの受信サイズを更新
  }while(strcmp(&recvBuf[recvTotal-2], DATA_END) != 0); //データ終了デリミタがあるまで繰り返す
  return recvTotal; //トータル受信データ数を返す
} 

//=========================================================
// record_division
// レコードの塊文字列から改行('\n')をレコードの終端としてレコードを分割
//   [PARAMETER]
//     char  *block     : レコードの塊文字列（分割対象文字列）
//     char  *records[] : レコードを表現するポインタ配列
//   [RETURN]
//     success : レコードの件数
//=========================================================
int record_division(char *block, char *records[]){
  int n=0;

  records[n++] = block;
 
  while(n < RECORD_MAX){
    while(*block && *block != '\n') block++; //改行('\n')になるまでポインタを進める
    *block = '\0';                           //改行を文字列終端文字('\0')に変更
    if(strncmp(block+1, DATA_END, 2)==0) break; //データ終了デリミタであれば終了
    records[n++] = ++block; //レコード表現ポインタ配列に次のレコードのポインタを設定
  }
  return n;  //レコード件数を返す
}
