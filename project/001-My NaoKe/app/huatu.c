#include "common.h"
#include "LQ12864.h"
#include "yingjian.h"
#include "huatu.h"
uint16 huatu1[8][8]={0};
int16 dir1[4][2]={-1,0,1,0,0,-1,0,1};
uint16 cu_x=4;
uint16 cu_y=4;
uint16 xuanzhetx,txshu=1;
extern uint16 key_up,key_down,key_left,key_right;
extern uint16 gameing,xuanzhetx,mosixuanzi,gamestart;
void huatu(void){
LCD_CLS();
huatu1[2][2]=2;
for (uint16 i=0;i<2;++i){    //右面板
      LCD_P14x16Ch1(72+i*14,0,i+46+1);
    }
for (uint16 i=0;i<2;++i){
      LCD_P14x16Ch(77+i*14,2,i+48+1);
    }
for (uint16 i=0;i<2;++i){
      LCD_P14x16Ch(82+i*14,4,i+50+1);
    }
for (uint16 i=0;i<2;++i){
      LCD_P14x16Ch(87+i*14,6,i+52+1);
    }
for(uint16 i=0;i<8;i++){
        for(uint16 j=0;j<8;j++){
        LCD_P16x8Ch(i*8,j,huatu1[i][j]);
        }
    }

  while(1){
      if(gamestart==0){
          LCD_CLS();
          return;
      }
    huatu1[2][2]=2;
    key();
    if(xuanzhetx)
        xuanzhetxjm();

    LCD_P16x8Ch(cu_x*8,cu_y,5);
    delay(1000);
    if(key_up==0){
      delay(500);
      if(key_up==0){
        move(cu_x,cu_y,2);
      }while(!key_up){key();}
    }
    else if(key_down==0){
      delay(500);
      if(key_down==0){
        move(cu_x,cu_y,3);
      }while(!key_down){key();}
    }
    else if(key_left==0){
      delay(500);
      if(key_left==0){
        move(cu_x,cu_y,0);
      }while(!key_left){key();}
    }
    else if(key_right==0){
      delay(500);
      if(key_right==0){
        move(cu_x,cu_y,1);
      }while(!key_right){key();}
    }
  }
}

void move(uint16 x,uint16 y,uint16 id){
    int16 xx1,yy1;
    uint16 key1;
    xx1=(int16)x+dir1[id][0]; yy1=(int16)y+dir1[id][1];
    if(xx1<0||xx1>=8||yy1<0||yy1>=8)
        key1=0;
    else
        key1=1;

    if(key1==1){//直接移动
    cu_x+=dir1[id][0]; cu_y+=dir1[id][1];
    //huatu1[cu_x][cu_y]=2;

    for(uint16 i=0;i<8;i++){
        for(uint16 j=0;j<8;j++){
        LCD_P16x8Ch(i*8,j,huatu1[i][j]);
        }
    }
  }
}

void xuanzhetxjm(void){
uint16 d=1;
while(d){
  /* if(gamestart==0)
    return; */
  key();
    if(key_up==0){
      delay(500);
      if(key_up==0){
        txshu--;
        playmusic(2);
        showtx();
      }while(!key_up){key();}
    }
    if(key_down==0){
      delay(500);
      if(key_down==0){
        txshu++;
        playmusic(2);
        if(txshu==5)
          txshu=1;
        showtx();
      }while(!key_down){key();}
    }
    if(key_left==0||key_right==0){
      delay(500);
      if(key_left==0||key_right==0){
        d=0;xuanzhetx=0;
        if(txshu==1)
        huatu1[cu_x][cu_y]=0;
        if(txshu==2)
        huatu1[cu_x][cu_y]=1;
        if(txshu==3)
        huatu1[cu_x][cu_y]=3;
        if(txshu==4)
        huatu1[cu_x][cu_y]=4;
        huatu1[2][2]=2;
      }while(!key_left||!key_right){key();}
    }
  }
}

void showtx(void){
    if(txshu==0)
          txshu=4;
        if(txshu==1){
            for (uint16 i=0;i<2;++i){    //右面板
                LCD_P14x16Ch1(72+i*14,0,i+46+1);
                }
            for (uint16 i=0;i<2;++i){
                LCD_P14x16Ch(77+i*14,2,i+48+1);
                }
            for (uint16 i=0;i<2;++i){
                LCD_P14x16Ch(82+i*14,4,i+50+1);
                }
            for (uint16 i=0;i<2;++i){
                LCD_P14x16Ch(87+i*14,6,i+52+1);
                }
        }
        if(txshu==2){
            for (uint16 i=0;i<2;++i){    //右面板
                LCD_P14x16Ch(72+i*14,0,i+46+1);
                }
            for (uint16 i=0;i<2;++i){
                LCD_P14x16Ch1(77+i*14,2,i+48+1);
                }
            for (uint16 i=0;i<2;++i){
                LCD_P14x16Ch(82+i*14,4,i+50+1);
                }
            for (uint16 i=0;i<2;++i){
                LCD_P14x16Ch(87+i*14,6,i+52+1);
                }
        }
        if(txshu==3){
            for (uint16 i=0;i<2;++i){    //右面板
                LCD_P14x16Ch(72+i*14,0,i+46+1);
                }
            for (uint16 i=0;i<2;++i){
                LCD_P14x16Ch(77+i*14,2,i+48+1);
                }
            for (uint16 i=0;i<2;++i){
                LCD_P14x16Ch1(82+i*14,4,i+50+1);
                }
            for (uint16 i=0;i<2;++i){
                LCD_P14x16Ch(87+i*14,6,i+52+1);
                }
        }
        if(txshu==4){
            for (uint16 i=0;i<2;++i){    //右面板
                LCD_P14x16Ch(72+i*14,0,i+46+1);
                }
            for (uint16 i=0;i<2;++i){
                LCD_P14x16Ch(77+i*14,2,i+48+1);
                }
            for (uint16 i=0;i<2;++i){
                LCD_P14x16Ch(82+i*14,4,i+50+1);
                }
            for (uint16 i=0;i<2;++i){
                LCD_P14x16Ch1(87+i*14,6,i+52+1);
                }
        }
}