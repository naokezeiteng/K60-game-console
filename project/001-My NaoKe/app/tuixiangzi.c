#include "common.h"
#include "LQ12864.h"
#include "tuixiangzi.h"
#include "yingjian.h"

extern uint16 huatu1[8][8];

uint16 cur_x,cur_y,X,dir[4][2]={-1,0,1,0,0,-1,0,1},wherex[4]={0,0,0,0},wherey[4]={0,0,0,0};
uint16 mat1[8][8]={  // 0表示空格，1表示墙，2表示人，3表示箱子，4表示目的地
1,1,1,1,1,1,1,1,
1,1,1,4,1,1,1,1,
1,1,1,0,1,1,1,1,
1,1,1,3,0,3,4,1,
1,4,0,3,2,1,1,1,
1,1,1,1,3,1,1,1,
1,1,1,1,4,1,1,1,
1,1,1,1,1,1,1,1
};
uint16 mat2[8][8]={
1,1,1,1,1,1,1,1,
1,1,1,1,0,2,1,1,
1,1,0,0,3,0,1,1,
1,1,0,1,0,1,1,1,
1,0,0,1,0,1,4,1,
1,0,1,0,0,3,4,1,
1,0,3,0,0,0,4,1,
1,1,1,1,1,1,1,1
};
uint16 mat3[8][8]={
1,1,1,1,1,1,1,1,
1,1,1,0,0,1,1,1,
1,1,2,3,0,1,1,1,
1,1,1,3,0,1,1,1,
1,1,1,0,3,0,1,1,
1,1,4,3,0,0,1,1,
1,1,4,4,0,4,1,1,
1,1,1,1,1,1,1,1
};
uint16 map[8][8];
uint16 mat[8][8];
uint16 mapshu=1;

void tuixiangzi(void){
  uint16 i,num,index=6;
  uint16 bushu=0;
  uint16 cj=500;
  unsigned char s[]="0";
  head:
  show_start();
  if(g_app.exit_req){
    g_app.exit_req=0;
    g_app.need_ack=0;
    mapshu=1;
    return;
  }
  start();
  show_map();
  bushu=0;
  index=6;
  for (i=0;i<3;++i){
      LCD_P14x16Ch(66+i*14,0,i);
    }
      LCD_P8x16Str(66+14*2,0,":");
  for (i=0;i<2;++i){
      LCD_P14x16Ch(66+i*14,2,i+17);
    }
      LCD_P8x16Str(66+14*2,2,":");
      g_app.playing=1;
      g_app.game_over=0;
      input_clear();
  while(1){
    input_poll();
    if(input_edge(IN_BACK)){
      g_app.playing=0;
      g_app.timer_on=0;
      LCD_CLS();
      goto head;
    }
    s[0]=(int)bushu/10+48;
    LCD_P8x16Str(66+14*2+8,0,s);
    s[0]=bushu%10+48;
    LCD_P8x16Str(66+14*2+16,0,s);
    g_app.timer_on=1;
    s[0]=(int)g_app.time_left/10+48;
    LCD_P8x16Str(66+14*2+8,2,s);
    s[0]=g_app.time_left%10+48;
    LCD_P8x16Str(66+14*2+16,2,s);
    if(g_app.time_left==0)
      goto LOSS;
    num=0;
    for(i=0;i<X;i++){
      if(map[wherex[i]][wherey[i]]==3)num++;
      else break;
    }
    if(num==X)goto WIN;
    if(input_edge(IN_UP)){
        index=judge(cur_x,cur_y,2);
        fun(index,2);
        bushu++;
    }
    else if(input_edge(IN_DOWN)){
        index=judge(cur_x,cur_y,3);
        fun(index,3);
        bushu++;
    }
    else if(input_edge(IN_LEFT)){
        index=judge(cur_x,cur_y,0);
        fun(index,0);
        bushu++;
    }
    else if(input_edge(IN_RIGHT)){
        index=judge(cur_x,cur_y,1);
        fun(index,1);
        bushu++;
    }
  }
WIN:
  g_app.game_over=1;
  g_app.playing=0;
  g_app.timer_on=0;
  for (i=0;i<4;++i){
      LCD_P14x16Ch(66+i*14,2,i+2);
    }
    cj=(int)(((100-bushu)+g_app.time_left)/2);
    s[0]=(int)cj/100+48;
    LCD_P8x16Str(66+14+8,4,s);
    s[0]=(int)cj/10+48;
    LCD_P8x16Str(66+14+8,4,s);
    s[0]=cj%10+48;
    LCD_P8x16Str(66+14+16,4,s);
  LCD_P8x16Str(66,6,"YON WIN");
  g_app.need_ack=1;
  input_clear();
  while(g_app.need_ack){
    input_poll();
    if(input_edge(IN_BACK)||input_edge(IN_OK))
      g_app.need_ack=0;
  }
  LCD_CLS();
  goto head;

LOSS:
  g_app.game_over=1;
  g_app.playing=0;
  g_app.timer_on=0;
  LCD_CLS();
  LCD_P128x64Ch1();
  for (i=0;i<4;++i){
      LCD_P14x16Ch(100,i*2,i+19);
    }
  g_app.need_ack=1;
  input_clear();
  while(g_app.need_ack){
    input_poll();
    if(input_edge(IN_BACK)||input_edge(IN_OK))
      g_app.need_ack=0;
  }
  LCD_CLS();
  goto head;
}

