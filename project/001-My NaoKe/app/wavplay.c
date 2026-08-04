#include "wavplay.h"
#include "yingjian.h"
#include "common.h"

PIT_InitTypeDef pit2_init_struct;
WAV_file wav1;//wav文件
uint8 wav_buf[1024];
uint16 DApc;
uint8 CHanalnum;
uint8 Bitnum;
uint8 DACdone;
uint8 half_done;  /* 0=前半缓冲可填充, 1=后半缓冲可填充 */
extern uint8 volume;
extern FRESULT rc;     //结果代码 
extern FATFS fatfs,*fs;      // 文件系统对象 
extern FIL fil;      // 文件对象 
extern UINT bw, br;

void pit_time2(void){
	uint16 temp;
      if(Bitnum==8)//8位精度
      {
        LPLD_DAC_SetBufferDataN(DAC0,wav_buf[DApc]*10/volume,1);
        DApc++;
      }
      else if(Bitnum==16)//16位精度(先低位后高位)
      {
        temp=(((uint8)(wav_buf[DApc+1]-0x80)<<4)|(wav_buf[DApc]>>4))*10/volume;
        LPLD_DAC_SetBufferDataN(DAC0,temp,1);
        DApc+=2;
      }
    if(DApc==512){DACdone=1;half_done=0;}   /* 前半消耗完，可填充前半 */
    if(DApc==1024){DApc=0;DACdone=1;half_done=1;}  /* 后半消耗完，可填充后半 */
}

uint8 WAV_Init(uint8* pbuf)//初始化并显示文件信息
{
	if(Check_Ifo(pbuf,"RIFF"))return 1;//RIFF标志错误
	wav1.wavlen=Get_num(pbuf+4,4);//文件长度，数据偏移4byte
	if(Check_Ifo(pbuf+8,"WAVE"))return 2;//WAVE标志错误
	if(Check_Ifo(pbuf+12,"fmt "))return 3;//fmt标志错误
	wav1.formart=Get_num(pbuf+20,2);//格式类别
	wav1.CHnum=Get_num(pbuf+22,2);//通道数
	CHanalnum=wav1.CHnum;
	wav1.SampleRate=Get_num(pbuf+24,4);//采样率
	wav1.speed=Get_num(pbuf+28,4);//音频传送速率
	wav1.ajust=Get_num(pbuf+32,2);//数据块调速数
	wav1.SampleBits=Get_num(pbuf+34,2);//样本数据位数
	Bitnum=wav1.SampleBits;
	if(Check_Ifo(pbuf+36,"data"))return 4;//data标志错误
	wav1.DATAlen=Get_num(pbuf+40,4);//数据长度	
	///////////////////////////////////////////////
	/*if(wav1.wavlen<0x100000)
	{
		LCD_ShowNum(170,30,(wav1.wavlen)>>10,3,16);//文件长度
		LCD_ShowString(200,30,"Kb");
	}
	else
	{
		LCD_ShowNum(170,30,(wav1.wavlen)>>20,3,16);//文件长度
		LCD_ShowString(200,30,"Mb");
	}
	if(wav1.formart==1)LCD_ShowString(170,50,"WAV PCM");
	if(wav1.CHnum==1)LCD_ShowString(170,70,"single");
	else LCD_ShowString(170,70,"stereo");
	LCD_ShowNum(170,90,(wav1.SampleRate)/1000,3,16);//采样率
	LCD_ShowString(200,90,"KHz");
	LCD_ShowNum(170,110,(wav1.speed)/1000,3,16);//传送速度
	LCD_ShowString(200,110,"bps");
	LCD_ShowNum(177,130,wav1.SampleBits,2,16);//样本数据位数
	LCD_ShowString(200,130,"bit");*/
	return 0;
}

uint8 Playwav(uint8 qumu)
{
	uint16 i,times;
	/*F_Open(CurFile);
	F_Read(CurFile,wav_buf);//先读512字节到
	F_Read(CurFile,wav_buf+512);//再读512字节*/

	f_open(&fil, "0:/music/test.wav", FA_READ);
	f_read(&fil,wav_buf,512,&br);
	f_read(&fil,wav_buf+512,512,&br);

	while(WAV_Init(wav_buf));
	//LCD_ShowString(35,70,"format illegal!");
	//根据采样率（wav1.SampleRate）设置定时器，在中断中进行DA转换
	DACdone=0;
	DApc=44;//DA转换地址(跳过头信息)
	half_done=1;  /* 初始后半缓冲已填充，等待前半消耗完 */
	pit2_init_struct.PIT_Pitx = PIT2;
  	pit2_init_struct.PIT_PeriodUs =1000000/wav1.SampleRate; //定时周期
  	pit2_init_struct.PIT_Isr = pit_time2;  //设置中断函数
  	LPLD_PIT_Init(pit2_init_struct);//初始化PIT2
  	LPLD_PIT_EnableIrq(pit2_init_struct);
	times=(wav1.DATAlen>>10)-1;
	for(i=0;i<times;i++)//循环一次转换1KB数据
	{
		/* 等待前半缓冲消耗完，再填充前半 */
		while(!DACdone || half_done!=0);
		DACdone=0;
		f_read(&fil,wav_buf,512,&br);
		/* 等待后半缓冲消耗完，再填充后半 */
		while(!DACdone || half_done!=1);
		DACdone=0;
		f_read(&fil,wav_buf+512,512,&br);
	}
	LPLD_PIT_Deinit(pit2_init_struct);
	rc = f_close(&fil);
    if (rc) die(rc);
    //printf("\n解除挂载.\n");
    f_mount(0,NULL);
	return 0;
}

uint8 Check_Ifo(uint8* pbuf1,uint8* pbuf2)
{
	uint8 i;
	for(i=0;i<4;i++)if(pbuf1[i]!=pbuf2[i])return 1;//不同
	return 0;//相同
}

uint32 Get_num(uint8* pbuf,uint8 len)
{
    uint32 num;
	if(len==2)num=(pbuf[1]<<8)|pbuf[0];
	else if(len==4)num=(pbuf[3]<<24)|(pbuf[2]<<16)|(pbuf[1]<<8)|pbuf[0];
	return num;
}