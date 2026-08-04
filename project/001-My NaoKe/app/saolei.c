#include "common.h"
#include "LQ12864.h"
#include "saolei.h"
#include "yingjian.h"
#define rows 8
#define cols 8
#define Count 10

extern uint16 result;
extern uint16 result1;
extern uint16 key_down,key_up,key_left,key_right;
extern uint16 gamestart,gameing;
uint16 gameoversl=0;
uint16 t1=0;
uint16 t2=0;
uint16 counter=0;
uint16 a;
uint16 b;
uint16 t3=0;
uint16 t4=0;
uint16 t5=0;
uint16 t6=0;
uint16 ct=0;
unsigned char mine[rows][cols];
unsigned  char show[rows][cols];
uint16 x=0;
uint16 y=0;
uint16 biaozhi=0; 
uint16 count = Count ;
uint16 t=0;
uint16 ti=0;
uint16 kaiguan[4]={0};
uint16 cit=0;
uint16 k=0;
uint16 timesl=0;
unsigned char s[]="0";
unsigned char f[]="0";
unsigned char g[]="0";

void saolei(void){
  gamestart=1;gameoversl=1;
	uint16 i = 0;
	uint16 j = 0;
	for (i = 0; i < rows ; i++)
	{
		for (j = 0; j < cols ; j++)
		{
			mine[i][j] = '0';
			show[i][j] = '*';
		}
	}
  menu();
  while(1)
  {  //duqu();
    gamestart=1;
    kaiguan[0]=1;
		if (kaiguan[0]==1)
		{      if(t1==0&&t2==0){
	          LCD_CLS();
                    pit_init();
                       delay(10);
                       t2=1;
                }     if(cit==0)
                {  LCD_P8x8Ch(8*x,y,0);
                }
			Game(mine,show);
		}
    if(gameoversl==1){
      if(gamestart==0){
        gameoversl=0;gameing=0;
        t1=t2=counter=t3=t4=t5=t6=ct=x=y=biaozhi=t=ti=cit=k=timesl=0;
        count = Count ;
        return;
      }
  } 
  }   
}

//菜单函数
uint16 menu()
{
  //LCD_P128x64Ch();
	return 0;
}

 
//设置雷的位置
void set_mine(unsigned char mine[rows][cols])
{   
	uint16 n = 0;
	uint16 m = 0;
	while (count)
	{     delay1(20);
          a= LPLD_ADC_Get(ADC1, DAD1)%10;
             delay1(20);
                b= LPLD_ADC_Get(ADC1, DAD1)%10;
		n = (a*8)/10;
		m = (b*8)/10;
		if (mine[n][m] == '0')     
		{
			mine[n][m] = '1';
			count--;
		}
	}
}
 

void display(unsigned char show[rows][cols])  
{  
	uint16 i = 0;
	uint16 j = 0;
 
	for (i = 0; i < rows ; i++)
	{
		for (j = 0; j <cols ; j++)
		{   	s[0]=show[i][j];
			   LCD_P6x8Str(8*i,j,s);  
		}
	
	}
        
}
 
//计算雷的个数
uint16 get_num(unsigned char mine[rows][cols], uint16 x, uint16 y)
{
	int16 count = 0;
	int16 i, j;
	// 先检查边界再访问数组，避免越界读取
	for (i = -1; i <= 1; i++) {
		for (j = -1; j <= 1; j++) {
			if (i == 0 && j == 0) continue;
			if ((int16)x + i >= 0 && (int16)x + i <= 7 &&
			    (int16)y + j >= 0 && (int16)y + j <= 7) {
				if (mine[x + i][y + j] == '1')
					count++;
			}
		}
	}
	return count;
}
//扫雷
uint16 Sweep(unsigned char mine[rows][cols],unsigned  char show[rows][cols])
{        
        
         if(key_up==1&&ti==0)
         {
           y--;
           ti++;
         } 
          if(key_down==1&&ti==0)
          {
           y++;
           ti++;
          }
          if(key_left==1&&ti==0)
         {
           x--;
           ti++;
         }
          if(key_right==1&&ti==0)
         {
           x++;
           ti++;
         }
         if(x<=0)
         { x=0;
         }
          if(x>=7)
         { x=7;}
          if(y<=0)
         { y=0;}
          if(y>=7)
         { y=7;}
         if(cit==0){
           display(show); 
        k=0;
        uint16 i = 0;
	uint16 j = 0;
	for (i = 0; i < rows ; i++)
	{
		for (j = 0; j <cols ; j++)
		{   	
                  if(show[i][j]=='*')
                  {  k++;

                  }
		}
	}
      sprintf(g, "%d", k);
       LCD_P8x16Str(70,3,"*");
LCD_P14x16Ch(78,3,26);
LCD_P14x16Ch(92,3,27);
    if(t6==0&&g[0]<10){
            LCD_CLS();
                       delay1(10);
                       t6=1;
                } 
LCD_P8x16Str(106,3,g);
        if(k<=15&&k>Count){
        LCD_P14x16Ch(70,0,23);
        LCD_P14x16Ch(84,0,24);
        LCD_P14x16Ch(98,0,25);
        }
        if(k==Count){
          cit=2;  
		                 if(t5==0){
            LCD_CLS();
                       delay1(10);
                       t5=1;
                } 
            LCD_P8x16Str(30,2,"you are a ");
             LCD_P8x16Str(40,4," winner ");
             gameoversl=1;
        }

            if(ct==1){
                     if(t4==0){
            LCD_CLS();
                       delay1(10);
                       t4=1;
                } 
            cit=2;
            LCD_P8x16Str(20,3,"try again ! ");
            gameoversl=1;
/*             for (int i=0;i<500;i++)//蜂鸣器
	{
		LPLD_GPIO_Toggle_b(PTC,9); 
             delay1(100);
              LPLD_GPIO_Toggle_b(PTC,9); 
             delay1(100);
	}*/        
           }
         } 
	return 0;
}
 
 
//游戏
uint16 Game(unsigned char mine[rows][cols],unsigned char show[rows][cols])
{    
  time1();
  kongzhi();
	set_mine(mine);
        if(cit==0){
	display(show);
        }
	//display(mine);//可以将雷的位置显示出来
	Sweep(mine,show);
	return 0;
}

