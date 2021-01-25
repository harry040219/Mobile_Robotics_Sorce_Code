/************************************************************************************************************/
/*                                                                                                          */
/*                                              Motor.c                                                     */
/*                                                                                                          */
/*                                                                                       2020. 1. 1.        */
/************************************************************************************************************/
#include "Interface.h"

#define READ_STATUS(X, Y)   ((Y) = MOTOR_CMD(X))
#define	CHECK_BUSY(X, Y)	while(((Y) = MOTOR_CMD(X)) & 0x01)

//unsigned char status1, status2, status3;


// Function  : ��� ������ ����� �����Ѵ�.
// Parameter : ����
// Return    : ����
void StartMotion(void)

{
    WriteCommand(MOTOR0, STT);
    WriteCommand(MOTOR2, STT);
    WriteCommand(MOTOR1, STT);
}

// Function  : ��� ������ ����� �����Ѵ�.
// Parameter : ����
// Return    : ����
void StopMotion(uint16_t mode)
{
    // Motor1 ����
    // LTRJ Ŀ�ǵ� ���
    WriteCommand(MOTOR0, LTRJ);
    // ��Ʈ�� ���� ���
    WriteDataTwoByte(MOTOR0, (1 << mode));

    // Motor2 ����
    // LTRJ Ŀ�ǵ� ���
    WriteCommand(MOTOR1, LTRJ);
    // ��Ʈ�� ���� ���
    WriteDataTwoByte(MOTOR1, (1 << mode));

    // Motor3 ����
    // LTRJ Ŀ�ǵ� ���
    WriteCommand(MOTOR2, LTRJ);
    // ��Ʈ�� ���� ���
    WriteDataTwoByte(MOTOR2, (1 << mode));

    // ��� ����
    WriteCommand(MOTOR0, STT);
    WriteCommand(MOTOR2, STT);
    WriteCommand(MOTOR1, STT);
}


void MOTOR_CTR(long MT0,long MT1,long MT2){
	
	unsigned char i=0;
	double V[3]={MT0,MT1,MT2};

	for(i=0;i<3;++i){
		
		V[i]=V[i]*0.2728;

		SetVelocity(i, V[i]*65536);
	}
	StartMotion();	
}
//***********************************************************************************************************


// Status Byte�� �д´�
#define READ_STATUS(X, Y)   ((Y) = MOTOR_CMD(X))
// Status Byte�� Busy Bit�� üũ�Ͽ� ���µǸ� ������ Ż���Ѵ�.
#define	CHECK_BUSY(X, Y)	while(((Y) = MOTOR_CMD(X)) & 0x01)
// status Byte�� ������ ����
unsigned char status;

// Function  : LM629���� �����͸� �д´�.
// Parameter :
//      motor - ������ ����
// Return    :
//      data  - ���� ������
unsigned char ReadData(unsigned char motor)
{
    unsigned char data;

    // Status Byte�� Busy Bit üũ
    CHECK_BUSY(motor, status);
    // ������ �Է�
    data = MOTOR_DATA(motor);

	CHECK_BUSY(motor, status);
    // �Էµ� ������ ����
    return data;
}    

// Function  : Ŀ�ǵ带 LM629�� ����.
// Parameter :
//      motor - ������ ����
//      cmd   - ������ Ŀ�ǵ�
// Return    : ����

void WriteCommand(unsigned char motor, unsigned char cmd)
{
    // Status Byte�� Busy Bit üũ
    CHECK_BUSY(motor, status);
    // Ŀ�ǵ� ���
    MOTOR_CMD(motor) = cmd;

	CHECK_BUSY(motor, status);
}

// Function  : 2 ����Ʈ �����͸� LM629�� ����.
// Parameter :
//      motor - ������ ����
//      data  - �� ������
// Return    : ����
void WriteDataTwoByte(unsigned char motor, int data)
{
    // Status Byte�� Busy Bit üũ
    CHECK_BUSY(motor, status);
    // 2 ����Ʈ ������ ���
    MOTOR_DATA(motor) = (uint8_t)((data >> 8)&0xFF);
	CHECK_BUSY(motor, status);
    MOTOR_DATA(motor) = (uint8_t)(data&0xFF);
	CHECK_BUSY(motor, status);
}

