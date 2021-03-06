#include <msp430g2553.h>

#define LED1 0x01
#define LED2 0x02
#define LED3 0x04
#define LED4 0x10
#define LED5 0x20
#define BTN 0x08

//Parameters
#define SECONDS_TO_ADVANCE 3600 //3600 for one hour

int i = 0;
int down = 0;
int crntLED[] = {LED1, LED2, LED3, LED4, LED5};
int delay_per_led[] = {4, 32, 16, 8, 4};
volatile unsigned int seconds = 0;
volatile unsigned int pressed = 0;

void qsdelay (unsigned int time);

void main(void) {
	WDTCTL = WDTPW + WDTHOLD;

	// Timing crystal setup
	BCSCTL1 &= ~XT2OFF; // turn on the LFXT2 osc. (watch crystal)
	BCSCTL2 |= SELS; // Source SMCLK from crystal
	BCSCTL3 |= XCAP_3; //Capacitance for watch crystal

	// Output setup
	P1DIR = LED1 + LED2 + LED3 + LED4 + LED5;  //00110111
	P1OUT = 0;

	// Enable button interrupt and input settings
	P1REN |= BTN;
	P1OUT |= BTN;
	P1IES &= ~BTN;
	P1IE  |= BTN;

	// GIE
	_enable_interrupts();

	// Hourly advance setup (Setup second-ly timer and do hour check in ISR)
	TA0CCR0 = 32768;//1 second
	TA0CTL = TASSEL_1 + MC_1 + TACLR + TAIE;

	// Quarter second long timer for flash patterns
	TA1CCR0 = 2048;// 1/16 of a second
	TA1CTL = TASSEL_1 + MC_1 + TACLR + TAIE;

	// Init before enter loop, wait for things to settle and then start. 
	qsdelay(4); // 1 second is an eternity
	// Reset state variables
	i = 0;
	down = 0; // Not really needed
	seconds = 0;
	pressed = 0; // Also maybe not needed

	// Main display loop
	while (1){
		P1OUT |= crntLED[i];
		qsdelay(delay_per_led[i]+3);
		P1OUT &= ~(LED1 + LED2 + LED3 + LED4 + LED5);
		if (i) qsdelay(delay_per_led[i]-3);
	}
}

// Wait for a specified number of quarter seconds, depend on timer ISR to wakeup every 1/4
//+second
void qsdelay(unsigned int time){
	for(;time; time--){
		LPM0; //Wait
	}
	return;
}

// Advance the meter if button is pressed
void __attribute__ ((interrupt(PORT1_VECTOR))) btnPressed(void){
	P1IFG &= ~BTN;
	pressed = 1;
}

// quarter-second Timer
void __attribute__ ((interrupt(TIMER1_A1_VECTOR))) delay_isr(void){
	if (TA1IV == 0x0A){
		LPM0_EXIT;// wakeup
		// Button code, debounce by only checking button status once a quarter second
		if (pressed == 1){
			pressed++;
			if (i == 0) down = 0;
			else if (i >= 4) down = 1;
			if (down == 0) i++;
			else i--;
		}	
		else if (pressed >= 2) pressed = 0; // Only take at most one press every half second
		return;
	}
}

// Second timer, also advance if over a defined threshold
void __attribute__ ((interrupt(TIMER0_A1_VECTOR))) hour_isr(void){
	if (TAIV == 0x0A){
		if (seconds >= SECONDS_TO_ADVANCE){
			seconds = 0;
			if (i == 0) down = 0;
			else if (i >= 4) down = 1;
			if (down == 0) i++;
			else i--;
		}
		else seconds++;
	}
	return;
}
