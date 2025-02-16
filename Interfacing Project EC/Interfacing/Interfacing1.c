#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Global variables to store time values
unsigned char s = 0, m = 0, h = 0;
unsigned char mode = 1; // 1: Count Up, 0: Count Down

// Function Prototypes
void Timer1_Init(void);
void INT0_Init(void);
void INT1_Init(void);
void INT2_Init(void);
void count_up(void);
void count_down(void);
void PIN_DIRECTIONS(void);
void DISPLAY(void);
void MODE(void);
void ADJUST_TIME(void);

// Interrupt Service Routines (ISRs)
ISR(TIMER1_COMPA_vect);
ISR(INT0_vect);
ISR(INT1_vect);
ISR(INT2_vect);

int main(void){
	sei();  // Enable global interrupts
	Timer1_Init();  // Initialize Timer1
	INT0_Init(); // Initialize Interrupt 0 (Reset)
	INT1_Init(); // Initialize Interrupt 1 (Pause)
	INT2_Init(); // Initialize Interrupt 2 (Resume)
	PIN_DIRECTIONS(); // Configure I/O pins

    while(1){
    	DISPLAY();
    	MODE();
    	ADJUST_TIME();
    }
}

// Timer1 Initialization (CTC Mode, 1-second interval)
void Timer1_Init(void){
	TCCR1A |= (1<<FOC1A);
    TCCR1B |= (1 << WGM12) | (1 << CS12) | (1<<CS10);  // CTC Mode, Prescaler 1024
    TIMSK |= (1 << OCIE1A); // Enable Compare Match Interrupt
    OCR1A =  15624; // 1-second interval
}

// External Interrupt 0 (Reset Timer to 00:00:00)
void INT0_Init(void){
	MCUCR |= (1<<ISC01); // Falling Edge Trigger
	GICR |= (1<<INT0); // Enable External Interrupt 0
}

// External Interrupt 1 (Pause Timer)
void INT1_Init(void){
	MCUCR |= (1<<ISC10) | (1<<ISC11); // Rising Edge Trigger
	GICR |= (1<<INT1); // Enable External Interrupt 1
}

// External Interrupt 2 (Resume Timer)
void INT2_Init(void){
	MCUCSR &= ~(1<<ISC2); // Falling Edge Trigger
	GICR |= (1<<INT2); // Enable External Interrupt 2
}

// ISR for Timer1 Compare Match
ISR(TIMER1_COMPA_vect){
	if(mode == 1){
		count_up();
	}
	else{
		count_down();
	}
}

// ISR for Reset Button (INT0)
ISR(INT0_vect){
	s=m=h=0;
}

// ISR for Pause Button (INT1)
ISR(INT1_vect){
	PORTD &= ~(1<<PD0); // Deactivate buzzer
	TCCR1B &= ~(1 << CS12) & ~(1 << CS10); // Disable Timer
}

// ISR for Resume Button (INT2)
ISR(INT2_vect){
	TCCR1B |= (1 << CS12) | (1 << CS10); // Enable Timer
}

// Function to increment time (Count Up Mode)
void count_up(void){
	 PORTD &= ~(1<<PD0); // Deactivate buzzer
	 s++;
	 if (s >= 60){
		 s = 0; m++;
	 }
	 if (m >= 60){
		 m = 0; h++;
	 }
	 if (h >= 24){
		 h = 0; // Reset after 24 hours
	 }
}

// Function to decrement time (Count Down Mode)
void count_down(void){
	if (s == 0 && m == 0 && h == 0) {
	    TCCR1B &= ~((1 << CS12) | (1 << CS10)); // Stop timer
	    PORTD |= (1<<PD0); // Activate buzzer
	}
	else{
		PORTD &= ~(1<<PD0); // Deactivate buzzer
		if(s == 0){
			s = 59;
			if(m == 0){
				m = 59;
				if(h > 0) h--;
			}
			else{
				m--;
			}
		}
		else{
			s--;
		}
	}
}

