 

#include "User_Func.h"

bool user_task_en = false;	//用户线程允许允许标志位



void  UserTask_5ms(u16 dT_ms)
{
}

void  UserTask_10ms(u16 dT_ms)
{
}

void  UserTask_20ms(u16 dT_ms)
{
}

void UserTask_50ms(u16 dT_ms)
{
	
	
	
	
	//用户线程
	static bool task_100ms_en = false;
	task_100ms_en = !task_100ms_en;
	if(task_100ms_en)
		UserTask_100ms(100);
}

void UserTask_100ms(u16 dT_ms)
{
	
}

void  UserTask_500ms(uint16_t dT_ms)
{
}



