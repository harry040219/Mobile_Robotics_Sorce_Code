/************************************************************************************************************/
/*                                                                                                          */
/*                                                Move.c                                                    */
/*                                                                                                          */
/*                                                                                       2020. 1. 1.        */
/************************************************************************************************************/
#include "Interface.h"


ISR(TIMER1_OVF_vect){
	TCNT1H=0xFF; TCNT1L=0x70; //0.01ÃÊ
}

void non_Holonomic(float Fx, float Fy, float Fw){

	float V[3]={0,0,0};

	V[0]=( 0.056*Fx)+(0.033*Fy)+(0.14*Fw);
	V[1]=(-0.065*Fy)+(0.14*Fw);
	V[2]=(-0.056*Fx)+(0.033*Fy)+(0.14*Fw);

	SetVelocity(0, V[0]*65536);
	SetVelocity(1, V[1]*65536);
	SetVelocity(2, V[2]*65536);

	StartMotion();
}

void HolonomicW(float f_agl, float f_speed, float fw_speed){
	long Fx=0, Fy=0, Fw=0; //¼Óµµ

	Fx = f_speed * cos(f_agl*0.017453);
	Fy = f_speed * sin(f_agl*0.017453);
	Fw=fw_speed;
	if(f_agl>=360||f_agl<0){
		Fx=0;
		Fy=0;
		Fw=f_speed;
		if(f_agl<0)Fw=-f_speed;
	}	

	non_Holonomic(Fx,Fy,Fw);
}

