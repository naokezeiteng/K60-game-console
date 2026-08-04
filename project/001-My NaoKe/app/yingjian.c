#include "common.h"
#include "wuziqi.h"
#include "LQ12864.h"
#include "yingjian.h"
#include "saolei.h"
#include "jianmian.h"
#define rows 8
#define cols 8
#define Count 10
GPIO_InitTypeDef gpio_init_struct;//变量声明
ADC_InitTypeDef adc_init_struct;
PIT_InitTypeDef pit0_init_struct;
PIT_InitTypeDef pit1_init_struct;
FTM_InitTypeDef ftm_init_struct;
DAC_InitTypeDef dac_init_struct;

extern uint16 result,result1;//储存ADC模块的采集数据
extern uint16 key_up,key_down,key_left,key_right;
extern uint16 flag;//返回
extern uint16 flag1,flag2;
extern uint16 time;
extern uint16 timeflag;
extern uint16 gameover;
extern uint16 mosixuanzi;
extern uint16 cit,x,y,ct,biaozhi;
extern uint16 counter;
extern unsigned char mine[rows][cols];
extern uint16 gamestart,gameing,gameovertxz;
extern uint8 board[8][8];
extern int8 codn_x,codn_y,rou,player;
extern uint8 wj_wzq;
extern uint8 djm,jmms,badapple_time;
extern uint16 xuanzhetx;
unsigned char round[]="round:00";
uint16 pit_n=0;

void key(void){
result = LPLD_ADC_Get(ADC0, DAD1);//获得数字量
result1 = LPLD_ADC_Get(ADC0, DAD3);
if(result<1000){
  key_left=0;
  key_right=1;
}
else if(result>3000){
  key_left=1;
  key_right=0;
}
else{
  key_left=1;
  key_right=1;
}
if(result1<1000){
  key_down=1;
  key_up=0;
}
else if(result1>3000){
  key_down=0;
  key_up=1;
}
else{
  key_up=1;
  key_down=1;
}
delay(20);  /* 消抖延迟，从100ms减至20ms提升响应速度 */
}

void playmusic(uint16 play){
  if(play==2){
    LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch0,30);
    delay(2000);
    LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch0,0);
  }
  else if(play==1){
    LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch0,10000);
    delay(2000);
    LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch0,0);
  }
}

void init_gpio(void){   //配置gpio口
  gpio_init_struct.GPIO_PTx = PTD;
  gpio_init_struct.GPIO_Pins = GPIO_Pin0|GPIO_Pin1|GPIO_Pin2|GPIO_Pin3;
  gpio_init_struct.GPIO_Dir = DIR_OUTPUT;
  gpio_init_struct.GPIO_Output = OUTPUT_H;
  gpio_init_struct.GPIO_PinControl = IRQC_DIS;
  LPLD_GPIO_Init(gpio_init_struct);

  gpio_init_struct.GPIO_PTx = PTD;
  gpio_init_struct.GPIO_Pins = GPIO_Pin4;
  gpio_init_struct.GPIO_Dir = DIR_OUTPUT;
  gpio_init_struct.GPIO_Output = OUTPUT_L;
  gpio_init_struct.GPIO_PinControl = IRQC_DIS;
  LPLD_GPIO_Init(gpio_init_struct);
  // 配置 PTD5为GPIO功能,输入,内部上拉，上升沿产生中断
  gpio_init_struct.GPIO_PTx = PTD;
  gpio_init_struct.GPIO_Pins = GPIO_Pin5|GPIO_Pin6;     //引脚5/6或运算置一//与运算置零（PDD=PDD&~（pin7|pin8））
  gpio_init_struct.GPIO_Dir = DIR_INPUT;        //输入
  gpio_init_struct.GPIO_PinControl = INPUT_PULL_UP|IRQC_FA;     //内部上拉|上升沿中断
  gpio_init_struct.GPIO_Isr = portb_isr;        //中断函数
  LPLD_GPIO_Init(gpio_init_struct);
  //使能中断
  LPLD_GPIO_EnableIrq(gpio_init_struct);

  /* gpio_init_struct.GPIO_PTx = PTC;      //PORTC
  gpio_init_struct.GPIO_Pins = GPIO_Pin0;
  gpio_init_struct.GPIO_Dir = DIR_INPUT;        //输入
  gpio_init_struct.GPIO_PinControl = INPUT_PULL_UP|IRQC_DIS;     //内部上拉|不产生中断
  LPLD_GPIO_Init(gpio_init_struct);

  gpio_init_struct.GPIO_PTx = PTC;
  gpio_init_struct.GPIO_Pins = GPIO_Pin1|GPIO_Pin2|GPIO_Pin3;
  gpio_init_struct.GPIO_Dir = DIR_OUTPUT;
  gpio_init_struct.GPIO_Output = OUTPUT_H;
  LPLD_GPIO_Init(gpio_init_struct); */
}

