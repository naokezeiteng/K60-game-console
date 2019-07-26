#include "common.h"
#include "LQ12864.h"
#include "yingjian.h"
#include "jianmian.h"
#include "tuixiangzi.h"
#include "saolei.h"
#include "wuziqi.h"
#include "huatu.h"
//#include "wavplay.h"  ���ʧ��
void start_pic(void);//��ʼ����
void readsd(void);
void die(FRESULT rc);
DWORD get_fattime (void);// �û��Զ����ΪFatFsϵͳ�ṩʵʱʱ��ĺ���
void badapple(uint8 spshu);//��Ƶ����
//void music(void);//���ֲ���
void huatu(void);//�Զ����ͼ����
void flash_start(void);//��Ƶ����ѡ�����
// ���±������������FatFs�ļ�ϵͳ��������
FRESULT rc;     //�������
FATFS fatfs,*fs;      // �ļ�ϵͳ����
FIL fil;      // �ļ�����
UINT bw, br;
unsigned char buff_86[688],buff_114[912];//86*64�ֱ���|114*64�ֱ���
//ͨ�ñ�־λ
uint16 result,result1;//����ADCģ��Ĳɼ�����
uint16 key_up,key_down,key_left,key_right;//
uint16 mosixuanzi=1;
uint16 gamestart=0,gameing=0;
uint8 badapple_time=1;
//ɵ��������־λ
uint16 flag=1;//����
uint16 flag1=0,flag2=0;
uint16 time=60;
uint16 timeflag=0;
uint16 gameover=0;
uint8 wj_wzq=0;

void main (void){
  init_gpio();//��ʼ��gpio��
  LCD_Init();//��ʼ��OLED
  adc_init();//��ʼ��ADC
  pit_init();
  pwm_init();//��ʼ��PWM
  //dac_init();//��ʼ��DACģ��
  start_pic();
  //gameing=1;mosixuanzi=6;gamestart=1;
  //huatu();
  while(1){
    jianmian();
    if(gamestart==1)
    switch(mosixuanzi){
      case 1:gameover=0;tuixiangzi();break;
      case 2:gamestart=1;gameing=1;saolei();break;
      case 3:gameing=0;wj_wzq=1;wuziqi();wj_wzq=0;break;
      case 4:gamestart=1;gameing=0;flash_start();break;
      case 5:gamestart=1;gameing=0;xtjianmian();break;
    }
    LCD_CLS();
  }
}

void start_pic(void){
	gameover=1;
  uint16 pic=1;
  LCD_CLS();
  //LCD_P128x64Ch();
  for(uint8 i=0;i<6;i++)//ͼ�ν���ϵͳ
    LCD_P14x16Ch(i*16+10,0,i+66);

  for(uint8 i=0;i<4;i++)//ָ����ʦ
    LCD_P14x16Ch(i*15,4,i+66+6);
  for(uint8 i=0;i<2;i++)//11
    LCD_P14x16Ch(i*22+15*4+14,4,i+66+10);

  for(uint8 i=0;i<3;i++)//�㱨��
    LCD_P14x16Ch(i*18,6,i+66+12);
  for(uint8 i=0;i<3;i++)//111
    LCD_P14x16Ch(i*15+15*4+14,6,i+66+15);
    LCD_P8x16Str(58,4,":");
    LCD_P8x16Str(16*3,6,":");

  while(pic){
  key();
  if(key_up==0||key_down==0){
      delay(500);
      if(key_up==0||key_down==0){
        pic=0;
      }
    }
}
while(1){
	key();
	if(key_up==1&&key_down==1)
		delay(500);
      if(key_up==1&&key_down==1)
      	break;
}
  LCD_CLS();
}

void die(FRESULT rc)
{
  //printf("������� rc=%u.\n", rc);
  LCD_P8x16Str(30,2,"SD Error!");
  LCD_P8x16Str(30,4,"Check it");
  rc = f_close(&fil);
  f_mount(0,NULL);
  while(1);
}
// �û��Զ����ΪFatFsϵͳ�ṩʵʱʱ��ĺ���
DWORD get_fattime (void)
{
  return ((DWORD)(2018 - 1980) << 25) //2018��
       | ((DWORD)12 << 21)               //12��
       | ((DWORD)12 << 16)              //12��
       | ((DWORD)0 << 11)
       | ((DWORD)0 << 5)
       | ((DWORD)0 >> 1);
}

