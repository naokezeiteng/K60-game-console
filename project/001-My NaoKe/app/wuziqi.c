#include "common.h"
#include "LQ12864.h"
#include "wuziqi.h"
#include "yingjian.h"

extern volatile uint16 gameing,gamestart;
extern uint8 board[8][8];
extern int8 codn_x,codn_y;
uint8 player = 1;
uint8 rou = 0;

void wuziqi(void){
  for(int n=0;n<8;n++)
  {
    for(int m=0;m<5;m++){
      LCD_P14x16Ch(30+m*14,n,m+33);
    }
    delay(7000);
    LCD_Fill(0);
  }  
  gameing=1;
  /*pwm_init(50);                            背景音乐
  LPLD_FTM_PWM_ChangeDuty(FTM0,FTM_Ch0, 0);*/
  LCD_P8x16Str(70,3,"PLAYER1");
  LCD_P8x16Str(74,1,"GoBang");
  while(gamestart){
  	key_scan();}
  	player=1;
  	rou=0;
  	codn_x=codn_y=0;
    memset(board,0,sizeof(board));
  	gameing=gamestart=0;
}

void show_win(void)
{
  gameing=0;
  LCD_CLS();
  chess_board();
  if(player == 2)
    LCD_P6x8Str(84,7,"PLAYER1");
  else if(player == 1)
    LCD_P6x8Str(84,7,"PLAYER2");
}
void key_scan(void)
{

    if(LPLD_ADC_Get(ADC0, DAD1)>3000)
    {
      delay(200);  /* 消抖延迟，从8000减至200 */
      if(LPLD_ADC_Get(ADC0, DAD1)>3000)codn_x+=8;
      if(codn_x > 56) codn_x = 0;
    }
    else if(LPLD_ADC_Get(ADC0, DAD1) < 1000)
    {
      delay(200);  /* 消抖延迟，从8000减至200 */
      if(LPLD_ADC_Get(ADC0, DAD1)< 1000)codn_x-=8;
      if(codn_x < 0) codn_x = 56;
    }
    if(LPLD_ADC_Get(ADC0, DAD3)>3000)
    {
      delay(200);  /* 消抖延迟，从8000减至200 */
      if(LPLD_ADC_Get(ADC0, DAD3)>3000)codn_y++;
      if(codn_y > 7) codn_y = 0;
    }
    else if(LPLD_ADC_Get(ADC0, DAD3) < 1000)
    {
      delay(200);  /* 消抖延迟，从8000减至200 */
      if(LPLD_ADC_Get(ADC0, DAD3)< 1000)codn_y--;
      if(codn_y < 0) codn_y = 7;
    }
}

uint8 board_scan(int num)
{
  uint8 count = 0;

  for(int i = 0; i < 8;i++)   //行扫描
  {
    count = 0;  /* 每行重置 */
    for(int j = 0; j < 7;j++)
    {
      uint8 tmp = board[i][j];
      if(tmp != board[i][j+1] || tmp ==0){count = 0;}
      else count++;
      if(count > num)return 1;
    }
  }
  for(int i = 0; i < 8;i++)  //列扫描
  {
    count = 0;  /* 每列重置 */
    for(int j = 0; j < 7;j++)
    {
      uint8 tmp = board[j][i];
      if(tmp != board[j+1][i] || tmp ==0)count = 0;
      else count++;
      if(count > num)return 1;
    }
  }

  for(int i = 0; i < 4;i++)
  {
    count = 0;  /* 每条对角线重置 */
    int a = i;
    for(int j = 0; j < 8 - i;j++)
    {
      uint8 tmp = board[a][j];
      if(tmp != board[a+1][j+1] || tmp ==0)count = 0;
      else {count++;}
      if(count > 3)return 1;
      a++;
    }
  }

  for(int i = 0; i<4;i++)//斜方向下遍历次数3
  {
    count = 0;
    int a = 0;
    for(int j = 4-i;j < 8;j++)
    {
      uint8 tmp = board[a][j];
      if(tmp != board[a+1][j+1] || tmp ==0)count = 0;
      else {count++;}
      if(count > 3)return 1;
      a++;
    }
  }

  for(int i = 0; i<4;i++)//斜方向下遍历次数4
  {
    count = 0;
    int a = i;
    for(int j = 7;j > i;j--)
    {
      uint8 tmp = board[a][j];
      if(tmp != board[a+1][j-1] || tmp ==0)count = 0;
      else {count++;}
      if(count > 3)return 1;
      a++;
    }
  }

  for(int i = 0; i<3;i++)//斜方向下遍历次数4
  {
    count = 0;
    int a = 0;
    for(int j = 4+i;j > 0;j--)
    {
      uint8 tmp = board[a][j];
      if(tmp != board[a+1][j-1] || tmp ==0)count = 0;
      else {count++;}
      if(count > 3)return 1;
      a++;
    }
  }
  return 0;
}
