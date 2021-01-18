/************************************************************************************************************/
/*                                                                                                          */
/*                                              Sensor.c                                                    */
/*                                                                                                          */
/*                                                                                       2020. 1. 1.        */
/************************************************************************************************************/
#include "Interface.h"

void Camera_init(void){
	unsigned char buff=0;
	while(1){	//우선순위 큰것부터
		putchar1(11);
		while(!rx1_flg); 
		buff=getchar1();
		if(buff){
			buff=0;
			break;
		}
	}
	while(1){	//추적 할 최소 픽셀수  255픽셀
		putchar1(17);
		putchar1(255);
		while(!rx1_flg);
		buff=getchar1();
		if(buff)break;
	}
}

/// Cmd로 이름 변경 
unsigned char Cmd(unsigned char color, unsigned char cmd)
{	
	volatile unsigned char buff=0, data=0;
	
	data=cmd+(color*10);

	putchar1(data);
	while(!rx1_flg);
	buff=getchar1();
	
	return buff;
}

/////////// Setting 추가
void Setting(unsigned char ins){
	while(Cmd(0,ins+10) != (ins-10)){
		Cmd(0,ins);
	}
}


// Function  : IR, PSD 센서를 초기화
// Parameter : 없음
// Return    : 없음
void Sensor_init(void)
{
    DDRE  &= ~0x1C;
    DDRG  &= ~0x18;

    UCSR0A = 0x00;
    UCSR0B = 0x98;
    UCSR0C = 0x06;
    UBRR0H = 0x00;
    UBRR0L = 0x0F;

    sei();
}

// Function  : USART를 통해 전송되는 PSD 센서값을 전역변수에 저장하는 ISR
// Parameter : 없음
// Return    : 없음
ISR(USART0_RX_vect)
{
    volatile static unsigned char rx_en = 0;
    volatile static unsigned char rx_cnt = 0;
    volatile static unsigned char chksum = 0;
    volatile static unsigned char buf[11] = {0, };
    uint8_t rxbuf = 0, i;

    rxbuf = UDR0;
    //수신완료 및 에러 검출되지 않을시
    while(!(UCSR0A & 0xB8));

    if(rxbuf == 0xFF)
    {
        //시작 버퍼 수신
        rx_en = 1;
    }
    else if(rx_en == 1)
    {
        //데이터 받기
        //0번지부터 PSD0 저장
        buf[rx_cnt++] = rxbuf;

        if(rx_cnt > 10)
        {
            // PSD 10개, 체크섬 1개 수신 완료되면
            //플래그 리셋
            rx_en = rx_cnt = 0;

            // 체크섬 계산
            for(i = 0 ; i < 10 ; i++)
                chksum += buf[i];

            //00~FE 이므로 FF나오면 +1 (오류방지)
            if(chksum == 0xFF)chksum++;

            // 체크섬이 올바른지 체크 후 맞으면 전역변수로 이동
            if(chksum == buf[10])
                for(i = 0 ; i < 10 ; i++)
				{
                    if(buf[i] == 10) continue;
					psd[i] = buf[i];
					g_psd[i] = (27/((double)psd[i]/100) - 4.5);
					if(g_psd[i] > 80) g_psd[i] = 80;
				}

            //체크섬 리셋
            chksum = 0;
    	}
    }
}

