#include <stdio.h>
#include <unistd.h>

#define FG_BLACK   "\x1b[30m"  //文字色を黒
#define FG_RED     "\x1b[31m"  //文字色を赤
#define FG_GREEN   "\x1b[32m"  //文字色を緑
#define FG_YELLOW  "\x1b[33m"  //文字色を黄色
#define FG_BLUE    "\x1b[34m"  //文字色を青
#define FG_MAGENTA "\x1b[35m"  //文字色をマゼンタ
#define FG_CYAN    "\x1b[36m"  //文字色をシアン
#define FG_WHITE   "\x1b[37m"  //文字色を白

#define ESC_DEFAULT "\x1b[0m"  //デフォルトに戻す

#define  clear   printf("\x1b[2J");
#define  clear_r printf("\x1b[0K");
#define  location(x,y)  printf("\x1b[%d;%dH", x, y)
#define  cursor_left(x) printf("\x1b[%dD", x)
int main(void){

  int i;
  clear;
  location(0,0);

  printf("%s",FG_CYAN);
  printf("[COUNT]:");

  for(i=0; i<10; i++){
    clear_r;
    printf("%2d", i);
    fflush(stdout);
    sleep(1);
    cursor_left(2);
  }
  printf("%s", ESC_DEFAULT);
  printf("\n");
  
  return 0;
}
