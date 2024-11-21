#ifndef Class_Test_Platform_H
#define Class_Test_Platform_H

#include <LiquidCrystal.h>
#include <String.h>

#include "I2C_Communication.h"

class TestPlatform {
  uint8_t throttle, voltage;
  LiquidCrystal lcd;

  void FormatString( int i)  {
    if (x.length() > i) {
      x.remove(i,x.length()-i);
    }
    for ( int a = 0; x.length() < i; a++){
      x = "0"+x;            
    }
  }
  String x;

public:
  TestPlatform(LiquidCrystal a) { lcd = a;};

  void begin(uint8_t a, uint8_t b) {lcd.begin(a,b);};

  void set_Throttle(uint8_t a){throttle = a;};
  void set_Voltage(uint8_t a){voltage = a;};
  uint8_t print_Voltage(){return voltage;};
// Screen 1 -------------------------------
  void DisplayScreen1(){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.write("PHASE:   STAT:  ");
    lcd.setCursor(0,1);
    lcd.write("MODE:    POLE:  ");
  }

  void UpdateDisplayScreen1(I2C_Display1 A){
    lcd.setCursor(7,0);
    lcd.write(A.Phase);
    lcd.setCursor(15,0);
    lcd.write(A.Stator);
    lcd.setCursor(5, 1);
    switch (A.Mode){
      case 0:
        lcd.write("BLND");
        break;
      case 1:
        lcd.write("TRAP");
        break;
      case 2:
        lcd.write("SINE");
        break;
    }
    lcd.setCursor(15,1);
    lcd.write(A.Pole);
    
  }

// Screen 2 -------------------------------
  void DisplayScreen2(){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.write("ANG:    OFF:");
    lcd.setCursor(0,1);
    lcd.write("PHS:    CHG:");
  }

// Screen 3 -------------------------------
  void DisplayScreen3() {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.write("THR:   %CRT:");
    lcd.setCursor(0,1);
    lcd.write("SPD:    VLT:");
  }

  void UpdateScreen3(I2C_Display3 A) {
    //Throttle Update
      lcd.setCursor(6,0);
      x = throttle*throttle_per_bit;
      FormatString(2);
      lcd.print(x);
    //Current Update
      lcd.setCursor(12,0);
      x = A.current*MOTOR_amp_per_bit/1000;
      lcd.print(x);
    //Speed update
      lcd.setCursor(4,1);
      x = A.rpm; //returns in RPM
      FormatString(4);
      lcd.print(x);
    //Voltage Update
      lcd.setCursor(12,1);
      x = A.voltage*MOTOR_volt_per_bit/1000;
      lcd.print(x); 
  }
// Screen 4 -------------------------------
  void DisplayScreen4() {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.write("F:    N  SPD:");
    lcd.setCursor(0,1);
    lcd.write("PWR:  W EFF:");
  }

  void UpdateScreen4(I2C_Display4 A) {
    //update Force
    x = A.force;
    //update Power
    x = A.power;
  }
// Screen A - Booting Up -------------------
  void Screen_Bootup() {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("BOOTING UP...  ");
  }
  void Screen_MotoruC(){
    lcd.setCursor(0,1);
    lcd.print("Motor uC:      ");
  }
  void Screen_AS5600(){
    lcd.setCursor(0,1);
    lcd.print("AS5600  :      ");
  }
  void Screen_Power(){
    lcd.setCursor(0,1);
    lcd.print("POWER   :      ");
  }
  void Screen_Motor(){
    lcd.setCursor(0,1);
    lcd.print("MOTOR   :      ");
  }
  void ScreenA_CONK(){
    lcd.setCursor(10,1);
    lcd.print("CONK");
  }
  void ScreenA_DISX(){
    lcd.setCursor(10,1);
    lcd.print("DISX");
  }
  void ScreenA_USB(){
    lcd.setCursor(11,1);
    lcd.print("USB");   
  }
  void ScreenA_BATT(){
    lcd.setCursor(10,1);
    lcd.print("BATT");   
  }
};

#endif
