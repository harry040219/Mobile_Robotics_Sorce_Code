/************************************************************************************************************/
/*                                                                                                          */
/*                                              MobileRobot.c                                               */
/*                                                                                                          */
/*                                                                                       2020. 1. 1.        */
/************************************************************************************************************/
#include "Interface.h"

volatile int gyaw=0;

int main(void){
    volatile int yaw=0;
    char str[21];

    Interface_init();


		
	//write_gyro(0x1E, 16);	//지자기 센서 보정 시작
	//write_gyro(0x1E, 17);	//지자기 센서 보정 종료
	//write_gyro(0x1E, 20);	//포즈센서 초기화
    gyaw=read_gyro(0x04); //YAW 상위 8비트
    gyaw<<=8;
    gyaw|=read_gyro(0x05); //YAW 하위 8비트
	
	while(1){
	    yaw=read_gyro(0x04); //YAW 상위 8비트
	    yaw<<=8;
	    yaw|=read_gyro(0x05); //YAW 하위 8비트
	    while(read_gyro(0x1E));
		lcd_clear_screen();
	    sprintf(str, "%5d", (yaw-gyaw)/100);
        lcd_display_str(0, 0, str);
		_delay_ms(500);
    }
}


