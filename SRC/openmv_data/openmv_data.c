#include "openmv_data.h"

Openmv_Data Code_data;
Openmv_Data Block_data;
MCU_Data MCU_data;
unsigned char ucRxBuffer[MAX_len] ;

//串口中断调用此函数，串口每收到一个数据，调用一次这个函数，收到的数据作为入口参数 
void OPNMV_DataHandle(unsigned char ucData)
{	   
    static unsigned char ucRxCnt = 0;
	ucRxBuffer[ucRxCnt++] = ucData;	             //将收到的数据存入缓冲区中
	
	if ((ucRxCnt==1) && ucRxBuffer[0]!=0xA5)    //数据头不对，则重新开始寻找0x55数据头
	{
		ucRxCnt=0;
		return;
	}
	
	if ((ucRxCnt==2) && ucRxBuffer[1]!=0x5A)
	{
		ucRxCnt=0;
		return;
	}
	
	if (ucRxCnt<MAX_len) {return;}             //数据不满，则返回
    else 
	{
        
	if(ucRxBuffer[2]==0xFF)                              //接收到色块数据  
    {      
        memcpy(&Block_data,&ucRxBuffer[0],MAX_len);
        if((Block_data.data[3]<<8 | Block_data.data[4])<400&&(Block_data.data[5]<<8 | Block_data.data[6])<400)
        { 
              Block_data.Y =Block_data.data[3]<<8 | Block_data.data[4] ;
            Block_data.X = Block_data.data[5]<<8 | Block_data.data[6];

        }  
    }
    
	 ucRxCnt=0;                                                //清空缓存区
     
	}
}

