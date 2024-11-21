#include "Definitions.h"
#include "Class_Motor.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <Wire.h>


I2C_Display1 Display1;
I2C_Display2 Display2;
I2C_Throttle I2C_Throttle;

Motor motor(pole_count,stator_count,Motor_mode_Trap);

void setup() {
  //Serial Setup
  // PORT Set-ups
    DDRB = 0xFF; // all pins are output
    PORTB = 0x00; // all pins are low

    DDRC = 0x00;
    PORTC = 0x30; // pins C4, C5 are SDA/SCL, pull-up enabled
    
    DDRD = 0xFF; // all pins are output
    PORTD = 0x00 ; // all pins are low

  // I2C Set-up
    Wire.begin(Motor_I2C_Address);

  // AS5600 Setup
    motor.as5600.begin();
    motor.as5600.setDirection(0); // defines clockwise rotation
    while(!Serial.println(motor.as5600.isConnected()));

  // ADC Setup
    ADMUX = 0x60; // 0b 0110 0000
    // AVcc reference, left adjusted, ADC0 read
    ADCSRA = 0x8C; // 0b 1000 1100
    // Enabled, not auto, interupt enabled, 1/16 clock

  // TIMER Set ups
    // Fast mode PWM at 1/8 clk
    //TIMER0
      TCCR0A = 0x03; //0b 0000 0011
        //non-inverting pins, fast pwm w/ 0xFF top
      TCCR0B = 0x02; //0b 0000 0010
        // fast pwm w/ 0xff top, 1/8 clk
      TIMSK0 = 0x00; //0b 0000 0000
        // no interupts needed
    //TIMER1
      TCCR1A = 0x01; //0b 0000 0001
        //non-inverting pins, fast pwm w/ 0xFF top
      TCCR1B = 0x0A; //0b 0000 1010
        // fast pwm w/ 0xff top, 1/8 clk
      TIMSK1 = 0x00; //0b 0000 0000
        //no interupts needed
    //TIMER2
      TCCR2A = 0x03; //0b 0000 0011
        //non-inverting pins, fast pwm w/ 0xFF top
      TCCR2B = 0x02; //0b 0000 0010
        // fast pwm w/ 0xff top, 1/8 clk
      TIMSK2 = 0x00; //0b 0000 0000
        //no interupts needed


  // TIMER Triggers set up
    // High Sides
      OCR0A = 0x00;

      OCR1AL = 0x00;
      OCR1AH = 0x00;

      OCR2A = 0x00;
    // Low Sides

      OCR0B = 0xFF;
      
      OCR1BL = 0xFF;
      OCR1BH = 0x00;

      OCR2B = 0xFF;

  // Check-ins

  sei();

  motor.Commutation_Startup();
  delay(100000);
  motor.as5600.setOffset(motor.as5600.readAngle());

  motor.Commutation_Erase();
}



void loop() {
  ADCSRA |= (1<<ADSC);
  motor.updatePhase();

  if (phase == 3){
    motor.ThreePhaseCommutation();
  }
  else if (phase == 1){
    motor.OnePhaseCommutation();
  }

  motor.Print_To_Display1(Display1);
  motor.Print_To_Display2(Display2);
}


ISR(ADC_vect){
  //supports reading throttle (8-bit), voltage (10-bit) and current (10-bit)
  //switch function for choosing the source
  //function increases by one, when overflows, goes back to one
  //triggers the ADC to run again
  switch( ADMUX & 0x0F ){
    case 0: //throttle
      motor.set_Throttle( ADCH );
      if (motor.printMode() == Motor_mode_Trap || motor.printMode() == Motor_mode_Blind){
        OCR0A = OCR1A = OCR2A = ADCH;
      }
      break;
    case 1: //voltage
      motor.set_Voltage( ADC>>6 );
      break;
    case 2: //current
      motor.set_Current( ADC>>6 );
      break;
    default:
      break;
  }
  ADMUX++; // switches ADC to next source
  if (ADMUX & 0x0F == 3){
    ADMUX &= 0xF0;
  }
  ADCSRA |= (1<<ADSC);
}
