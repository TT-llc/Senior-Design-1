/*
 * SeniorDesign.c
 *
 * Created: 9/25/2016 9:17:25 PM
 * Author : avile
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


int main(void)

{
	DDRC = 0x00 //input for uv sensor
	DDRE = 0x00 //INPUT FOR TIMER
	DDRD = 0xFF //Outputs for good drinking water
	DDRB = 0xFF //output for bad drinking water
	sei();// enable global interrupt
	while(1){

	}

}

int default = 1000; // default timer value
int Timer = 0; //dummy variables for timer
double movAverage = 20; // dummy variable for average


//This interrupt will handle the global reset
ISR(PCINT2_vect,ISR_BLOCK) {
Timer = default;
movAverage = 0;
	return; 
}

int uvValue = 0;


//Handle the timer and moving average 
//For the dummy i will use a 4 sec delay as reading average
ISR(PCINT0_vect, ISR_BLOCK)
{
	//checks to see if 4 minutes have passed
	if(Timer == 4){ // can be changed depending on equation or alg. Thinking of using if timer % 4 == 0
		movAverage = uvValue;
	}
		
	//using 100 and 50 as a dummy number for acceptable range
if(Timer == 0){
	movAverage = movAverage/4; //AVERAGES MOVAVERAGE
	if(((movAverage <= 100) && movAverage >= 50){
		Timer = default; // sets defaults
	movAverage= 0; //sets reset
					//drink water
					// essentially turning on a green led
					PORTD = 0xFF;
				
					return;
											}
		else{
			Timer = default; // sets defaults
			movAverage= 0; //sets reset
			//dont drink water
			//essential turns on a red dont drink led
			PORTB=0xFF;
			return;
		}

}

	
}
