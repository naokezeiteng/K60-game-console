#include "vs10xx.h"	
#include "common.h"
#include "yingjian.h"	  	    
////////////////////////////////////////////////////////////////////////////////
//移植时候的接口
//data:要写入的数据
//返回值:读到的数据
uint8 VS_SPI_ReadWriteByte(uint8 data)
{
	return LPLD_SPI_Master_Write(SPI0,data,SPI_PCS0,SPI_PCS_ASSERTED);
}
  
////////////////////////////////////////////////////////////////////////////////	 	 
//硬复位MP3
//返回1:复位失败;0:复位成功	   
void VS_HD_Reset(void)
{
	uint8 retry=0;
	//VS_RST=0;
	LPLD_GPIO_Output_b(PTC,1,0);
	delay(100);
	//VS_XDCS=1;//取消数据传输
	LPLD_GPIO_Output_b(PTC,3,1);
	//VS_XCS=1;//取消数据传输
	LPLD_GPIO_Output_b(PTC,2,1);
	//VS_RST=1;
	LPLD_GPIO_Output_b(PTC,1,1);
	//VS_WR_Cmd(0x00,0x08,0x04);
	delay(10);	   
	while(VS_DQ==0&&retry<200)//等待DREQ为高
	{
		retry++;
		delay_us(50);
	};
	delay(100);
	//if(retry>=200)return 1;
	//else return 0;
	/* VS_WR_Cmd(0x03,0x98,0x00);
	delay(10);
    VS_WR_Cmd(0x05,0xbb,0x81);
	delay(10);
	VS_WR_Cmd(0x02,0x00,0x55);
	delay(10);
	VS_WR_Cmd(0x0b,0x20,0x20);
	delay(10);
	LPLD_SPI_Master_Write(SPI0,0,SPI_PCS0,SPI_PCS_ASSERTED);
	LPLD_SPI_Master_Write(SPI0,0,SPI_PCS0,SPI_PCS_ASSERTED);
	LPLD_SPI_Master_Write(SPI0,0,SPI_PCS0,SPI_PCS_ASSERTED);
	LPLD_SPI_Master_Write(SPI0,0,SPI_PCS0,SPI_PCS_ASSERTED);  */		    		 
}
//向VS10XX写命令
//address:命令地址
//data:命令数据
void VS_WR_Cmd(uint8 address,uint16 data){
	while(!VS_DQ);//等待空闲		  	   
	//VS_XDCS=1;
	LPLD_GPIO_Output_b(PTC,3,1); 	 
	//VS_XCS=0;
	LPLD_GPIO_Output_b(PTC,2,0); 	 
	VS_SPI_ReadWriteByte(VS_WRITE_COMMAND);//发送VS10XX的写命令
	VS_SPI_ReadWriteByte(address); 	//地址
	VS_SPI_ReadWriteByte(data>>8); 	//发送高八位
	VS_SPI_ReadWriteByte(data);	 	//第八位
	//VS_XCS=1;
	LPLD_GPIO_Output_b(PTC,2,1);              
} 

//发送一次音频数据
//固定为32字节
//返回值:0,发送成功
//		 1,VS10xx不缺数据,本次数据未成功发送    
uint8 VS_Send_MusicData(uint8* buf)
{
	uint8 n;
	if(VS_DQ!=0)  //送数据给VS10XX
	{			   	 
		//VS_XDCS=0;
		LPLD_GPIO_Output_b(PTC,3,0);  
        for(n=0;n<32;n++)
		{
			VS_SPI_ReadWriteByte(buf[n]);	 			
		}
		//VS_XDCS=1;
		LPLD_GPIO_Output_b(PTC,3,1);     				   
	}else return 1;
	return 0;//成功发送了
}

//设定VS10XX播放的音量
//volx:音量大小(0~254)
/* void VS_Set_Vol(uint8 volx)
{
    uint16 volt=0; 			//暂存音量值
    volt=254-volx;			//取反一下,得到最大值,表示最大的表示 
	volt<<=8;
    volt+=254-volx;			//得到音量设置后大小
    VS_WR_Cmd(SPI_VOL,volt);//设音量 
} */