#include "Definitions.h"
#include "I2C_Communication_Methods.h"


#include <avr/io.h>
#include <avr/interrupt.h>
#include <Wire.h>

I2C_Display1 Display1;

void setup() {
  //Pin Set-up
    DDRC = 0x00; // inputs
    PORTC = 0x30; // pins C4, C5 are SDA/SCL, pull-up enabled
  //Wire Setup
    Wire.begin(Master_I2C_Address);

  // Display Set-up
    Display1.Phase = phase;
    Display1.Mode = Trap;
    Display1.Pole = pole_count;
    Display1.Stator = stator_count;
}

void loop() {

  Send_Display1(Display_I2C_Address);
  delay(500);

}

void Send_Display1(uint8_t address){
  Wire.beginTransmission(address);
  Wire.write(Display1.ID);
  Wire.write(Display1.Phase);
  Wire.write(Display1.Mode);
  Wire.write(Display1.Pole);
  Wire.write(Display1.Stator);
  Wire.endTransmission();
}
