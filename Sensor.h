/************************************************************************************************************/
/*                                                                                                          */
/*                                              Sensor.h                                                    */
/*                                                                                                          */
/*                                                                                       2020. 1. 1.        */
/************************************************************************************************************/
#ifndef __SENSOR_H
#define __SENSOR_H


//������ 3�� �� �������� 2���� ������ ��Ʈ������ ����
//��Ʈ  7		6		5		4		3		2		1		0
//    	X		X		X	   OPT3    OPT2    OPT1    IND2    IND1              
#define READ_SENSOR() 		((~(PING>>3)&0x03) | (~PINE&0x1C))

#define IDT1			0x01
#define IDT2			0x02
#define PHT1			0x04
#define PHT2			0x08
#define PHT3			0x10

#define Color_Pixel	    101	//��ü�� �ȼ���(������ �ּ� �ȼ������� ���� ���� ����)
#define Coordinate_Y	102 //��ü�� X ��ǥ 0~180
#define Coordinate_X	103 //��ü�� Y ��ǥ 0~240
#define Coordinate_9	106 //��ü�� ȭ����� 9��� ��ġ  1  2  3
                            //                            4  5  6
                            //                            7  8  9
							// ������ 0


unsigned char Cmd(unsigned char color, unsigned char cmd); // �̸� ����
void Setting(unsigned char ins); // �߰��ϱ� 

void Sensor_init(void);

#endif      // __SENSOR_H
