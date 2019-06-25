#include "wavplay.h"
#include "yingjian.h"
#include "common.h"

PIT_InitTypeDef pit2_init_struct;
WAV_file wav1;//wav�ļ�
uint8 wav_buf[1024];
uint16 DApc;
uint8 CHanalnum;
uint8 Bitnum;
uint8 DACdone;
extern uint8 volume;
extern FRESULT rc;     //������� 
extern FATFS fatfs,*fs;      // �ļ�ϵͳ���� 
extern FIL fil;      // �ļ����� 
extern UINT bw, br;

void pit_time2(void){
	uint16 temp;
      if(Bitnum==8)//8λ����
      {
        //DAC->DHR12R1=wav_buf[DApc]*10/volume;//ͨ��1��12λ�Ҷ�������
        LPLD_DAC_SetBufferDataN(DAC0,wav_buf[DApc]*10/volume,1);
        DApc++;
      }
      else if(Bitnum==16)//16λ����(�ȵ�λ���λ)
      {
        temp=(((uint8)(wav_buf[DApc+1]-0x80)<<4)|(wav_buf[DApc]>>4))*10/volume;
        LPLD_DAC_SetBufferDataN(DAC0,temp,1);
        DApc+=2;        
      } 
    if(DApc==512)DACdone=1;
    if(DApc==1024){DApc=0;DACdone=1;}                                         
}

uint8 WAV_Init(uint8* pbuf)//��ʼ������ʾ�ļ���Ϣ
{
	if(Check_Ifo(pbuf,"RIFF"))return 1;//RIFF��־����
	wav1.wavlen=Get_num(pbuf+4,4);//�ļ����ȣ�����ƫ��4byte
	if(Check_Ifo(pbuf+8,"WAVE"))return 2;//WAVE��־����
	if(Check_Ifo(pbuf+12,"fmt "))return 3;//fmt��־����
	wav1.formart=Get_num(pbuf+20,2);//��ʽ���
	wav1.CHnum=Get_num(pbuf+22,2);//ͨ����
	CHanalnum=wav1.CHnum;
	wav1.SampleRate=Get_num(pbuf+24,4);//������
	wav1.speed=Get_num(pbuf+28,4);//��Ƶ��������
	wav1.ajust=Get_num(pbuf+32,2);//���ݿ������
	wav1.SampleBits=Get_num(pbuf+34,2);//��������λ��
	Bitnum=wav1.SampleBits;
	if(Check_Ifo(pbuf+36,"data"))return 4;//data��־����
	wav1.DATAlen=Get_num(pbuf+40,4);//���ݳ���	
	///////////////////////////////////////////////
	/*if(wav1.wavlen<0x100000)
	{
		LCD_ShowNum(170,30,(wav1.wavlen)>>10,3,16);//�ļ�����
		LCD_ShowString(200,30,"Kb");
	}
	else
	{
		LCD_ShowNum(170,30,(wav1.wavlen)>>20,3,16);//�ļ�����
		LCD_ShowString(200,30,"Mb");
	}
	if(wav1.formart==1)LCD_ShowString(170,50,"WAV PCM");
	if(wav1.CHnum==1)LCD_ShowString(170,70,"single");
	else LCD_ShowString(170,70,"stereo");
	LCD_ShowNum(170,90,(wav1.SampleRate)/1000,3,16);//������
	LCD_ShowString(200,90,"KHz");
	LCD_ShowNum(170,110,(wav1.speed)/1000,3,16);//�����ٶ�
	LCD_ShowString(200,110,"bps");
	LCD_ShowNum(177,130,wav1.SampleBits,2,16);//��������λ��
	LCD_ShowString(200,130,"bit");*/
	return 0;
}

uint8 Playwav(uint8 qumu)
{
	uint16 i,times;
	/*F_Open(CurFile);
	F_Read(CurFile,wav_buf);//�ȶ�512�ֽڵ�
	F_Read(CurFile,wav_buf+512);//�ٶ�512�ֽ�*/

	f_open(&fil, "0:/music/test.wav", FA_READ);
	f_read(&fil,wav_buf,512,&br);
	f_read(&fil,wav_buf+512,512,&br);

	while(WAV_Init(wav_buf));
	//LCD_ShowString(35,70,"format illegal!");
	//���ݲ����ʣ�wav1.SampleRate�����ö�ʱ�������ж��н���DAת��
	DACdone=0;
	DApc=44;//DAת����ַ(����ͷ��Ϣ)
	//LCD_DrawRectangle(18,258,222,272);//���ȿ�
	//LCD_Fill(20,260,220,270,WHITE);//������
	pit2_init_struct.PIT_Pitx = PIT2;
  	pit2_init_struct.PIT_PeriodUs =1000000/wav1.SampleRate; //��ʱ����
  	pit2_init_struct.PIT_Isr = pit_time2;  //�����жϺ���
  	LPLD_PIT_Init(pit2_init_struct);//��ʼ��PIT2 
  	LPLD_PIT_EnableIrq(pit2_init_struct);
	//Timerx_Init(1000000/wav1.SampleRate,72);//1MHz�ļ���Ƶ��,�����Ͳ�����һ�����ж�Ƶ��
	times=(wav1.DATAlen>>10)-1;
	for(i=0;i<times;i++)//ѭ��һ��ת��1KB����
	{	
		while(!DACdone);//�ȴ�ǰ��512�ֽ�ת�����
		DACdone=0;
		//F_Read(CurFile,wav_buf);//��512�ֽ�
		f_read(&fil,wav_buf,512,&br);
		//LCD_Fill(20,260,20+(200*i/times),270,BLUE);//������
		while(!DACdone);//�ȴ�ǰ��512�ֽ�ת�����
		DACdone=0;
		//F_Read(CurFile,wav_buf+512);//��512�ֽ�
		f_read(&fil,wav_buf+512,512,&br);	
	}
	LPLD_PIT_Deinit(pit2_init_struct);
	rc = f_close(&fil);
    if (rc) die(rc);
    //printf("\n�������.\n");
    f_mount(0,NULL);
	return 0;
}

uint8 Check_Ifo(uint8* pbuf1,uint8* pbuf2)
{
	uint8 i;
	for(i=0;i<4;i++)if(pbuf1[i]!=pbuf2[i])return 1;//��ͬ
	return 0;//��ͬ
}

uint32 Get_num(uint8* pbuf,uint8 len)
{
    uint32 num;
	if(len==2)num=(pbuf[1]<<8)|pbuf[0];
	else if(len==4)num=(pbuf[3]<<24)|(pbuf[2]<<16)|(pbuf[1]<<8)|pbuf[0];
	return num;
}