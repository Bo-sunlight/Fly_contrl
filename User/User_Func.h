
#ifndef __INTERFACE__H__
#define __INTERFACE__H__

#include <stdbool.h>

#include "include.h"
#include "Ano_Imu.h"
#include "Ano_RC.h"
#include "Ano_DT.h"
#include "Ano_AttCtrl.h"
#include "Ano_FlightCtrl.h"
#include "Ano_RC.h"
#include "Ano_FlightCtrl.h"
#include "Ano_Parameter.h"
#include "Ano_LocCtrl.h"

//�����쳣�������Ƚ��쳣
#define Unlock_Err_Emergency 5


extern bool user_task_en;	//�û��߳����������־λ

/*�����Ƚ��������*/
void EmergencyLand_Check_Task();

//�û��߳�
void  UserTask_5ms	 (uint16_t dT_ms); 
void  UserTask_10ms  (uint16_t dT_ms);
void  UserTask_20ms  (uint16_t dT_ms);
void  UserTask_50ms  (uint16_t dT_ms);
void  UserTask_100ms  (uint16_t dT_ms);
void  UserTask_500ms (uint16_t dT_ms);



//ң���ĸ�AUXͨ���Ĳ�����ö��
typedef enum
{
	RC_loss_Sta=0,	//ʧ��״̬
	Front_Sta,		//����ǰ��
	Mid_Sta,		//�м�λ��
	Back_Sta,		//����β��
	
	InputPara_Error=-1,	//�����������
	
}AUX_Sta_enum;	


//��ȡң���ĸ�AUXͨ���Ĳ�����
//��ڲ�����AUX1..AUX4
s8 GetAuxSta(u8 AUX);


//һ�������
//��ڲ����������߶�(��λcm), �����������
void onekey_fly_to(float height_exp_cm, bool condition);


#endif

