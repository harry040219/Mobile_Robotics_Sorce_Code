/************************************************************************************************************/
/*                                                                                                          */
/*                                              Sensor.h                                                    */
/*                                                                                                          */
/*                                                                                       2020. 1. 1.        */
/************************************************************************************************************/
#ifndef __SENSOR_H
#define __SENSOR_H


//광센서 3개 및 근접센서 2개의 정보를 비트단위로 리턴
//비트  7		6		5		4		3		2		1		0
//    	X		X		X	   OPT3    OPT2    OPT1    IND2    IND1              
#define READ_SENSOR() 		((~(PING>>3)&0x03) | (~PINE&0x1C))

#define IDT1			0x01
#define IDT2			0x02
#define PHT1			0x04
#define PHT2			0x08
#define PHT3			0x10

#define Color_Pixel	    101	//물체의 픽셀수(설정란 최소 픽셀값으로 나눈 값을 리턴)
#define Coordinate_Y	102 //물체의 X 좌표 0~180
#define Coordinate_X	103 //물체의 Y 좌표 0~240
#define Coordinate_9	106 //물체의 화면사의 9등분 위치  1  2  3
                            //                            4  5  6
                            //                            7  8  9
							// 없으면 0


unsigned char Cmd(unsigned char color, unsigned char cmd); // 이름 변경
void Setting(unsigned char ins); // 추가하기 

void Sensor_init(void);

#endif      // __SENSOR_H
