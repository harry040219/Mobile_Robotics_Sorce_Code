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


		
	//write_gyro(0x1E, 16);	//���ڱ� ���� ���� ����
	//write_gyro(0x1E, 17);	//���ڱ� ���� ���� ����
	//write_gyro(0x1E, 20);	//����� �ʱ�ȭ
    gyaw=read_gyro(0x04); //YAW ���� 8��Ʈ
    gyaw<<=8;
    gyaw|=read_gyro(0x05); //YAW ���� 8��Ʈ
	
	while(1){
	    yaw=read_gyro(0x04); //YAW ���� 8��Ʈ
	    yaw<<=8;
	    yaw|=read_gyro(0x05); //YAW ���� 8��Ʈ
	    while(read_gyro(0x1E));
		lcd_clear_screen();
	    sprintf(str, "%5d", (yaw-gyaw)/100);
        lcd_display_str(0, 0, str);
		_delay_ms(500);
    }
}


