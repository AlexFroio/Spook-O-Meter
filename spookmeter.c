#include <msp430g2553.h>

#define LED1 0x01
#define LED2 0x02
#define LED3 0x04
#define LED4 0x10
#define LED5 0x20
#define BTN 0x08

int i = 4;
int down = 1;
int btnPush = 0;
unsigned int cycle = 20000;
//int crntLED[] = {LED1, LED2, LED3, LED4, LED5};
int crntLED[] = {LED1, LED1, LED1, LED1, LED1};
int delay_per_led[] = {1, 8, 4, 2, 1};
volatile int state = 1;
volatile int flag = 0;
volatile unsigned int debounce=0;
volatile unsigned int delay_flag = 0;

void hsdelay (unsigned int);

void main(void) {
	WDTCTL = WDTPW + WDTHOLD;
	DCOCTL = CALDCO_1MHZ;
	BCSCTL1	= CALBC1_1MHZ;

	P1DIR = LED1 + LED2 + LED3 + LED4 + LED5;  //00110111
	P1OUT = 0;

	P1REN = BTN;

	P1IES &= ~BTN;
	P1IE  = BTN;
	_enable_interrupts();

	while (1){
		P1OUT |= crntLED[i];
		hsdelay(delay_per_led[i]);
		P1OUT &= ~(LED1 + LED2 + LED3 + LED4 + LED5);
		if (i) hsdelay(delay_per_led[i]);
	}


}

void hsdelay(unsigned int time){
	TA1CCR0 = 1000;
	TA1CTL = TASSEL_1 + MC_1 + TACLR + TAIE;
	for (unsigned int time_left=time; time_left; time_left--){
		for (unsigned int delay_time=250; delay_time; delay_time--){
			delay_flag = 1;
			while (delay_flag);
		}
		__no_operation();
	}
	TA1CTL = 0;		
	return;
}

void __attribute__ ((interrupt(PORT1_VECTOR))) btnPressed(void){
	flag = 1;
	if (down == 0) i++;
	else i--;


	if (i == 0) down = 0;
	else if (i == 4) down = 1;
}



// Millisecond Timer
void __attribute__ ((interrupt(TIMER1_A1_VECTOR))) delay_isr(void){
	if (TA1IV == 0x0A){
		delay_flag = 0;
		return;
	}
}