void show_start(void){
  uint16 b=45;
  uint16 d=1;
  uint16 i;
  LCD_P14x16Ch(80,mapshu*2-2,16);
  g_app.exit_req=0;
  input_clear();
while(d){
  input_poll();
  if(input_edge(IN_BACK)){
    g_app.exit_req=1;
    return;
  }
    for (i=0;i<2;++i){
      LCD_P14x16Ch(b+14*i,0,i+9);}
      for (i=0;i<2;++i){
      LCD_P14x16Ch(b+14*i,2,i+11);}
      for (i=0;i<2;++i){
      LCD_P14x16Ch(b+14*i,4,i+13);}
      for (i=0;i<2;++i){
      LCD_P14x16Ch(b+14*i,6,i+55);}
    if(input_edge(IN_UP)){
        mapshu--;
        playmusic(2);
        if(mapshu==0)
          mapshu=4;
    LCD_P14x16Ch(80,0,15);
    LCD_P14x16Ch(80,2,15);
    LCD_P14x16Ch(80,4,15);
    LCD_P14x16Ch(80,6,15);
    LCD_P14x16Ch(80,mapshu*2-2,16);
    }
    if(input_edge(IN_DOWN)){
        mapshu++;
        playmusic(2);
        if(mapshu==5)
          mapshu=1;
    LCD_P14x16Ch(80,0,15);
    LCD_P14x16Ch(80,2,15);
    LCD_P14x16Ch(80,4,15);
    LCD_P14x16Ch(80,6,15);
    LCD_P14x16Ch(80,mapshu*2-2,16);
    }
    if(input_edge(IN_LEFT)||input_edge(IN_RIGHT)||input_edge(IN_OK)){
        d=0;
    }
  }
LCD_CLS();
}
void start(void){
  uint16 i,j;

  switch(mapshu){
    case 1:{
      g_app.time_left=30;
      cur_x=4; cur_y=4;
      uint16 wherex1[4]={1,4,6,3};
      uint16 wherey1[4]={4,6,3,1};
      for(i=0;i<4;i++)
        wherex[i]=wherex1[i];
      for(i=0;i<4;i++)
        wherey[i]=wherey1[i];
      X=4;
       for(i=0;i<8;i++){
        for(j=0;j<8;j++)mat[i][j]=mat1[i][j];
      }
      for(i=0;i<8;i++){
        for(j=0;j<8;j++)map[i][j]=mat[j][i];
      }
      break;}
    case 3:{
      g_app.time_left=35;
      cur_x=5; cur_y=1;
      uint16 wherex1[4]={6,6,6,0};
      uint16 wherey1[4]={4,5,6,0};
      for(i=0;i<3;i++)
        wherex[i]=wherex1[i];
      for(i=0;i<3;i++)
        wherey[i]=wherey1[i];
      X=3;
       for(i=0;i<8;i++){
        for(j=0;j<8;j++)mat[i][j]=mat2[i][j];
      }
      for(i=0;i<8;i++){
        for(j=0;j<8;j++)map[i][j]=mat[j][i];
      }
      break;}
    case 2:{
      g_app.time_left=40;
      cur_x=2; cur_y=2;
      uint16 wherex1[4]={2,2,3,5};
      uint16 wherey1[4]={5,6,6,6};
      for(i=0;i<4;i++)
        wherex[i]=wherex1[i];
      for(i=0;i<4;i++)
        wherey[i]=wherey1[i];
      X=4;
       for(i=0;i<8;i++){
        for(j=0;j<8;j++)mat[i][j]=mat3[i][j];
      }
      for(i=0;i<8;i++){
        for(j=0;j<8;j++)map[i][j]=mat[j][i];
      }
      break;}

      case 4:{
      g_app.time_left=50;X=0;
      uint16 wherex1[4]={0};
      uint16 wherey1[4]={0};
      cur_x=2; cur_y=2;
      for(uint16 aaa=0;aaa<8;aaa++){
        for(uint16 bbb=0;bbb<8;bbb++){
          if(huatu1[aaa][bbb]==4){
            wherex1[X]=aaa;
            wherey1[X]=bbb;
            X++;
          }
        }
      }
      for(i=0;i<4;i++)
        wherex[i]=wherex1[i];
      for(i=0;i<4;i++)
        wherey[i]=wherey1[i];
      //X=4;
       for(i=0;i<8;i++){
        for(j=0;j<8;j++)mat[i][j]=huatu1[j][i];
      }
      for(i=0;i<8;i++){
        for(j=0;j<8;j++)map[i][j]=mat[j][i];
      }
      break;}
  }
}

