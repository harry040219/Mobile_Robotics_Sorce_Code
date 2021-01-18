/************************************************************************************************************/
/*                                                                                                          */
/*                                           Motor.h                                                        */
/*                                                                                                          */
/*                                                                                       2020. 1. 1.        */
/************************************************************************************************************/
#ifndef __MOTOR_H
#define __MOTOR_H


void StartMotion(void);
void StopMotion(uint16_t mode);
void MOTOR_CTR(long MT0,long MT1,long MT2);

#define	MOTOR0			    0
#define	MOTOR1			    1
#define	MOTOR2			    2

// Memory Map
#define MOTOR_CMD(x)	    (*(volatile unsigned char *)(0x2000 + (0x100 * (x))))
#define	MOTOR_DATA(x)	    (*(volatile unsigned char *)(0x2800 + (0x100 * (x))))
#define LMD18200_OUTPUT	    (*(volatile unsigned char *)0xB00)

// LM629 User Command Set
#define	RESET			    0x00
#define	PORT8			    0x05
#define	PORT12			    0x06
#define	DFH				    0x02
#define	SIP				    0x03
#define	LPEI			    0x1B
#define	LPES			    0x1A
#define	SBPA			    0x20
#define	SBPR			    0x21
#define	MSKI			    0x1C
#define	RSTI			    0x1D
#define	LFIL			    0x1E
#define	UDF				    0x04
#define	LTRJ			    0x1F
#define	STT				    0x01
#define	RDSIGS			    0x0C
#define	RDIP			    0x09
#define	RDDP			    0x08
#define	RDRP			    0x0A
#define	RDDV			    0x07
#define	RDRV			    0x0B
#define	RDSUM			    0x0D

// Filter Control Word Bit
#define LOAD_KP             3
#define LOAD_KI             2
#define LOAD_KD             1
#define LOAD_IL             0
#define sampling			0x800

// Trajectory Control Word Bit
#define FORWARD_DIR         12
#define VELOCITY_MODE       11
#define STOP_SMOOTHLY       10
#define STOP_ABRUPTLY       9
#define TURNOFF_MOTOR       8
#define ACC_LOAD            5
#define ACC_RELATIVE        4
#define VELOCITY_LOAD       3
#define VELCOITY_RELATIVE   2
#define POSITION_LOAD       1
#define POSITION_RELATIVE   0

unsigned char ReadData(uint8_t motor);
void WriteCommand(uint8_t motor, uint8_t cmd);
void WriteDataTwoByte(uint8_t motor, int16_t data);
void WriteDataFourByte(uint8_t motor, int32_t data);

void Motor_init(void);
void Motor_Reset(uint8_t motor);
void SetGain(uint8_t motor, int16_t kp, int16_t ki, int16_t kd);
void SetGain2(uint8_t motor, int16_t kp, int16_t ki, int16_t kd, int16_t iL);

void SetAcceleration(uint8_t motor, int32_t acc);
void SetVelocity(uint8_t motor, int32_t velocity);
void SetPosition(uint8_t motor, uint8_t mode, int32_t acc,int32_t velocity,int32_t position);

void LM629_HW_Reset(void);

#endif      // __MOTOR_H