void adc_init(void){//配置ADC采样参数
  adc_init_struct.ADC_Adcx = ADC0;
  adc_init_struct.ADC_DiffMode = ADC_SE;        //单端采集
  adc_init_struct.ADC_BitMode = SE_12BIT;       //单端12位精度
  adc_init_struct.ADC_SampleTimeCfg = SAMTIME_SHORT;    //短采样时间
  adc_init_struct.ADC_HwAvgSel = HW_4AVG;       //4次硬件平均
  adc_init_struct.ADC_CalEnable = TRUE; //使能初始化校验
  LPLD_ADC_Init(adc_init_struct);//初始化ADC
  LPLD_ADC_Chn_Enable(ADC0, DAD1);//单端DAD1通道的引脚为(ADC0_DP1)
  adc_init_struct.ADC_Adcx = ADC0;
  adc_init_struct.ADC_DiffMode = ADC_SE;        //单端采集
  adc_init_struct.ADC_BitMode = SE_12BIT;       //单端12位精度
  adc_init_struct.ADC_SampleTimeCfg = SAMTIME_SHORT;    //短采样时间
  adc_init_struct.ADC_HwAvgSel = HW_4AVG;       //4次硬件平均
  adc_init_struct.ADC_CalEnable = TRUE; //使能初始化校验
  LPLD_ADC_Init(adc_init_struct);
  LPLD_ADC_Chn_Enable(ADC0, DAD3);//单端DAD3通道的引脚为(ADC0_DP3)

  adc_init_struct.ADC_Adcx = ADC1;
  adc_init_struct.ADC_DiffMode = ADC_SE;        //单端采集
  adc_init_struct.ADC_BitMode = SE_12BIT;       //单端12位精度
  adc_init_struct.ADC_SampleTimeCfg = SAMTIME_SHORT;    //短采样时间
  adc_init_struct.ADC_HwAvgSel = HW_4AVG;       //4次硬件平均
  adc_init_struct.ADC_CalEnable = TRUE; //使能初始化校验
  LPLD_ADC_Init(adc_init_struct);
  LPLD_ADC_Chn_Enable(ADC1, DAD1);

  }

void pit_init(void){//配置PIT1参数,初始化PIT1通道
  pit0_init_struct.PIT_Pitx = PIT0;
  pit0_init_struct.PIT_PeriodMs = 50; //定时周期50毫秒
  pit0_init_struct.PIT_Isr = pit_time0;  //设置中断函数
  LPLD_PIT_Init(pit0_init_struct);//初始化PIT0
  LPLD_PIT_EnableIrq(pit0_init_struct);

  pit1_init_struct.PIT_Pitx = PIT1;
  pit1_init_struct.PIT_PeriodMs = 200; //定时周期200毫秒
  pit1_init_struct.PIT_Isr = pit_time;  //设置中断函数
  LPLD_PIT_Init(pit1_init_struct);//初始化PIT1
  LPLD_PIT_EnableIrq(pit1_init_struct);

  }//使能PIT1

void pwm_init(void){
  ftm_init_struct.FTM_Ftmx = FTM0;  //使能FTM0通道
  ftm_init_struct.FTM_Mode = FTM_MODE_PWM;  //使能PWM模式
  ftm_init_struct.FTM_PwmFreq = 50; //PWM频率50Hz
  LPLD_FTM_Init(ftm_init_struct);
  LPLD_FTM_PWM_Enable(FTM0, //使用FTM0
                      FTM_Ch0, //使能Ch0通道
                      0, //初始化角度0度
                      PTC1, //使用Ch0通道的PTC1引脚
                      ALIGN_LEFT        //脉宽左对齐
                      );}

