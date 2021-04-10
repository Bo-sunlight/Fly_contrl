
#include "User_Func.h"


/*�����Ƚ����*/
void EmergencyLand_Check_Task()
{
			
	s8 AUX4_Sta = GetAuxSta(AUX4);
	
	if((AUX4_Sta==Back_Sta) || (AUX4_Sta==RC_loss_Sta))	//ң�ص���ͨ������β������ʧȥң���ź�
	{
		user_task_en = false;							//��ֹ�����û��߳�
		
		//δ������棬ִ����½����
		if(loc_ctrl_2.fb[Z]>5 || ABS(loc_ctrl_1.fb[Z])>5)
		{
			one_key_land();								//һ����½
		}
		//�ѵ�����棬ʧ�ܽ���Ȩ�ޣ�ֱ��AU1..AUX4�Ը�λ
		else
		{
			if(flag.unlock_err != Unlock_Err_Emergency)
			{
				flag.unlock_cmd = 0;					//����
				flag.unlock_err = Unlock_Err_Emergency;	//�����쳣����������쳣
			}
		}
		
	}
	else //ң�ص���ͨ��δ����β��������δʧȥң���ź�
	{
		
		//�ѽ��������������û��߳�
		if(flag.unlock_sta)
		{
			user_task_en = true;			
		}
		//δ�����������������Ƿ����㣬��ʧ���û��߳�����Ȩ��
		else	
		{
			//AUX1..3�Ը�λ���������
			if(GetAuxSta(AUX1)==Front_Sta && GetAuxSta(AUX2)==Front_Sta && GetAuxSta(AUX3)==Front_Sta)
			{
				flag.unlock_err = 0;					//�޽����쳣
			}
			//���򣬲��������
			else
			{
				flag.unlock_err = Unlock_Err_Emergency; //�����쳣����������쳣
			}
			
			user_task_en = false;						//ʧ���û��߳�����Ȩ��				
		}
		
		
	}
	
	
}

//��ȡң���ĸ�AUXͨ����λ��״̬
//��ڲ�����AUX1..AUX4
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


//һ�������
//��ڲ����������߶�(��λcm), �����������
void onekey_fly_to(float height_exp, bool condition)
{
	if(condition == true)
	{
		Ano_Parame.set.auto_take_off_height = height_exp;
		one_key_take_off();
	}
}




