#include "U8glib.h"
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_NO_ACK|U8G_I2C_OPT_FAST);  // Fast I2C / TWI 

/* 
 The ML8511 UV Sensor outputs an analog signal in relation to the amount of UV light it detects.
 
 This sensor detects 280-390nm light most effectively. This is categorized as part of the UVB (burning rays)
 spectrum and most of the UVA (tanning rays) spectrum.
 
 There's lots of good UV radiation reading out there:
 http://www.ccohs.ca/oshanswers/phys_agents/ultravioletradiation.html
 https://www.iuva.org/uv-faqs
*/

//Hardware pin definitions
int UVOUT = A2; //Output from the sensor
int REF_3V3 = A1; //3.3V power on the Arduino board
int LDROUT = A3;

//other global variables
char charBuff[50];

float outputVoltage = 0.0;
int uvLevel = 0;
int refLevel = 0;
float uvIntensity = 0.0;

void drawUV()
{
  uvLevel = averageAnalogRead(UVOUT);
  refLevel = averageAnalogRead(REF_3V3);
  
  //Use the 3.3V power pin as a reference to get a very accurate output value from sensor
  outputVoltage = 3.3 / refLevel * uvLevel;
  
  uvIntensity = mapfloat(outputVoltage, 0.99, 2.8, 0.0, 15.0); //Convert the voltage to a UV intensity level
  // graphic commands to redraw the complete screen should be placed here  
  
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  
  uvIntensity = mapfloat(outputVoltage, 0.99, 2.8, 0.0, 15.0); //Convert the voltage to a UV intensity level

  //Draw string
  String message = "UV: ";
  String senseString = String(uvIntensity, 2);
  String unit = " mW/cm^2";

  String measure = message+ senseString + unit;
  measure.toCharArray(charBuff,50);
  u8g.drawStr( 0, 22, charBuff);
}

void drawLDR()
{
 // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);

  int LDRmeasure = ldrMeasure();
  String senseString = String(LDRmeasure,DEC);
  senseString.toCharArray(charBuff,50);
  u8g.drawStr( 0, 50, charBuff); 
}
//Takes an average of readings on a given pin
//Returns the average
int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0; 

  for(int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;

  return(runningValue);  
}

void draw(void) 
{
  drawUV();
  drawLDR(); 
}

void setup()
{
 cli(); //disable global interrupts
  TCCR1A = 0; //set TCCR1A register to 0
  TCCR1B = 0;

  //set compare register
 
  OCR1A = 0xFFFF; //max time between readings ~4s
  OCR1B = 0x00;
  
  //turn on CTC mode
  TCCR1B |= (1 << WGM12);
  
  //set prescaler of 1024
  TCCR1B |= (1 << CS10); 
  TCCR1B |= (1 << CS12);
  
  //enable timer1 overflow
  TIMSK1 = (1 << OCIE1A);

  //enable interrupts
  sei();

 pinMode(UVOUT, INPUT);
 pinMode(REF_3V3, INPUT);

#if defined(ARDUINO)
  pinMode(13, OUTPUT);           
  digitalWrite(13, HIGH);  
#endif

}

void loop()
{

   // picture loop  
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );

}


//The Arduino Map function but for floats
//From: http://forum.arduino.cc/index.php?topic=3922.0
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/*********************************************************************************
 ******************ISR calls temperature function to update LDRreadout***********
 *********************************************************************************/
ISR(TIMER1_COMPA_vect)
{
  ldrMeasure();
}

//takes temperature reading using tmp36
int ldrMeasure()
{
 int reading = averageAnalogRead(analogRead(LDROUT)); //take voltage reading
 
 return reading;
}//end