void kongzhi(void)
  {  
result1 = LPLD_ADC_Get(ADC0, DAD1);//获得数字量
result = LPLD_ADC_Get(ADC0, DAD3);
if(result<1800&&t==0){

   key_up=1;  
    key_left=0;
  key_right=0; 
  key_down=0;
 t++;
           //up
}
else if(result>2200&&t==0){
     key_up=0;  
    key_left=0;
  key_right=0; 
  key_down=1;
    t++;         //down
}

if(result1<1800&&t==0){
      key_up=0;  
    key_left=1;
  key_right=0; 
  key_down=0;
     t++;     //left
}
else if(result1>2200&&t==0){
      key_up=0;  
    key_left=0;
  key_right=1; 
  key_down=0;            //right
  t++;
}
if(result>=1800&&result<=2200&&result1>=1800&&result1<=2200)
{
 t=0;
 ti=0;
   key_up=0;  
   key_left=0;
  key_right=0; 
  key_down=0; 
}
  delay1(100);
}

void delay1(uint16 ms)
{  
   uint16 ii,jj;
   if (ms<1) ms=1;
   for(ii=0;ii<ms;ii++)
     for(jj=0;jj<500;jj++);  
       
} 

/*void duqu(void){   
  
  if(LPLD_GPIO_Input_b(PTC, 0)==0){
    delay1(10);
    if(LPLD_GPIO_Input_b(PTC, 0)==0)
    {
      kaiguan[0]=1;
    }
  }
}*/

void time1(void)      
{        
     timesl=counter;
     if(cit==0){
       sprintf(f, "%d", timesl);
   LCD_P8x16Str(100,6,f);
  LCD_P14x16Ch(70,6,17);
LCD_P14x16Ch(84,6,18);
     }
       if(timesl>=200&&cit!=2)
       {  
         cit=1;
            if(t3==0){
	          LCD_CLS();
                       delay1(10);
                       t3=1;
                } 
        
          LCD_P8x16Str(10,2,"you are out of ");
             LCD_P8x16Str(40,4," time ");
             gameoversl=1;    
       }    
}

void surrond(uint16 a, uint16 b)
{   if((a+1)<=7&&(b+1)<=7){
  if(mine[a+1][b+1] != '1')
  {   
       show[a+1][b+1] = get_num(mine, a+1, b+1)+ '0';
                    //    set_mine(mine);
                        delay1(50);
  }
}  if((b+1)<=7){
   if(mine[a][b+1] != '1')
  {
       show[a][b+1] = get_num(mine, a, b+1)+ '0';
                       //  set_mine(mine);
                        delay1(50);
  }
}
if((a+1)<=7){
 if(mine[a+1][b] != '1')
  {
       show[a+1][b] = get_num(mine, a+1, b)+ '0';
                    //    set_mine(mine);
                        delay1(50);
  }}
  if((a-1)>=0&&(b+1)<=7){
 if(mine[a-1][b+1] != '1')
  {
       show[a-1][b+1] = get_num(mine, a-1, b+1)+ '0';
                   //      set_mine(mine);
                        delay1(50);
  }
  }
  if((b-1)>=0&&(a-1)>=0){
 if(mine[a-1][b-1] != '1')
  {
       show[a-1][b-1] = get_num(mine, a-1, b-1)+ '0';
                    //    set_mine(mine);
                        delay1(50);
  }  }
        if((a+1)<=7&&(b-1)>=0){
 if(mine[a+1][b-1] != '1')
  {
       show[a+1][b-1] = get_num(mine, a+1, b-1)+ '0';
                      //  set_mine(mine);
                        delay1(50);
  }} 
        if((b-1)>=0){
  if(mine[a][b-1] != '1')
  {
       show[a][b-1] = get_num(mine, a, b-1)+ '0';
                 //       set_mine(mine);
                        delay1(50);
  } }  if((a-1)>=0){
  if(mine[a-1][b] != '1')
  {
       show[a-1][b] = get_num(mine, a-1, b)+ '0';
                      //   set_mine(mine);
                        delay1(50);
  }
  }
     return;

}
void panduan(uint16 a, uint16 b) {
    uint16 i, j;
    if (mine[a][b] != '1')
    {
    uint16 ret = get_num(mine, a, b);  
     if(ret!=0){    
                          show[a][b] = ret + '0';   //‘0'+1=‘1’
                    //   set_mine(mine);
                        delay1(10);
                           return;
     }
     if(biaozhi<=80){
    if (ret==0) {    
    show[a][b] = ret + '0';
        surrond(a, b);
           biaozhi++;
        if(a>=0&&a<=7&&b>=0&&b<=7)
        for (i = -1; i <= 1; i++) {
            for (j = -1; j <= 1; j++) {
           
                if (i == 0 && j == 0) continue;

               else if((a+i)>=0&&(a+i)<=7&&(b+j)>=0&&(b+j)<=7){
                     /* 检查目标格是否已展开，避免无限递归 */
                     if(show[a+i][b+j] != '*') continue;
                     uint16 ret1 = get_num(mine, a+i, b+j);
                     if(ret1==0){
            panduan(a + i, b + j);
                }
               }
            }
        }
    }
    }
    }
    return;
} 