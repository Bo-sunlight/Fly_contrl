#include "Ano_LocCtrl.h"
#include "Drv_Gps.h"
#include "Ano_Imu.h"
#include "Ano_FlightCtrl.h"
#include "Ano_OF.h"
#include "Ano_Parameter.h"


//位置速度环控制参数
_PID_arg_st loc_arg_1[2] ; 

//位置速度环控制数据
_PID_val_st loc_val_1[2] ; 

//位置速度环修正控制参数
_PID_arg_st loc_arg_1_fix[2] ; 

//位置速度环修正控制数据
_PID_val_st loc_val_1_fix[2] ; 

/*角度环PID参数初始化*/
void Loc_1level_PID_Init()
{
	if(switchs.gps_on)
	{
		loc_arg_1[X].kp = Ano_Parame.set.pid_gps_loc_1level[KP];//0.22f  ;
		loc_arg_1[X].ki = Ano_Parame.set.pid_gps_loc_1level[KI]  ;
		loc_arg_1[X].kd_ex = 0.00f ;
		loc_arg_1[X].kd_fb = Ano_Parame.set.pid_gps_loc_1level[KD];
		loc_arg_1[X].k_ff = 0.02f;
		
		loc_arg_1[Y] = loc_arg_1[X];

	}
	else if(switchs.of_flow_on)
	{
		loc_arg_1[X].kp = Ano_Parame.set.pid_loc_1level[KP];//0.22f  ;
		loc_arg_1[X].ki = 0.0f  ;
		loc_arg_1[X].kd_ex = 0.00f ;
		loc_arg_1[X].kd_fb = Ano_Parame.set.pid_loc_1level[KD];
		loc_arg_1[X].k_ff = 0.02f;
		
		loc_arg_1[Y] = loc_arg_1[X];
	//fix	
		loc_arg_1_fix[X].kp = 0.0f  ;
		loc_arg_1_fix[X].ki = Ano_Parame.set.pid_loc_1level[KI] ;
		loc_arg_1_fix[X].kd_ex = 0.00f;
		loc_arg_1_fix[X].kd_fb = 0.00f;
		loc_arg_1_fix[X].k_ff = 0.0f;
		
		loc_arg_1_fix[Y] = loc_arg_1_fix[X];	
	}
	else
	{
		//null
	}
	

}

