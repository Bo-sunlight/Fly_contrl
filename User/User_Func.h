
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

//解锁异常：紧急迫降异常
#define Unlock_Err_Emergency 5


extern bool user_task_en;	//用户线程允许允许标志位

/*紧急迫降检测任务*/
void EmergencyLand_Check_Task();

//用户线程
void  UserTask_5ms	 (uint16_t dT_ms); 
void  UserTask_10ms  (uint16_t dT_ms);
void  UserTask_20ms  (uint16_t dT_ms);
void  UserTask_50ms  (uint16_t dT_ms);
void  UserTask_100ms  (uint16_t dT_ms);
void  UserTask_500ms (uint16_t dT_ms);



//遥控四个AUX通道的拨向方向枚举
typedef enum
{
	RC_loss_Sta=0,	//失控状态
	Front_Sta,		//拨向前方
	Mid_Sta,		//中间位置
	Back_Sta,		//拨向尾部
	
	InputPara_Error=-1,	//传入参数错误
	
}AUX_Sta_enum;	


//获取遥控四个AUX通道的拨向方向
//入口参数：AUX1..AUX4
s8 GetAuxSta(u8 AUX);


//一键起飞至
//入口参数：期望高度(单位cm), 允许起飞条件
void onekey_fly_to(float height_exp_cm, bool condition);


#endif

