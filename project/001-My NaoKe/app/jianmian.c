#include "common.h"
#include "LQ12864.h"
#include "jianmian.h"
#include "yingjian.h"
#include "huatu.h"
extern volatile uint16 mosixuanzi,gamestart,gameing;
extern uint16 key_up,key_down,key_left,key_right;
volatile uint8 djm=1,jmms=0;
void jianmian(void){
  uint8 jmi,jma[5]={1,0,0,0,0};
  jmms=1;
  while(djm){
    key();
    for(jmi=0;jmi<5;jmi++)
      LCD_tuxing(jmi*26,0,jma[jmi]);
    for(jmi=0;jmi<6;jmi++){
      LCD_tuxing1(0,jmi+2,0);
      LCD_tuxing1(127,jmi+2,0);
  }
    for(jmi=2;jmi<126;jmi++)
      LCD_tuxing1(jmi,7,1);
  switch (mosixuanzi){
    case 1:for (uint16 i=0;i<3;++i){
           LCD_P14x16Ch(72+14*i,3,i+6);}
           LCD_tuxing2(15,3,0);
           break;//推箱子
    case 2:for (uint16 i=0;i<2;++i){
           LCD_P14x16Ch(72+14*i,3,i+28);}LCD_P14x16Ch(72+14*2,3,15);
           LCD_tuxing2(15,3,2);
           break;//扫雷
    case 3:for (uint16 i=0;i<3;++i){
           LCD_P14x16Ch(72+14*i,3,i+34);}
           LCD_tuxing2(15,3,1);
           break;//五子棋
    case 4:LCD_P8x16Str(72,3,"Flash ");
           LCD_tuxing2(15,3,3);
           break;//bad Apple
    case 5:for (uint16 i=0;i<2;++i){
           LCD_P14x16Ch(72+14*i,3,i+57);}LCD_P14x16Ch(72+14*2,3,15);
           LCD_tuxing2(15,3,4);
           break;//其他
  }

    if(key_left==0){
      delay(500);
      if(key_left==0){
        mosixuanzi--;
        playmusic(2);
        if(mosixuanzi==0)
          mosixuanzi=5;
         }while(!key_left){key();}
       }
    if(key_right==0){
      delay(500);
      if(key_right==0){
        mosixuanzi++;
        playmusic(2);
        if(mosixuanzi==6)
          mosixuanzi=1;
      }while(!key_right){key();}
    }
    for(jmi=0;jmi<5;jmi++)
      jma[jmi]=0;
    jma[mosixuanzi-1]=1;
  }
  djm=1;gamestart=1;
LCD_CLS();
}

void xtjianmian(void){
  uint8 xtshu=1;
  LCD_P14x16Ch(105,xtshu*2+1,16);
  uint8 a=33;
  uint8 b=45;
  uint8 d=1;
while(1){
while(d){
  if(gamestart==0)
    return;
  key();
  for (uint8 i=0;i<2;++i){LCD_P14x16Ch(40+30*i,0,i+64);}
  for (uint8 i=0;i<5;++i){LCD_P14x16Ch(25+16*i,3,i+59);}
  //LCD_P8x16Str(16,0,"Flash Player");
  //LCD_P8x16Str(20,3,"666666");
  LCD_P8x16Str(25,5,"  SD Card");
  LCD_P14x16Ch(105,xtshu*2+1,16);
    if(key_up==0){
      delay(500);
      if(key_up==0){
        xtshu--;
        playmusic(2);
        if(xtshu==0)
          xtshu=2;
    LCD_P14x16Ch(105,3,15);
    LCD_P14x16Ch(105,5,15);
    //LCD_P14x16Ch(105,6,15);
    LCD_P14x16Ch(105,xtshu*2+1,16);
      }while(!key_up){key();}
    }
    if(key_down==0){
      delay(500);
      if(key_down==0){
        xtshu++;
        playmusic(2);
        if(xtshu==3)
          xtshu=1;
    LCD_P14x16Ch(105,3,15);
    LCD_P14x16Ch(105,5,15);
    //LCD_P14x16Ch(105,6,15);
    LCD_P14x16Ch(105,xtshu*2+1,16);
      }while(!key_down){key();}
    }
    if(key_left==0||key_right==0){
      delay(500);
      if(key_left==0||key_right==0){
        d=0;
      }while(!key_left||!key_right){key();}
    }
  }
  LCD_CLS();
  gameing=1;
  if(xtshu==1){
    mosixuanzi=6;
    huatu();
    mosixuanzi=5;
  }
  else
  readsd();
  LCD_CLS();
  gamestart=1;d=1;
  delay(100);
  }
}