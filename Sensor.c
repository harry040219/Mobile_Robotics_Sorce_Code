/************************************************************************************************************/
/*                                                                                                          */
/*                                              Sensor.c                                                    */
/*                                                                                                          */
/*                                                                                       2020. 1. 1.        */
/************************************************************************************************************/
#include "Interface.h"

void Camera_init(void){
	unsigned char buff=0;
	while(1){	//�켱���� ū�ͺ���
		putchar1(11);
		while(!rx1_flg); 
		buff=getchar1();
		if(buff){
			buff=0;
			break;
		}
	}
	while(1){	//���� �� �ּ� �ȼ���  255�ȼ�
		putchar1(17);
		putchar1(255);
		while(!rx1_flg);
		buff=getchar1();
		if(buff)break;
	}
}

/// Cmd�� �̸� ���� 
unsigned char Cmd(unsigned char color, unsigned char cmd)
{	
	volatile unsigned char buff=0, data=0;
	
	data=cmd+(color*10);

	putchar1(data);
	while(!rx1_flg);
	buff=getchar1();
	
	return buff;
}

/////////// Setting �߰�
void Setting(unsigned char ins){
	while(Cmd(0,ins+10) != (ins-10)){
		Cmd(0,ins);
	}
}


// Function  : IR, PSD ������ �ʱ�ȭ
// Parameter : ����
// Return    : ����
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

// Function  : USART�� ���� ���۵Ǵ� PSD �������� ���������� �����ϴ� ISR
// Parameter : ����
// Return    : ����
ISR(USART0_RX_vect)
{
    volatile static unsigned char rx_en = 0;
    volatile static unsigned char rx_cnt = 0;
    volatile static unsigned char chksum = 0;
    volatile static unsigned char buf[11] = {0, };
    uint8_t rxbuf = 0, i;

    rxbuf = UDR0;
    //���ſϷ� �� ���� ������� ������
    while(!(UCSR0A & 0xB8));

    if(rxbuf == 0xFF)
    {
        //���� ���� ����
        rx_en = 1;
    }
    else if(rx_en == 1)
    {
        //������ �ޱ�
        //0�������� PSD0 ����
        buf[rx_cnt++] = rxbuf;

        if(rx_cnt > 10)
        {
            // PSD 10��, üũ�� 1�� ���� �Ϸ�Ǹ�
            //�÷��� ����
            rx_en = rx_cnt = 0;

            // üũ�� ���
            for(i = 0 ; i < 10 ; i++)
                chksum += buf[i];

            //00~FE �̹Ƿ� FF������ +1 (��������)
            if(chksum == 0xFF)chksum++;

            // üũ���� �ùٸ��� üũ �� ������ ���������� �̵�
            if(chksum == buf[10])
                for(i = 0 ; i < 10 ; i++)
				{
                    if(buf[i] == 10) continue;
					psd[i] = buf[i];
					g_psd[i] = (27/((double)psd[i]/100) - 4.5);
					if(g_psd[i] > 80) g_psd[i] = 80;
				}

            //üũ�� ����
            chksum = 0;
    	}
    }
}