_loc_ctrl_st loc_ctrl_1;
static float fb_speed_fix[2];
static u8 mode_f[2];
/*位置速度环*/
void Loc_1level_Ctrl(u16 dT_ms,s16 *CH_N)
{
	static float loc_hand_exp_vel[2]={0};
	static unsigned short waite_gps_loc_cnt = 0;
	float ne_pos_control[2];
	unsigned char vel_diff = 5;
	float loc_gps_h_out[2];
	float loc_gps_w_out[2];
	
	if(switchs.of_flow_on && (!switchs.gps_on))
	{
		mode_f[1] = 1;
		if(mode_f[1] != mode_f[0])
		{
			Loc_1level_PID_Init();
			mode_f[0] = mode_f[1];
		}
		////
		loc_ctrl_1.exp[X] = fs.speed_set_h[X];
		loc_ctrl_1.exp[Y] = fs.speed_set_h[Y];


		loc_ctrl_1.fb[X] = OF_DX2 + 0.03f *imu_data.h_acc[X];
		loc_ctrl_1.fb[Y] = OF_DY2 + 0.03f *imu_data.h_acc[Y];
		
		fb_speed_fix[0] = OF_DX2FIX;
		fb_speed_fix[1] = OF_DY2FIX;
		
		for(u8 i =0;i<2;i++)
		{
			PID_calculate( dT_ms*1e-3f,            //周期（单位：秒）
										loc_ctrl_1.exp[i] ,				//前馈值
										loc_ctrl_1.exp[i] ,				//期望值（设定值）
										loc_ctrl_1.fb[i] ,			//反馈值（）
										&loc_arg_1[i], //PID参数结构体
										&loc_val_1[i],	//PID数据结构体
										50,//积分误差限幅
										10 *flag.taking_off			//integration limit，积分限幅
										 )	;	
			
			//fix
			PID_calculate( dT_ms*1e-3f,            //周期（单位：秒）
										loc_ctrl_1.exp[i] ,				//前馈值
										loc_ctrl_1.exp[i] ,				//期望值（设定值）
										fb_speed_fix[i] ,			//反馈值（）
										&loc_arg_1_fix[i], //PID参数结构体
										&loc_val_1_fix[i],	//PID数据结构体
										50,//积分误差限幅
										10 *flag.taking_off			//integration limit，积分限幅
										 )	;	
			
			loc_ctrl_1.out[i] = loc_val_1[i].out + loc_val_1_fix[i].out;		
		}		


	}
	else if (switchs.gps_on)
	{
		mode_f[1] = 2;
		if(mode_f[1] != mode_f[0])
		{
			Loc_1level_PID_Init();
			mode_f[0] = mode_f[1];
		}
		////
		for(u8 j = 0; j < 2; j++)
		{
			if (fs.speed_set_h[j] != 0)				//判读是否动控制摇杆
			{
				if (ABS(loc_hand_exp_vel[j]) < ABS(fs.speed_set_h[j]))	//判断速度是否达到期望速度
				{
					if (loc_hand_exp_vel[j]*fs.speed_set_h[j] < 0)	//判断摇杆方向和期望方向是否相同
					{
						if (fs.speed_set_h[j] > 0)					//判断摇杆方向
						{
							fs.speed_set_h[j] = MAX_SPEED;			//限制最大速度
						}
						else 
						{
							fs.speed_set_h[j] = -MAX_SPEED;
						}
					}
					loc_hand_exp_vel[j] += 0.5f*dT_ms*fs.speed_set_h[j]/MAX_SPEED;		//计算期望速度
				}
				else												
				{
					if (loc_hand_exp_vel[j] > 0)					//回杆响应按最大加速度响应
					{
						loc_hand_exp_vel[j] -= vel_diff;
					}
					else
					{
						loc_hand_exp_vel[j] += vel_diff;
					}
				}
			}
			else
			{
				if (loc_hand_exp_vel[j] > vel_diff)					//回杆响应按最大加速度响应
				{
					loc_hand_exp_vel[j] -= vel_diff;
				}
				else if (loc_hand_exp_vel[j] <= -vel_diff)
				{
					loc_hand_exp_vel[j] += vel_diff;
				}
				else
				{
					loc_hand_exp_vel[j] = 0;
				}
			}
		}
		if (loc_hand_exp_vel[X] || loc_hand_exp_vel[Y])				//判断是否有手动期望速度
		{
			ne_pos_control[0] = 0;									//位置控制量清零
			ne_pos_control[1] = 0;
			waite_gps_loc_cnt = 50;									//期望速度归零之后等待500ms
		}
		else
		{
			if (waite_gps_loc_cnt > 0)
			{
				ne_pos_control[0] = 0;					//位置控制量清零
				ne_pos_control[1] = 0;
				waite_gps_loc_cnt--;
				if (waite_gps_loc_cnt == 0)				//估计位置已经稳定 记录期望位置以及对位置进行控制
				{
					Gps_information.hope_latitude = Gps_information.latitude_offset;		//期望位置等于当前位置
					Gps_information.hope_longitude = Gps_information.longitude_offset;
				}
			}
			else
			{
				Gps_information.hope_latitude_err = Gps_information.hope_latitude - Gps_information.latitude_offset;		//纬度误差
				Gps_information.hope_longitude_err = Gps_information.hope_longitude - Gps_information.longitude_offset;		//经度误差
				length_limit(&(Gps_information.hope_latitude_err), &(Gps_information.hope_longitude_err), MAX_SPEED*1.2f, ne_pos_control);	//控制模长限制
			}
		}
		
		
		loc_ctrl_1.exp[X] =  ne_pos_control[0]*Ano_Parame.set.pid_gps_loc_2level[KP] + loc_hand_exp_vel[X]*imu_data.hx_vec[0] - loc_hand_exp_vel[Y]*imu_data.hx_vec[1];		//期望速度（航向坐标转换到世界坐标NED）
		loc_ctrl_1.exp[Y] = -ne_pos_control[1]*Ano_Parame.set.pid_gps_loc_2level[KP] + loc_hand_exp_vel[X]*imu_data.hx_vec[1] + loc_hand_exp_vel[Y]*imu_data.hx_vec[0];		

		loc_ctrl_1.fb[X] =  (Gps_information.last_N_vel) + (wcx_acc_use*980/4096);			//速度反馈+加速度提前
		loc_ctrl_1.fb[Y] = -(Gps_information.last_E_vel) + (wcy_acc_use*980/4096);
		
		for(u8 i =0;i<2;i++)
		{
			PID_calculate( dT_ms*1e-3f,            //周期（单位：秒）
										loc_ctrl_1.exp[i] ,				//前馈值
										loc_ctrl_1.exp[i] ,				//期望值（设定值）
										loc_ctrl_1.fb[i] ,			//反馈值（）
										&loc_arg_1[i], //PID参数结构体
										&loc_val_1[i],	//PID数据结构体
										50,//积分误差限幅
										10 *flag.taking_off			//integration limit，积分限幅
										 )	;		

			if (!flag.taking_off)
			{
				loc_val_1[i].err_i = 0;
			}				
		}
		loc_gps_w_out[0] = loc_val_1[0].out;
		loc_gps_w_out[1] = loc_val_1[1].out;
		w2h_2d_trans(loc_gps_w_out, imu_data.hx_vec, loc_gps_h_out);	//世界坐标（NED）控制结果转换到航向坐标下
		loc_ctrl_1.out[X] = loc_gps_h_out[0];
		loc_ctrl_1.out[Y] = loc_gps_h_out[1];
	}
	else
	{
		mode_f[1] = 3;
		if(mode_f[1] != mode_f[0])
		{
			Loc_1level_PID_Init();
			mode_f[0] = mode_f[1];
		}
		////
		loc_ctrl_1.out[X] = (float)MAX_ANGLE/MAX_SPEED *fs.speed_set_h[X] ;
		loc_ctrl_1.out[Y] = (float)MAX_ANGLE/MAX_SPEED *fs.speed_set_h[Y] ;
	}
}

_loc_ctrl_st loc_ctrl_2;

