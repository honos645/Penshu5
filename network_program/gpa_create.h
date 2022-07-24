#ifndef VIEW_GRADE_H
#define VIEW_GRADE_H

#define BUFSIZE 1024
#define ENTER "\n" //<LF>

//*** レスポンスステータス ***//
#define OK_STAT    "+OK"     //成功
#define ER_STAT    "-ERR"    //失敗

//*** エラーコード ***//
#define E_CODE_1    100 //コマンドの引数エラー
#define E_CODE_2    200 //データベースエラー
#define E_CODE_3    300 //権限なし・ユーザレベルエラー

//*** ユーザレベル ***//
#define ADMIN       0
#define STUDENT     1
#define CLERK       2
#define TEACH_COM   3
#define TEACH       4
#define TEACH_HR    5
#define TEACH_VHR   6
#define TEACH_W_HR  7
#define TEACH_W     8
#define CHAIR       9

//*** 成績区分 ***//
#define ATTEND_LESS 999
#define NOT_TAKEN   333

//*** 期間 ***//
#define TERMS	    32
//*** 科目区分 ***//
#define NUM_SUB	    3
#define ALL	    0

#define BASEEDU	    1
#define TECHNIC	    2
#define BASEEGN	    3
#define TECHNES	    4
#define TECHSEL	    5


#endif
