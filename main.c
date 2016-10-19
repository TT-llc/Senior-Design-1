/*
 * SeniorDesign.c
 *
 * Created: 9/25/2016 9:17:25 PM
 * Author : aviles
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

int main(void)

{
	 
	EIMSK |= (1 << INT0); // enables interrupt 1 and 0
	PCICR |= (1 << PCIE0); //Pin change interrupt control register is sent to one
	PCMSK0 |= (1<<PCINT0);
	
	//SPCR |= (1 << SPE)|(1<<MSTR); // enables spi interface and master
	
	//TWCR |= (1<<TWEN); //Enables i2c mode
	DDRF = 0xFF; //Port used to count how many sodis cycles have occurred
	DDRE = 0xFF; //PORT USED FOR AVG
	DDRB = 0x00; //input for uv sensor
	DDRD = 0x00;//INPUT FOR TIMER
	DDRC = 0xFF; //Outputs for  drinking water
	TIMSK0 = ((1<<OCIE0A) || (1<<TOIE0));
	TCCR0A = 0x02;//Again dummy value but his will control what mode and prescaler we are using
	sei();// enable global interrupt
	while(1){
}

}
int Turbidiy = 0; // This will be the light range read in from the UV sensor for Turbidity

int IdealTurb = 2; //Dummy value for ideal turb level
//Ideal Turbidity needs to be researched

int count; // used to count how many iterations are done

int def = 1000; // default timer value dummy variable
int Timer = 0; //dummy variables for timer
int cycleCount = 0; //This variable will count the number of sodis cycles
double movAverage = 20; // dummy variable for average


//This interrupt will handle the global reset 
ISR(PCINT0_vect, ISR_BLOCK) {
Timer = def;
PORTE = 0; // can use port as a global dynamic variable
count = 0;  // can use port as a global dynamic variable
PORTF = 0x00; // can use port as a global dynamic variable
PORTC = 0x00;
TCNT0 = 160; //This is a test value till we decide what clock we are using

	return; 
}

int uvValue = 0;

//Handle the timer and moving average 
//For the dummy i will use a 4 sec delay as reading average
ISR(INT0_vect,ISR_BLOCK)
{
	
	//using 100 and 50 as a dummy number for acceptable range
	PORTE = PORTE/count; //AVERAGES MOVAVERAGE

if(((movAverage <= 100) && movAverage >= 50) && Turbidiy<IdealTurb){
		Timer = def; // sets defaults
		PORTE= 0; //sets reset
		count = 0;
					//drink water
					// essentially turning on a green led
					PORTC = 0x0F;
					return;
											}
		
		else{
				Timer = def; // sets defaults
				PORTE= 0; //sets reset FOR MOVE AVERAGE
				count = 0;
				//dont drink water
				//essential turns on a red dont drink led
				PORTC=0xF0;
				PORTF= PORTF+1;
	if(PORTF == 5){
		//TELL USER TO RUN WATER THROUGH FITLERS AGAIN and to hit global reset
	    PORTC=0xFF; //All lights are on
			    }
			return;
		}


}

void T0Delay(){
	while((TIFR &(1<<TOV0)==0)); //WAIT FOR DELAY TO FINISH
	
	TCCR0A=0; //turn off timer0
	TIFR = 0x1;//CLEAR TOV0
}

ISR(TIMER0_COMPA_vect){
//tHIS CAN BE REPLACED USING BUILT IN DELAYS IE DELAY(29) OR WHAT EVER THE CODE IS
//checks to see if 4 minutes have passed

if(Timer % 4 == 0 && (Timer !=0)){ // can be changed depending on equation or alg. Thinking of using if timer % 4 == 0
	//PORTE += SPDR; // spdr IS THE SPI DATA REG
	count++;
}	
}


	