void dac_init(void)
{
  //在不使用DAC缓冲区、各种中断DMA的情况下
  //仅需初始化DAC通道即可
  dac_init_struct.DAC_Dacx = DAC0;
  LPLD_DAC_Init(dac_init_struct);
}

void SPI_Init(uint8 spi){
  SPI_InitTypeDef spi_init_param;
  //配置SPI
  spi_init_param.SPI_SPIx = SPI0;
  spi_init_param.SPI_SckDivider = SPI_SCK_DIV_64;//1.56Mhz
  LPLD_SPI_Init(spi_init_param);
  if(spi==1){
    LPLD_SPI_EnableIrq(spi_init_param);
  }
  else{
    LPLD_SPI_Deinit(spi_init_param);
  }
}

void delay(int ms){
   /* 使用SysTick实现精确毫秒级延时，避免空循环被优化器消除 */
   if (ms < 1) ms = 1;
   LPLD_SYSTICK_DelayMs(ms);
}

void portb_isr(void){
  	delay(500);
    if(LPLD_GPIO_IsPinxExt(PORTD, GPIO_Pin5)){
      if(jmms==1){
        djm=0;jmms=0;
      }
    if(gamestart)
    switch(mosixuanzi){
      case 1:break;
      case 2:if(mine[x][y] == '1')ct=1;else {biaozhi=0;panduan(x,y);};break;
      case 3:if(board[codn_y][codn_x/8] == 0){
            board[codn_y][codn_x/8] = player;
            /*pwm_init(50);                              背景音乐
            LPLD_FTM_PWM_ChangeDuty(FTM0,FTM_Ch0, 5000);
            delay(1000);
            LPLD_FTM_PWM_ChangeDuty(FTM0,FTM_Ch0, 0);*/
            if(player == 1)
            {
              rou++;
              round[6]=rou/10+48;
              round[7]=rou%10+48;
              LCD_P6x8Str(70,5,round);
              player = 2;
              LCD_P8x16Str(70,3,"PLAYER2");
            }
            else if(player == 2)
            {
              player = 1;
              LCD_P8x16Str(70,3,"PLAYER1");
            }
            if(board_scan(3) == 1)
            {
              show_win();
            }
        }break;
      case 6:if(gameing)xuanzhetx=1;
    }
  }
  if(LPLD_GPIO_IsPinxExt(PORTD, GPIO_Pin6)){
    if(mosixuanzi==1)
      if(gamestart){
        if(flag1==1)flag=0;flag1=0;if(gameover==0)flag2=1;while(PTD6_I==0);}
    switch(mosixuanzi){
      case 1:if(gamestart==1)gameovertxz=1;break;
      case 2:if(gamestart==1)gamestart=0;break;
      case 3:if(gameing==0&&gamestart==1){
            LCD_CLS();
            memset(board,0,sizeof(board));
            gameing=1;
            LCD_P8x16Str(70,3,"PLAYER1");
            LCD_P8x16Str(4,1,"GoBang");}
          else
             gamestart=0;
           break;
      case 4:if(gameing==0)gamestart=0;else gameing=0;break;
      case 6:gamestart=0;break;
    }
    if(mosixuanzi==5)
      gamestart=0;
  }
}

void pit_time0(void){
  badapple_time=0;
}

void pit_time(void){
  if(pit_n<5)
    pit_n++;
  else
    pit_n=0;

  switch(mosixuanzi){
case 1:
if(pit_n==4){
    if(timeflag==1){
    time--;
    if(time<=10&&gameover==0){
      playmusic(1);
    }
  }
}break;

case 2:
if(pit_n==4){
  if(cit==1)
    counter=500;
  if(cit==0)
    counter++;
  if(cit==2)
    counter=counter;
}break;

case 3:
if(wj_wzq==1)
if(gameing==1){
  if((pit_n%2))
    curCordBlink();
  else
    chess_board1();
}
break;
}
}

void delay_us(int us){
   int ii;
   if (us<1) us=1;
   for(ii=0;ii<us;ii++);}
