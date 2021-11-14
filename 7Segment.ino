/*
 * Interfacing Arduino with LM35 temperature sensor and 7-segment display.
 * Common anode 7-segment display is used.
 * This is a free software with NO WARRANTY.
 * https://simple-circuit.com/
 */

// define LM35 data pin connection
unsigned long prevByteMillis = 0;
unsigned long curMillis;

#include <gyro.h>
#define  LM35_pin  A0
GYRO hdt(ALL);

boolean stat_data= false;
float heading;
// segment pins definitions
#define SegA   13
#define SegB   12
#define SegC   11
#define SegD    10
#define SegE    9
#define SegF    8
#define SegG    7
#define SegDP   6
// common pins of the three digits definitions
#define Dig0    5
#define Dig1    4
#define Dig2    3
#define Dig3    2

// variable declarations
byte current_digit;
int  temp;
float d_hdt;
String data;


void setup()
{
  Serial.begin(4800);
  pinMode(SegA, OUTPUT);
  pinMode(SegB, OUTPUT);
  pinMode(SegC, OUTPUT);
  pinMode(SegD, OUTPUT);
  pinMode(SegE, OUTPUT);
  pinMode(SegF, OUTPUT);
  pinMode(SegG, OUTPUT);
  pinMode(SegDP, OUTPUT);
  pinMode(Dig0, OUTPUT);
  pinMode(Dig1, OUTPUT);
  pinMode(Dig2, OUTPUT);
  pinMode(Dig3, OUTPUT);


  disp_off();  // turn off the display

  // Timer1 module overflow interrupt configuration
  TCCR1A = 0;
  TCCR1B = 1;  // enable Timer1 with prescaler = 1 ( 16 ticks each 1 µs)
  TCNT1  = 0;  // set Timer1 preload value to 0 (reset)
  TIMSK1 = 1;  // enable Timer1 overflow interrupt

  analogReference(INTERNAL);  // set positive reference voltage to 1.1V
}

ISR(TIMER1_OVF_vect)   // Timer1 interrupt service routine (ISR)
{
  disp_off();  // turn off the display

  switch (current_digit)
  {
    case 1:
        disp((temp / 1000) % 10);
        digitalWrite(Dig0, LOW);  // turn on digit 1
      break;

    case 2:
      disp( (temp / 100) % 10);   // prepare to display digit 2
      //digitalWrite(SegDP, LOW);  // print decimal point ( . )
      digitalWrite(Dig1, LOW);   // turn on digit 2
      break;

    case 3:
      disp(temp/10 % 10);   // prepare to display digit 3
      digitalWrite(SegDP, HIGH);  // print decimal point ( . )
      digitalWrite(Dig2, LOW);    // turn on digit 3
      break;
    case 4:
      disp(temp % 10);   // prepare to display digit 3
      //digitalWrite(SegDP, LOW);
      digitalWrite(Dig3, LOW);    // turn on digit 3
  }

  current_digit = (current_digit % 4)+1;
}

// main loop
void loop()
{
  curMillis = millis();
   
   
  if(Serial.available()>0) {
          hdt.decode(Serial.read());
          data= hdt.term(1);
          d_hdt = data.toFloat();
          temp = d_hdt * 10;
          data ="";
          prevByteMillis = curMillis; 
          
      
  }
   if (curMillis - prevByteMillis >=20000) {
   temp=0;
   }
      
}

void disp(byte number)
{
  switch (number)
  {
    case 0:  // print 0
      digitalWrite(SegA, HIGH);
      digitalWrite(SegB, HIGH);
      digitalWrite(SegC, HIGH);
      digitalWrite(SegD, HIGH);
      digitalWrite(SegE, HIGH);
      digitalWrite(SegF, HIGH);
      digitalWrite(SegG, LOW);
      digitalWrite(SegDP, LOW);
      break;

    case 1:  // print 1
      digitalWrite(SegA, LOW);
      digitalWrite(SegB, HIGH);
      digitalWrite(SegC, HIGH);
      digitalWrite(SegD, LOW);
      digitalWrite(SegE, LOW);
      digitalWrite(SegF, LOW);
      digitalWrite(SegG, LOW);
      digitalWrite(SegDP, LOW);
      break;

    case 2:  // print 2
      digitalWrite(SegA, HIGH);
      digitalWrite(SegB, HIGH);
      digitalWrite(SegC, LOW);
      digitalWrite(SegD, HIGH);
      digitalWrite(SegE, HIGH);
      digitalWrite(SegF, LOW);
      digitalWrite(SegG, HIGH);
      digitalWrite(SegDP,LOW);
      break;

    case 3:  // print 3
      digitalWrite(SegA, HIGH);
      digitalWrite(SegB, HIGH);
      digitalWrite(SegC, HIGH);
      digitalWrite(SegD, HIGH);
      digitalWrite(SegE, LOW);
      digitalWrite(SegF, LOW);
      digitalWrite(SegG, HIGH);
      digitalWrite(SegDP,LOW);
      break;

    case 4:  // print 4
      digitalWrite(SegA, LOW);
      digitalWrite(SegB, HIGH);
      digitalWrite(SegC, HIGH);
      digitalWrite(SegD, LOW);
      digitalWrite(SegE, LOW);
      digitalWrite(SegF, HIGH);
      digitalWrite(SegG, HIGH);
      digitalWrite(SegDP, LOW);
      break;

    case 5:  // print 5
      digitalWrite(SegA, HIGH);
      digitalWrite(SegB, LOW);
      digitalWrite(SegC, HIGH);
      digitalWrite(SegD, HIGH);
      digitalWrite(SegE, LOW);
      digitalWrite(SegF, HIGH);
      digitalWrite(SegG, HIGH);
      digitalWrite(SegDP, LOW);
      break;

    case 6:  // print 6
      digitalWrite(SegA, HIGH);
      digitalWrite(SegB, LOW);
      digitalWrite(SegC, HIGH);
      digitalWrite(SegD, HIGH);
      digitalWrite(SegE, HIGH);
      digitalWrite(SegF, HIGH);
      digitalWrite(SegG, HIGH);
      digitalWrite(SegDP, LOW);
      break;
    
    case 7:  // print 7
      digitalWrite(SegA, HIGH);
      digitalWrite(SegB, HIGH);
      digitalWrite(SegC, HIGH);
      digitalWrite(SegD, LOW);
      digitalWrite(SegE, LOW);
      digitalWrite(SegF, LOW);
      digitalWrite(SegG, LOW);
      digitalWrite(SegDP, LOW);
      break;

    case 8:  // print 8
      digitalWrite(SegA, HIGH);
      digitalWrite(SegB, HIGH);
      digitalWrite(SegC, HIGH);
      digitalWrite(SegD, HIGH);
      digitalWrite(SegE, HIGH);
      digitalWrite(SegF, HIGH);
      digitalWrite(SegG, HIGH);
      digitalWrite(SegDP, LOW);
      break;

    case 9:  // print 9
      digitalWrite(SegA, HIGH);
      digitalWrite(SegB, HIGH);
      digitalWrite(SegC, HIGH);
      digitalWrite(SegD, HIGH);
      digitalWrite(SegE, LOW);
      digitalWrite(SegF, HIGH);
      digitalWrite(SegG, HIGH);
      digitalWrite(SegDP, LOW);
      break;
    
  }
}

void disp_off()
{
   digitalWrite(Dig0, HIGH);
   digitalWrite(Dig1, HIGH);
   digitalWrite(Dig2, HIGH);
   digitalWrite(Dig3, HIGH);
}



// end of code.
