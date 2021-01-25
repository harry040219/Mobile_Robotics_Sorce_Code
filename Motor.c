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


// Function  : 모든 모터의 모션을 시작한다.
// Parameter : 없음
// Return    : 없음
void StartMotion(void)

{
    WriteCommand(MOTOR0, STT);
    WriteCommand(MOTOR2, STT);
    WriteCommand(MOTOR1, STT);
}

// Function  : 모든 모터의 모션을 정지한다.
// Parameter : 없음
// Return    : 없음
void StopMotion(uint16_t mode)
{
    // Motor1 정지
    // LTRJ 커맨드 출력
    WriteCommand(MOTOR0, LTRJ);
    // 컨트롤 워드 출력
    WriteDataTwoByte(MOTOR0, (1 << mode));

    // Motor2 정지
    // LTRJ 커맨드 출력
    WriteCommand(MOTOR1, LTRJ);
    // 컨트롤 워드 출력
    WriteDataTwoByte(MOTOR1, (1 << mode));

    // Motor3 정지
    // LTRJ 커맨드 출력
    WriteCommand(MOTOR2, LTRJ);
    // 컨트롤 워드 출력
    WriteDataTwoByte(MOTOR2, (1 << mode));

    // 모션 정지
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


// Status Byte를 읽는다
#define READ_STATUS(X, Y)   ((Y) = MOTOR_CMD(X))
// Status Byte의 Busy Bit를 체크하여 리셋되면 루프를 탈출한다.
#define	CHECK_BUSY(X, Y)	while(((Y) = MOTOR_CMD(X)) & 0x01)
// status Byte를 저장할 변수
unsigned char status;

// Function  : LM629에서 데이터를 읽는다.
// Parameter :
//      motor - 선택할 모터
// Return    :
//      data  - 읽은 데이터
unsigned char ReadData(unsigned char motor)
{
    unsigned char data;

    // Status Byte의 Busy Bit 체크
    CHECK_BUSY(motor, status);
    // 데이터 입력
    data = MOTOR_DATA(motor);

	CHECK_BUSY(motor, status);
    // 입력된 데이터 리턴
    return data;
}    

// Function  : 커맨드를 LM629에 쓴다.
// Parameter :
//      motor - 선택할 모터
//      cmd   - 실행할 커맨드
// Return    : 없음

void WriteCommand(unsigned char motor, unsigned char cmd)
{
    // Status Byte의 Busy Bit 체크
    CHECK_BUSY(motor, status);
    // 커맨드 출력
    MOTOR_CMD(motor) = cmd;

	CHECK_BUSY(motor, status);
}

// Function  : 2 바이트 데이터를 LM629에 쓴다.
// Parameter :
//      motor - 선택할 모터
//      data  - 쓸 데이터
// Return    : 없음
void WriteDataTwoByte(unsigned char motor, int data)
{
    // Status Byte의 Busy Bit 체크
    CHECK_BUSY(motor, status);
    // 2 바이트 데이터 출력
    MOTOR_DATA(motor) = (uint8_t)((data >> 8)&0xFF);
	CHECK_BUSY(motor, status);
    MOTOR_DATA(motor) = (uint8_t)(data&0xFF);
	CHECK_BUSY(motor, status);
}

// Function  : 4 바이트 데이터를 LM629에 쓴다.
// Parameter :
//      motor - 선택할 모터
//      data  - 쓸 데이터
// Return    : 없음
void WriteDataFourByte(unsigned char motor, long data)
{
    // 4 바이트의 데이터를 둘로 나누어서 WriteDataTwoByte함수를 두번 호출
    WriteDataTwoByte(motor, (int16_t)(data >> 16));
    WriteDataTwoByte(motor, (int16_t)(data));
}

// Function  : 모터 초기화
// Parameter : 없음
// Return    : 없음
void Motor_init(void)
{
    SetGain(MOTOR0, 16, 0, 64); ////////////// 16, 0, 64로 전부 변경
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

// Function  : LM629를 리셋한다.
// Parameter :
//      motor - 선택할 모터
// Return    : 없음
void Motor_Reset(uint8_t motor)
{
    // RESET 커맨드 출력
    WriteCommand(motor, RESET);
    // 리셋이 완료될 때까지 최소 1.5ms이상 대기
    _delay_ms(2);
	// Morot1 인터럽트 마스크 리셋
    WriteCommand(motor, RSTI);
    WriteDataTwoByte(motor, 0x0000);
}

// Function  : 필터값 KP, KI, KD 값을 설정한다.
// Parameter :
//      motor - 선택할 모터
//      kp    - P값
//      ki    - I값
//      kd    - D값
// Return    : 없음
void SetGain(uint8_t motor, int16_t kp, int16_t ki, int16_t kd)
{
    unsigned char LD_kp=0, LD_ki=0, LD_kd=0;

	if(kp)LD_kp=1;
	else LD_kp=0;
	if(ki)LD_ki=1;
	else LD_ki=0;
	if(kd)LD_kd=1;
	else LD_kd=0;
	// LFIL 커맨드 출력
    WriteCommand(motor, LFIL);
    // 컨트롤 워드 출력, sampling interval은 341us 고정
    WriteDataTwoByte(motor, (0x0000) | (LD_kp << 3) | (LD_ki << 2) | (LD_kd << 1)  );
    ////////////////////////////
	// P값 출력
	if(kp)WriteDataTwoByte(motor, kp);
    // I값 출력
    if(ki)WriteDataTwoByte(motor, ki);
    // D값 출력
    if(kd)WriteDataTwoByte(motor, kd);
    // 필터 업데이트
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
	// LFIL 커맨드 출력
	WriteCommand(motor, LFIL);
	// 컨트롤 워드 출력, sampling interval은 341us 고정
	WriteDataTwoByte(motor, (0x0000) | (LD_kp << 3) | (LD_ki << 2) | (LD_kd << 1) | 0x01  );
	////////////////////////////
	// P값 출력
	if(kp)WriteDataTwoByte(motor, kp);
	// I값 출력
	if(ki)WriteDataTwoByte(motor, ki);
	// D값 출력
	if(kd)WriteDataTwoByte(motor, kd);
	
	WriteDataTwoByte(motor, iL);
	
	// 필터 업데이트
	WriteCommand(motor, UDF);
}

// Function  : 가속도값을 설정한다.
// Parameter :
//      motor - 선택할 모터
//      acc   - 가속도값
// Return    : 없음
void SetAcceleration(uint8_t motor, int32_t acc)
{
    // LTRJ 커맨드 출력
    WriteCommand(motor, LTRJ);
    // 컨트롤 워드 출력
    WriteDataTwoByte(motor, (1 << ACC_LOAD));
    // 가속도값 출력
    WriteDataFourByte(motor, acc);
}

// Function  : 속도값을 설정한다.
// Parameter :
//      motor    - 선택할 모터
//      velocity - 속도 설정  (0 ~ 1,073,741,823) 음수 입력시 역방향으로 회전
// Return    : 없음
void SetVelocity(uint8_t motor, int32_t velocity)
{
	unsigned char dir=1;

	if(velocity<0){
		velocity*=(-1);
		dir=0;
	}
    // LTRJ 커맨드 출력
    WriteCommand(motor, LTRJ);        
    // 컨트롤 워드 출력        
    WriteDataTwoByte(motor,  (dir << FORWARD_DIR) | (1 << VELOCITY_MODE) | (1 << VELOCITY_LOAD));
    // 속도값 출력
    WriteDataFourByte(motor, velocity);
}

// Function  : 가속도값을 설정한다.
// Parameter :
//      motor - 선택할 모터
//      mode  - 모드 설정    (0 -> Absolute Position, 1 -> Relative Position)
//		acc   - 가속도 설정  (0 ~ 1,073,741,823)
//   velocity - 속도 설정    (0 ~ 1,073,741,823)
//   position - 모터위치설정 (-1,073,741,824 ~ 1,073,741,823)counts
// Return    : 없음
void SetPosition(uint8_t motor, uint8_t mode, int32_t acc,int32_t velocity,int32_t position)
{
    // LTRJ 커맨드 출력
    WriteCommand(motor, LTRJ);
    // 컨트롤 워드 출력
    // mode : 0 - Absolute Position, 1 - Relative Position
    // Position Mode로 설정
    
	WriteDataTwoByte(motor, (1 << ACC_LOAD) | (1 << VELOCITY_LOAD) | (1 << POSITION_LOAD) | (mode << POSITION_RELATIVE) );//WriteDataTwoByte(motor, 0x2A|mode);
    // 위치값 출력
	WriteDataFourByte(motor, acc);    
	WriteDataFourByte(motor, velocity);
    WriteDataFourByte(motor, position);
}

// Function  : LM629 H/W Reset. 최초 전원 인가시 반드시 실행해야한다.
// Parameter : 없음
// Return    : 없음
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
			// SRE : 외부 데이터 메모리 영역 활성화
			MCUCR = (1 << SRE) | (0 << SRW10);
		    // SRL(0) : Upper Sector로만 사용 0x1100 ~ 0xFFFF(Upper)
		    // SRW11:SRW10(1:0) - Read/Write 신호에 2개의 Wait
			XMCRA = (0 << SRL2) | (0 << SRL1) | (0 << SRL0) | (1 << SRW11);
		    //XMBK : Bus Keeper Enable, XMM(0x4) - A11 ~ A8까지만 외부 메모리 포트로 사용
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
				else{	//다시 처음부터 LM629초기화
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
				else{	//다시 처음부터 LM629초기화
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
				else{	//다시 처음부터 LM629초기화
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
	

////////////////// 아래 전부 추가 


volatile double front, gyro,rearGyro,pos[3];



void get_gyro(){
	int yaw, ck=0;
	double now;

	retry:
	yaw=read_gyro(0x04);//YAW 상위8비트
	yaw<<=8;
	yaw|=read_gyro(0x05);//YAW 하위8비트
	//PSM_CMD=read_gyro(0x1E); //명령어읽기
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