// Configure I/O pins
void PIN_DIRECTIONS(void){
	DDRA |= 0x3F;  // First 6 PINs as output for 7-segment selection
	PORTA &= ~0x3F;

	DDRB = 0; // All Input
	PORTB = 0xFF; // Internal Pull-Up

	DDRC = 0x0F;  // First 4 PINs as output for decoder
	PORTC &= 0xF0;

	DDRD = (DDRD & ~0x3C) | 0x31;   // Configure PD2, PD3, PD4, PD5
	PORTD |= (1<<PD2) | (1<<PD4);
	PORTD &= ~(1<<PD5);
}

// Display time on 7-segment display
void DISPLAY(void){
	PORTC = (PORTC & 0xF0) | (s % 10);
	PORTA |= 0x20; _delay_ms(5); PORTA &= ~0x20;

	PORTC = (PORTC & 0xF0) | (s / 10);
	PORTA |= 0x10; _delay_ms(5); PORTA &= ~0x10;

	PORTC = (PORTC & 0xF0) | (m % 10);
	PORTA |= 0x08; _delay_ms(5); PORTA &= ~0x08;

	PORTC = (PORTC & 0xF0) | (m / 10);
	PORTA |= 0x04; _delay_ms(5); PORTA &= ~0x04;

	PORTC = (PORTC & 0xF0) | (h % 10);
	PORTA |= 0x02; _delay_ms(5); PORTA &= ~0x02;

	PORTC = (PORTC & 0xF0) | (h / 10);
	PORTA |= 0x01; _delay_ms(5);  PORTA &= ~0x01;
}

// Toggle mode between Count Up and Count Down
void MODE(void){
	if(!(PINB & (1 << PB7))) { // Check if button is pressed (active low)
		_delay_ms(1);
		if(!(PINB & (1 << PB7))) { // Ensure it's still pressed
			PORTD ^= (1<<PD5) ^ (1<<PD4);
			mode ^= 1; // Toggle between Count Up and Count Down
			while(!(PINB & (1 << PB7))); // Wait until button is released
			_delay_ms(1);
		}
	}
}

// Adjust time when paused
void ADJUST_TIME(void) {
	if (!(TCCR1B & (1 << CS12)) && !(TCCR1B & (1 << CS10))) {
	        if (!(PINB & (1 << PB0))) {
	    		_delay_ms(1);
	        	if(!(PINB & (1 << PB0))){
	        		 h = (h == 0) ? 23 : h - 1;
	        		 while(!(PINB & (1 << PB0)));
	 	    		_delay_ms(1);
	        	}
	        }
	        if (!(PINB & (1 << PB1))) {
	        	_delay_ms(1);
	        	 if (!(PINB & (1 << PB1))){
	        		 h = (h + 1) % 24;
	        		 while(!(PINB & (1 << PB1)));
	        		 _delay_ms(1);
	        	 }
	        }
	        if (!(PINB & (1 << PB3))) {
	        	_delay_ms(1);
	        	if (!(PINB & (1 << PB3))){
	        		m = (m == 0) ? 59 : m - 1;
	        		while(!(PINB & (1 << PB3)));
		        	_delay_ms(1);
	        	}
	        }
	        if (!(PINB & (1 << PB4))) {
	        	_delay_ms(1);
	        	if (!(PINB & (1 << PB4))){
	        		   m = (m + 1) % 60;
	        		   while(!(PINB & (1 << PB4)));
			        	_delay_ms(1);
	        	}
	        }
	        if (!(PINB & (1 << PB5))) {
	        	_delay_ms(1);
	        	if (!(PINB & (1 << PB5))){
		            s = (s == 0) ? 59 : s - 1;
	        		while(!(PINB & (1 << PB5)));
		        	_delay_ms(1);
	        	 }
	        }
	        if (!(PINB & (1 << PB6))) {
	        	_delay_ms(1);
	        	if (!(PINB & (1 << PB6))){
		            s = (s + 1) % 60;
	        		while(!(PINB & (1 << PB6)));
		        	_delay_ms(1);
	        	}
	        }
	    }
}