// Function  : 4 ����Ʈ �����͸� LM629�� ����.
// Parameter :
//      motor - ������ ����
//      data  - �� ������
// Return    : ����
void WriteDataFourByte(unsigned char motor, long data)
{
    // 4 ����Ʈ�� �����͸� �ѷ� ����� WriteDataTwoByte�Լ��� �ι� ȣ��
    WriteDataTwoByte(motor, (int16_t)(data >> 16));
    WriteDataTwoByte(motor, (int16_t)(data));
}

// Function  : ���� �ʱ�ȭ
// Parameter : ����
// Return    : ����
void Motor_init(void)
{
    SetGain(MOTOR0, 16, 0, 64); ////////////// 16, 0, 64�� ���� ����
    SetGain(MOTOR1, 16, 0, 64);
    SetGain(MOTOR2, 16, 0, 64);
	
	/*
	SetGain2(0,100,30,100,0x7FFF);
	SetGain2(1,100,30,100,0x7FFF);
	SetGain2(2,100,30,100,0x7FFF);
	*/
		
    SetAcceleration(MOTOR0, 0x3FFFFFFF);
    SetAcceleration(MOTOR1, 0x3FFFFFFF);
    SetAcceleration(MOTOR2, 0x3FFFFFFF);
}

// Function  : LM629�� �����Ѵ�.
// Parameter :
//      motor - ������ ����
// Return    : ����
void Motor_Reset(uint8_t motor)
{
    // RESET Ŀ�ǵ� ���
    WriteCommand(motor, RESET);
    // ������ �Ϸ�� ������ �ּ� 1.5ms�̻� ���
    _delay_ms(2);
	// Morot1 ���ͷ�Ʈ ����ũ ����
    WriteCommand(motor, RSTI);
    WriteDataTwoByte(motor, 0x0000);
}

// Function  : ���Ͱ� KP, KI, KD ���� �����Ѵ�.
// Parameter :
//      motor - ������ ����
//      kp    - P��
//      ki    - I��
//      kd    - D��
// Return    : ����
void SetGain(uint8_t motor, int16_t kp, int16_t ki, int16_t kd)
{
    unsigned char LD_kp=0, LD_ki=0, LD_kd=0;

	if(kp)LD_kp=1;
	else LD_kp=0;
	if(ki)LD_ki=1;
	else LD_ki=0;
	if(kd)LD_kd=1;
	else LD_kd=0;
	// LFIL Ŀ�ǵ� ���
    WriteCommand(motor, LFIL);
    // ��Ʈ�� ���� ���, sampling interval�� 341us ����
    WriteDataTwoByte(motor, (0x0000) | (LD_kp << 3) | (LD_ki << 2) | (LD_kd << 1)  );
    ////////////////////////////
	// P�� ���
	if(kp)WriteDataTwoByte(motor, kp);
    // I�� ���
    if(ki)WriteDataTwoByte(motor, ki);
    // D�� ���
    if(kd)WriteDataTwoByte(motor, kd);
    // ���� ������Ʈ
    WriteCommand(motor, UDF);
}

void SetGain2(uint8_t motor, int16_t kp, int16_t ki, int16_t kd, int16_t iL)
{
	unsigned char LD_kp=0, LD_ki=0, LD_kd=0;

	if(kp)LD_kp=1;
	else LD_kp=0;
	if(ki)LD_ki=1;
	else LD_ki=0;
	if(kd)LD_kd=1;
	else LD_kd=0;
	// LFIL Ŀ�ǵ� ���
	WriteCommand(motor, LFIL);
	// ��Ʈ�� ���� ���, sampling interval�� 341us ����
	WriteDataTwoByte(motor, (0x0000) | (LD_kp << 3) | (LD_ki << 2) | (LD_kd << 1) | 0x01  );
	////////////////////////////
	// P�� ���
	if(kp)WriteDataTwoByte(motor, kp);
	// I�� ���
	if(ki)WriteDataTwoByte(motor, ki);
	// D�� ���
	if(kd)WriteDataTwoByte(motor, kd);
	
	WriteDataTwoByte(motor, iL);
	
	// ���� ������Ʈ
	WriteCommand(motor, UDF);
}

// Function  : ���ӵ����� �����Ѵ�.
// Parameter :
//      motor - ������ ����
//      acc   - ���ӵ���
// Return    : ����
void SetAcceleration(uint8_t motor, int32_t acc)
{
    // LTRJ Ŀ�ǵ� ���
    WriteCommand(motor, LTRJ);
    // ��Ʈ�� ���� ���
    WriteDataTwoByte(motor, (1 << ACC_LOAD));
    // ���ӵ��� ���
    WriteDataFourByte(motor, acc);
}