void show_map(void){
  uint16 i,j;

  for(i=0;i<8;i++){
    for(j=0;j<8;j++){
      LCD_P16x8Ch(i*8,j,map[i][j]);
    }
  }
}

/* 增量更新单个格子 */
void update(uint16 x, uint16 y){
  LCD_P16x8Ch(x*8, y, map[x][y]);
}

uint16 judge(uint16 x,uint16 y,uint16 id){ //id 表示方向数组的行标，0,1,2,3 分别表示上下左右
  uint16 xx,yy,xxx,yyy;
  xx=x+dir[id][0]; yy=y+dir[id][1];
  if(map[xx][yy]==0 || map[xx][yy]==4)return 1; //1表示前面是 空格 或者 目的地，就是可以直接移动
  else if(map[xx][yy]==1)return 0;               //0表示无法移动
  else if(map[xx][yy]==3){
    xxx=xx+dir[id][0]; yyy=yy+dir[id][1];
    if(map[xxx][yyy]==1 || map[xxx][yyy]==3)return 0;
    else if(map[xxx][yyy]==0 || map[xxx][yyy]==4)return 2; //2表示需要间接移动，先把前面的箱子移动一，再让人移动一
  }return 0;
}

void fun(uint16 key,uint16 dirction){
  uint16 x,y;
  if(key==1){//直接移动
    if(mat[cur_y][cur_x]==4)map[cur_x][cur_y]=4;
    else map[cur_x][cur_y]=0;
    update(cur_x,cur_y);

    cur_x+=dir[dirction][0]; cur_y+=dir[dirction][1];
    map[cur_x][cur_y]=2;
    update(cur_x,cur_y);

  }
  else if(key==2){//间接移动
    if(mat[cur_y][cur_x]==4)map[cur_x][cur_y]=4;
    else map[cur_x][cur_y]=0;
    update(cur_x,cur_y);

    cur_x+=dir[dirction][0]; cur_y+=dir[dirction][1];
    map[cur_x][cur_y]=2;
    update(cur_x,cur_y);

    x=cur_x+dir[dirction][0]; y=cur_y+dir[dirction][1];
    map[x][y]=3;
    update(x,y);
  }
}