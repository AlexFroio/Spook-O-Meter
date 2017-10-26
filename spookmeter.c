#include <msp430g2553.h>

#define LED1 0x01
#define LED2 0x02
#define LED3 0x04
#define LED4 0x10
#define LED5 0x20
#define BTN 0x08

int i = 0;
int down = 0;
int btnPush = 0;
int cycle = 200;
int crntLED[] = {0x01, 0x02, 0x04, 0x10, 0x20};
volatile int state = 1;
volatile int flag = 0;
volatile unsigned int debounce=0;
volatile unsigned int delay_flag = 0;

void msdelay (unsigned int);

void main(void) {
	DCOCTL = CALDCO_1MHZ;
	BCSCTL1	= CALBC1_1MHZ;
	
	//pinMode(2,OUTPUT);
	//pinMode(3,OUTPUT);
	//pinMode(4,OUTPUT);
	//pinMode(6,OUTPUT);
	//pinMode(7,OUTPUT);
	P1DIR = LED1 + LED2 + LED3 + LED4 + LED5;  //00110111

	//pinMode(5,INPUT_PULLUP);
	P1REN = BTN;

	//attachInterrupt(5, btnPressed, RISING);
	P1IES = BTN;
	P1IE  = BTN;
	_EINT(); //_enable_interrupts();

	//if ( i == 0){
	//	digitalWrite(2,HIGH);
	//	delay(cycle);
	//	digitalWrite(2,LOW);
	//}
	//else if (i == 1){
	//	digitalWrite(3,HIGH);
	//	delay(cycle*4);
	//	digitalWrite(3,LOW);
	//	delay(cycle*4);
	//}
	//else if (i == 2){
	//	digitalWrite(4,HIGH);
	//	delay(cycle*2);
	//	digitalWrite(4,LOW);
	//	delay(cycle*2);
	//}
	//else if (i == 3){
	//	digitalWrite(6,HIGH);
	//	delay(cycle);
	//	digitalWrite(6,LOW);
	//	delay(cycle);
	//}
	//else if (i == 4){
	//	digitalWrite(7,HIGH);
	//	delay(cycle/2);
	//	digitalWrite(7,LOW);
	//	delay(cycle/2);
	//}
	while (1){
		P1OUT = crntLED[i];
		msdelay(cycle);
		P1OUT = 0;
	}


}

void msdelay(unsigned int time){
	TACCR0 = 1000;
	TA1CTL = TASSEL_1 + MC_1 + TACLR + TAIE;
	for ( ; time > 0; time--){
		delay_flag = 1;
		while (delay_flag != 0);
	}
	TA1CTL = 0;		
}

#pragma vector=PORT1_VECTOR
__interrupt void btnPressed(void){
	flag = 1;
	if (down == 0){
		i = i + 1;}
	else if (down == 1){
		i = i - 1;
	}
	if (i == 0){
		down = 0;}
	else if (i == 4){
		down = 1;};
}

#pragma vactor=TIMER1_A1_VECTOR
__interrupt void delay_isr(void){
	TAIV = 0;
	delay_flag = 0;
	LPM0_EXIT;
}
