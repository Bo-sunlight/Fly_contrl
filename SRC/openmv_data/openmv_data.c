#include "openmv_data.h"

Openmv_Data Code_data;
Openmv_Data Block_data;
MCU_Data MCU_data;
unsigned char ucRxBuffer[MAX_len] ;

//�����жϵ��ô˺���������ÿ�յ�һ�����ݣ�����һ������������յ���������Ϊ��ڲ��� 
void OPNMV_DataHandle(unsigned char ucData)
{	   
    static unsigned char ucRxCnt = 0;
	ucRxBuffer[ucRxCnt++] = ucData;	             //���յ������ݴ��뻺������
	
	if ((ucRxCnt==1) && ucRxBuffer[0]!=0xA5)    //����ͷ���ԣ������¿�ʼѰ��0x55����ͷ
	{
		ucRxCnt=0;
		return;
	}
	
	if ((ucRxCnt==2) && ucRxBuffer[1]!=0x5A)
	{
		ucRxCnt=0;
		return;
	}
	
	if (ucRxCnt<MAX_len) {return;}             //���ݲ������򷵻�
    else 
	{
        
	if(ucRxBuffer[2]==0xFF)                              //���յ�ɫ������  
    {      
        memcpy(&Block_data,&ucRxBuffer[0],MAX_len);
        if((Block_data.data[3]<<8 | Block_data.data[4])<400&&(Block_data.data[5]<<8 | Block_data.data[6])<400)
        { 
              Block_data.Y =Block_data.data[3]<<8 | Block_data.data[4] ;
            Block_data.X = Block_data.data[5]<<8 | Block_data.data[6];

        }  
    }
    
	 ucRxCnt=0;                                                //��ջ�����
     
	}
}