// Function  : �ӵ����� �����Ѵ�.
// Parameter :
//      motor    - ������ ����
//      velocity - �ӵ� ����  (0 ~ 1,073,741,823) ���� �Է½� ���������� ȸ��
// Return    : ����
void SetVelocity(uint8_t motor, int32_t velocity)
{
	unsigned char dir=1;

	if(velocity<0){
		velocity*=(-1);
		dir=0;
	}
    // LTRJ Ŀ�ǵ� ���
    WriteCommand(motor, LTRJ);        
    // ��Ʈ�� ���� ���        
    WriteDataTwoByte(motor,  (dir << FORWARD_DIR) | (1 << VELOCITY_MODE) | (1 << VELOCITY_LOAD));
    // �ӵ��� ���
    WriteDataFourByte(motor, velocity);
}

// Function  : ���ӵ����� �����Ѵ�.
// Parameter :
//      motor - ������ ����
//      mode  - ��� ����    (0 -> Absolute Position, 1 -> Relative Position)
//		acc   - ���ӵ� ����  (0 ~ 1,073,741,823)
//   velocity - �ӵ� ����    (0 ~ 1,073,741,823)
//   position - ������ġ���� (-1,073,741,824 ~ 1,073,741,823)counts
// Return    : ����
void SetPosition(uint8_t motor, uint8_t mode, int32_t acc,int32_t velocity,int32_t position)
{
    // LTRJ Ŀ�ǵ� ���
    WriteCommand(motor, LTRJ);
    // ��Ʈ�� ���� ���
    // mode : 0 - Absolute Position, 1 - Relative Position
    // Position Mode�� ����
    
	WriteDataTwoByte(motor, (1 << ACC_LOAD) | (1 << VELOCITY_LOAD) | (1 << POSITION_LOAD) | (mode << POSITION_RELATIVE) );//WriteDataTwoByte(motor, 0x2A|mode);
    // ��ġ�� ���
	WriteDataFourByte(motor, acc);    
	WriteDataFourByte(motor, velocity);
    WriteDataFourByte(motor, position);
}

// Function  : LM629 H/W Reset. ���� ���� �ΰ��� �ݵ�� �����ؾ��Ѵ�.
// Parameter : ����
// Return    : ����
void LM629_HW_Reset(void){
	unsigned char status[3]={0, }, i=0, step=0;
	
	DDRA=0x00;

	PORTC=0x00;
	DDRC=0xFF;

	PORTG=0x00;
	DDRG=0x03;

	while(1){
		if(step==0){
			while(1){
				PORTC |= 0x05; //Hw Rst Hi(decoder = Low)
				_delay_ms(3);
				PORTC &= ~0x05; //Hw Rst Low(decoder = Hi)
				_delay_ms(2);

				PORTG &= ~0x02; //RD low
				PORTC &= ~0x08; //PS low
		
				PORTC &=~0x07;	//cs motor0
				status[0]=PINA; //Read status motor0
				
				PORTC |=0x01;	//cs motor1
				status[1]=PINA; //Read status motor0
				
				PORTC = (PORTC&0xF8)|0x02; 	//cs motor2
				status[2]=PINA;  //Read status motor0


				if( (status[0]==0xC4||status[0]==0x84) ){
						step=1;
						break;
				}
				else{
					PORTC=0x00;
					LCD(0,0,"STEP1");
					LCD(1,0,"Error1. Restart");
				}
				if((status[1]==0xC4||status[1]==0x84)){
					step=1;
					break;
				}
				else{
					PORTC=0x00;
					LCD(0,0,"STEP1");
					LCD(2,0,"Error2. Restart");
				}
				if( (status[2]==0xC4||status[2]==0x84) ){
					step=1;
					break;
				}
				else{
					PORTC=0x00;
					LCD(0,0,"STEP1");
					LCD(3,0,"Error3. Restart");
				}
			}
		}
		
		if(step==1){
			// SRE : �ܺ� ������ �޸� ���� Ȱ��ȭ
			MCUCR = (1 << SRE) | (0 << SRW10);
		    // SRL(0) : Upper Sector�θ� ��� 0x1100 ~ 0xFFFF(Upper)
		    // SRW11:SRW10(1:0) - Read/Write ��ȣ�� 2���� Wait
			XMCRA = (0 << SRL2) | (0 << SRL1) | (0 << SRL0) | (1 << SRW11);
		    //XMBK : Bus Keeper Enable, XMM(0x4) - A11 ~ A8������ �ܺ� �޸� ��Ʈ�� ���
			XMCRB = (1 << XMBK) | (1 << XMM2) | (0 << XMM1) | (0 << XMM0);
			while(1){
				for(i=0;i<3;++i){
		        	WriteCommand(i, RSTI);
					WriteDataTwoByte(i, 0x0000);
					READ_STATUS(i, status[i]);
				}
				
				if( (status[0]==0xC0||status[0]==0x80) ){
					step=2;
					break;
				}
				else{	//�ٽ� ó������ LM629�ʱ�ȭ
					step=0;	
					MCUCR &= ~(1 << SRE) & ~(1 << SRW10);
					PORTC=0x00;
					LCD(0,0,"STEP2");
					LCD(1,0,"Error1. Restart");
					break;
				}
				if((status[1]==0xC0||status[1]==0x80)){
					step=2;
					break;
				}
				else{	//�ٽ� ó������ LM629�ʱ�ȭ
					step=0;
					MCUCR &= ~(1 << SRE) & ~(1 << SRW10);
					PORTC=0x00;
					LCD(0,0,"STEP2");
					LCD(2,0,"Error2. Restart");
					break;
				}
				if((status[2]==0xC0||status[2]==0x80) ){
					step=2;
					break;
				}
				else{	//�ٽ� ó������ LM629�ʱ�ȭ
					step=0;
					MCUCR &= ~(1 << SRE) & ~(1 << SRW10);
					PORTC=0x00;
					LCD(0,0,"STEP2");
					LCD(3,0,"Error3. Restart");
					break;
				}
			}
		}
		if(step==2){
			LCD(0,0,"Initialize success.");
			for(i=0;i<3;++i){
				PORTB|=0x07;
				_delay_ms(150);
				PORTB&=~0x07;
				_delay_ms(150);
			}
			lcd_clear_screen();
			break;
		}
	}
}
	