void readsd(void)
{
  DWORD fre_clust,fre_sect,tot_sect;
  unsigned char s[]="00000";
  // ע��һ�����̹�����
   f_mount(0,&fatfs);
   //printf("\n��ȡ����.\n");
   rc = f_getfree("0:", &fre_clust, &fs);
   if (rc) die(rc);
   /* Get total sectors and free sectors */
   tot_sect=(fs->n_fatent-2)*fs->csize;
   fre_sect=fre_clust*fs->csize;
   LCD_P14x16Ch(6,2,46);
   LCD_P14x16Ch(30-10,2,42);
   LCD_P14x16Ch(44-10,2,43);
   LCD_P8x16Str(59-10,2,":");
   LCD_P14x16Ch(30-10,4,44);
   LCD_P14x16Ch(44-10,4,45);
   LCD_P8x16Str(59-10,4,":");
    s[0]=(int)((fre_sect/2/1024))/10000+48;
    s[1]=(int)((fre_sect/2/1024))/1000%10+48;
    s[2]=(int)((fre_sect/2/1024))/100%10+48;
    s[3]=(int)((fre_sect/2/1024))/10%10+48;
    s[4]=(int)((fre_sect/2/1024))%10+48;
    LCD_P8x16Str(58,4,s);
    LCD_P8x16Str(58+5*8,2,"MB");
    s[0]=(int)((tot_sect/2/1024))/10000+48;
    s[1]=(int)((tot_sect/2/1024))/1000%10+48;
    s[2]=(int)((tot_sect/2/1024))/100%10+48;
    s[3]=(int)((tot_sect/2/1024))/10%10+48;
    s[4]=(int)((tot_sect/2/1024))%10+48;
    LCD_P8x16Str(58,2,s);
    LCD_P8x16Str(58+5*8,4,"MB");
    //printf("%lu MB total drive space.\n""%lu MB available.\n",fre_sect/2/1024,tot_sect/2/1024);
    while(gamestart);
    f_close(&fil);
    if (rc) die(rc);
    f_mount(0,NULL);
}

void badapple(uint8 spshu){
  uint16 i;
  // ע��һ�����̹�����
  //printf("1\n");
  rc=f_mount(0,&fatfs);
  if (rc) die(rc);
  //�򿪵��ļ�
  //printf("2\n");
  switch(spshu){
    case 1:rc=f_open(&fil, "0:/cartoon/badapple.bin", FA_READ);
          if (rc) die(rc);
          break;
    case 2:rc=f_open(&fil, "0:/cartoon/jljt.bin", FA_READ);
          if (rc) die(rc);
          break;
  }
  //printf("3\n");
  //��ӡ���ļ��ڵ�����
  //printf("��ӡ���ļ�����.\n");
  while(1)
  {
    switch(spshu){
      case 1:{
        rc = f_read(&fil,buff_86,sizeof(buff_86),&br); // ��ȡ�ļ���һ��
        if (rc || !br) break;     // ������ȡ���
        LCD_siping(21,86);
        break;}
      case 2:{
        rc = f_read(&fil,buff_114,sizeof(buff_114),&br); // ��ȡ�ļ���һ��
        if (rc || !br) break;     // ������ȡ���
        LCD_siping(7,114);
        break;}
    }
        	while(badapple_time);
          badapple_time=1;
      if(gameing==0){
        rc = f_close(&fil);
        if (rc) die(rc);
        //printf("\n�������.\n");
        f_mount(0,NULL);
        return;
      }
  }
  //�ر��ļ�
  //printf("\n�ر��ļ�.\n");
  rc=f_close(&fil);
  if (rc) die(rc);
  //printf("\n�������.\n");
  f_mount(0,NULL);
  if (rc) die(rc);
    while(gameing);
}

void flash_start(void){
  uint8 spshu=1;
  LCD_P14x16Ch(105,spshu*2+1,16);
  uint8 a=33;
  uint8 b=45;
  uint8 d=1;
while(1){
while(d){
  if(gamestart==0)
    return;
  key();
  LCD_P8x16Str(16,0,"Flash Player");
  LCD_P8x16Str(20,3,"Bad Apple");
  LCD_P8x16Str(20,5,"Garnidelia");
  LCD_P14x16Ch(105,spshu*2+1,16);
    if(key_up==0){
      delay(500);
      if(key_up==0){
        spshu--;
        playmusic(2);
        if(spshu==0)
          spshu=2;
    LCD_P14x16Ch(105,3,15);
    LCD_P14x16Ch(105,5,15);
    //LCD_P14x16Ch(105,6,15);
    LCD_P14x16Ch(105,spshu*2+1,16);
      }while(!key_up){key();}
    }
    if(key_down==0){
      delay(500);
      if(key_down==0){
        spshu++;
        playmusic(2);
        if(spshu==3)
          spshu=1;
    LCD_P14x16Ch(105,3,15);
    LCD_P14x16Ch(105,5,15);
    //LCD_P14x16Ch(105,6,15);
    LCD_P14x16Ch(105,spshu*2+1,16);
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
  badapple(spshu);
  LCD_CLS();
  gamestart=1;d=1;
  delay(100);
  }
}