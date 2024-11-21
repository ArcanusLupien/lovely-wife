#include "Definitions.h"
#include "Class_TestPlatform.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <Wire.h>

I2C_Display1 Display1;
I2C_Display2 Display2;
I2C_Display3 Display3;
I2C_Display4 Display4;
I2C_Throttle I2C_Throttle;

TestPlatform testbed( LiquidCrystal(LCD_RS,LCD_Enable,LCD_D4,LCD_D5,LCD_D6,LCD_D7));



void setup() {
  // Pin Set-up
    DDRB = 0xFF; // outputs
    PORTB = 0x30; // low, PB4, PB5, high to break relay

    DDRC = 0x00; // inputs
    PORTC = 0x30; // pins C4, C5 are SDA/SCL, pull-up enabled

    DDRD = 0xFF; // outputs
    PORTD = 0x00; //low

  // Wire Setup
    Wire.begin(Display_I2C_Address);
    Wire.onReceive(receiveEvent);
  // LCD Setup
    testbed.begin(16,2);
  // ADC Setup
    ADMUX = 0x60; // 0b 0110 0000
    // AVcc reference, left adjusted, ADC0 read
    ADCSRA = 0x8C; // 0b 1000 1100
    // Enabled, not auto, interupt enabled, 1/16 clock
    ADCSRA |= (1<<ADSC);

  // Start Up Procedure
      // Confirm motor uC
      // Confirm AS5600
      // Check Power
        // USB
        // Batt
      // Connect motor
    testbed.Screen_Bootup();
    delay(100);
    testbed.Screen_MotoruC();
    testbed.ScreenA_DISX();
    Wire.beginTransmission(Motor_I2C_Address);
    delay(1);
    while(Wire.endTransmission() == 1);
    testbed.ScreenA_CONK();
    delay(1000);

    testbed.Screen_MotoruC();
    testbed.ScreenA_DISX();
    Wire.beginTransmission(AS5600_I2C_Address);
    delay(1);
    while(Wire.endTransmission() == 1);
    testbed.ScreenA_CONK();
    delay(1000);

    testbed.Screen_Power();
    delay(1);
    if (testbed.print_Voltage() > 205){
      testbed.ScreenA_BATT();
    }
    else {
      testbed.ScreenA_USB();
    }
    delay(1000);

    testbed.Screen_Motor();
    if (testbed.print_Voltage() > 205){
      PORTB = 0x20;
      testbed.ScreenA_CONK();
    }
    else {
      PORTB = 0x30;
      testbed.ScreenA_DISX();
    }
    delay(1000);

  testbed.DisplayScreen1();

}

void loop() {

  testbed.UpdateDisplayScreen1(Display1);

}
// Functions
ISR(ADC_vect){
  //supports reading throttle (8-bit), voltage (10-bit)
  //switch function for choosing the source
  //function increases by one, when overflows, goes back to one
  //triggers the ADC to run again
  switch( ADMUX & 0x0F ){
    case 0: //throttle
      testbed.set_Throttle( ADCH );
      break;
    case 1: //voltage
      testbed.set_Voltage( ADC>>6);
      break;
    default:
      break;
  }
  ADMUX++; // switches ADC to next source
  if (ADMUX & 0x0F == 2){
    ADMUX &= 0xF0;
  }
  ADCSRA |= (1<<ADSC);
}

void receiveEvent(int howMany){
  //Read ID code from incoming Info
  uint8_t j = Wire.read();
  switch (j) {
  case 0:
    //error case. there is no ID
    break;
  case 1:
    Display1.Phase = Wire.read();
    Display1.Mode = Wire.read();
    Display1.Pole = Wire.read();
    Display1.Stator = Wire.read();
    break;
  }

}