////////////////// �Ʒ� ���� �߰� 


volatile double front, gyro,rearGyro,pos[3];



void get_gyro(){
	int yaw, ck=0;
	double now;

	retry:
	yaw=read_gyro(0x04);//YAW ����8��Ʈ
	yaw<<=8;
	yaw|=read_gyro(0x05);//YAW ����8��Ʈ
	//PSM_CMD=read_gyro(0x1E); //��ɾ��б�
	now = yaw/100;

	if(fabs(now) > 180) goto retry;

	if(front == now) return;

	if(now < 0 && front > 0){
		if( ( fabs(now) + fabs(front) ) < 180)  gyro -= front-now,ck=1;
		else								 	gyro += 359-(front-now),ck=1;
	}

	else if(now > 0 && front < 0){
		if( ( fabs(now) + fabs(front) ) < 180)	gyro += now-front,ck=1;
		else									gyro -= 359-(now-front),ck=1;
	}

	else if(!ck){
		if(now > front)	gyro += now-front;
		else if(now < front) gyro -= front-now;
	}

	front = now;
}



void speed(double fy, double fx, double fz, double w_a) 
{
   double v[3] ,ra1[3],ra2[3],da1=720-w_a,da2;
   int a=150;
   sgx=fx,sgy=fy,sga=w_a;

   if(w_a<=180)   da1 -= 360;
   da2 = da1 - 90;
   
   for(int i = 0; i < 3; i++) {
      ra1[i] = (a-da1)*cha;
      ra2[i] = (a-da2)*cha;
      a = (a+120)%360;
      v[i] = fx * cos(ra1[i]) + fy * cos(ra2[i]) - fz * (25 * M_PI / 360);
	   	SetVelocity(i, v[i] * 16777.216 * -2.55);
   }

   StartMotion();
   get_gyro();
}


void wrcm(double xs, double ys, double ang)
{
	double p[2] = {xs/100,ys/100},ane = 0, g;
	
	g = rearGyro - gyro;
   	rearGyro = gyro;
	pos[2] += g;

	if(xs || ys)
	{
		ane = pos[2] - ang;
		pos[0] += p[0] * cos(ane * cha) + p[1] * cos((ane + 90) * cha);
		pos[1] += p[1] * cos(ane * cha) + p[0] * cos((ane - 90) * cha);
	}
}
