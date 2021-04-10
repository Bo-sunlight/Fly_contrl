
#include "User_Func.h"


/*紧急迫降检查*/
void EmergencyLand_Check_Task()
{
			
	s8 AUX4_Sta = GetAuxSta(AUX4);
	
	if((AUX4_Sta==Back_Sta) || (AUX4_Sta==RC_loss_Sta))	//遥控第四通道拨向尾部，或失去遥控信号
	{
		user_task_en = false;							//禁止运行用户线程
		
		//未到达地面，执行着陆程序
		if(loc_ctrl_2.fb[Z]>5 || ABS(loc_ctrl_1.fb[Z])>5)
		{
			one_key_land();								//一键着陆
		}
		//已到达地面，失能解锁权限，直到AU1..AUX4皆复位
		else
		{
			if(flag.unlock_err != Unlock_Err_Emergency)
			{
				flag.unlock_cmd = 0;					//上锁
				flag.unlock_err = Unlock_Err_Emergency;	//解锁异常：紧急情况异常
			}
		}
		
	}
	else //遥控第四通道未拨向尾部，并且未失去遥控信号
	{
		
		//已解锁，允许运行用户线程
		if(flag.unlock_sta)
		{
			user_task_en = true;			
		}
		//未解锁，检查解锁条件是否满足，并失能用户线程运行权限
		else	
		{
			//AUX1..3皆复位，允许解锁
			if(GetAuxSta(AUX1)==Front_Sta && GetAuxSta(AUX2)==Front_Sta && GetAuxSta(AUX3)==Front_Sta)
			{
				flag.unlock_err = 0;					//无解锁异常
			}
			//否则，不允许解锁
			else
			{
				flag.unlock_err = Unlock_Err_Emergency; //解锁异常：紧急情况异常
			}
			
			user_task_en = false;						//失能用户线程运行权限				
		}
		
		
	}
	
	
}

//获取遥控四个AUX通道的位置状态
//入口参数：AUX1..AUX4
s8 GetAuxSta(u8 AUX)
{
	if(flag.rc_loss==1 || SysTick_GetTick()<5000) return RC_loss_Sta;
	
	switch(AUX)
	{
		case AUX1:case AUX4:
			if(CH_N[AUX]<-300)	return Front_Sta;
			else				return Back_Sta;
		
		case AUX2:case AUX3:
			if(CH_N[AUX]<-300)		return Front_Sta;
			else if(CH_N[AUX]>300)	return Back_Sta;
			else					return Mid_Sta;
		
		default:	return InputPara_Error;
	}
	
}


//一键起飞至
//入口参数：期望高度(单位cm), 允许起飞条件
void onekey_fly_to(float height_exp, bool condition)
{
	if(condition == true)
	{
		Ano_Parame.set.auto_take_off_height = height_exp;
		one_key_take_off();
	}
}




