#ifndef Class_Motor_H
#define Class_Motor_H

#include <AS5600.h>
#include <math.h>
#include "I2C_Communication.h"

class Motor{ 
  uint8_t mode;
  uint8_t throttle;
  uint8_t poles, stators;
  uint16_t voltage,current;
  double i = 0b00000001;


public:
  AS5600 as5600;
  //Create
    Motor(){};
    Motor(uint8_t a, uint8_t b,uint8_t c){
      poles = a;
      stators = b;
      mode = c;
    }

  // Set Values
    void set_Throttle(uint8_t a){throttle = a;}
    void set_Throttle(I2C_Throttle A){throttle = A.throttle;}
    void set_Voltage(uint16_t a){voltage = a;}
    void set_Current(uint16_t a){current = a;}
    void set_Phase(uint8_t a){i = a;}
    void updatePhase(){
      if (mode == 0){
        i++;
        if (i >= 6) {i = 0;};
      }
      else if (mode == 1){
        i = (as5600.rawAngle() % (2*4096/poles) / (2*4096/poles/(phase*2)));
      }
      else if (mode == 2){
        i = fmod(as5600.rawAngle(),(2*4096/poles));
      }
    }

  //Printing Values
    uint8_t printThrottle(){return throttle;}
    uint16_t printVoltage(){return voltage;}
    uint16_t printCurrent(){return current;}
    uint8_t printMode(){return mode;}
    uint8_t printPhase(){return i;}
    void Print_To_Display1(I2C_Display1 A){
      A.voltage = voltage;
      A.current = current;
      A.rpm = as5600.getAngularSpeed();
    }
    void Print_To_Display2(I2C_Display2 A){
      A.force = 0;
      A.power = voltage*current;
    }



  //Motor Commutation
    // OCR0 for A
    // OCR1 for B
    // OCR2 for C

    // Triggers the correct Phases based on the i-value of the class
    // Throttle control dicates maximum and minimum speeds
    void OnePhaseCommutation(){
      /*Phasing
        A b
        B a
      */
      if (i == 0){
        TCCR1A &= ~(1<<pwm_high);
        //turn off High B
        TCCR1A |= (1<<pwm_low);
        //turn on low B
        TCCR0A &= ~(1<<pwm_low);
        //turn off low A
        TCCR0A |= (1<<pwm_high);
        //turn on high A
      }
      else if (i == 1){
        TCCR0A &= ~(1<<pwm_high);
        //turn off High A
        TCCR0A |= (1<<pwm_low);
        //turn on low A
        TCCR1A &= ~(1<<pwm_low);
        //turn off low B
        TCCR1A |= (1<<pwm_high);
        //turn on high B
      }
    }
    // Triggers the correct Phases based on the i-value of the class
    // Throttle control dicates maximum and minimum speeds
    void ThreePhaseCommutation(){
      if (mode == 0 || mode == 1){
        /* Phasing
          0: A b
            turn off all
            turn on low b
            turn on High A
          1: C b
            turn off all
            turn on low b
            turn on High C
          2: C a
            turn off all
            turn on low a
            turn on high C
          3: B a
            turn off all
            turn on low a
            turn on high B
          4: B c
            turn off all
            turn on low c
            turn on High B
          5: A c
            turn off all
            turn on low C
            turn om High A
        */
        switch((int)i){
          case 0: // A b
            Commutation_Erase(2);
            TCCR0A |= (1<<pwm_high);
            //turn on High A
            TCCR1A |= (1<<pwm_low);
            //turn on low B
            break;

          case 1: // C b
            Commutation_Erase(2);
            TCCR2A |= (1<<pwm_high);
            //turn on High C
            TCCR1A |= (1<<pwm_low);
            //turn on low b
            break;

          case 2: // C a
            Commutation_Erase(2);
            TCCR2A |= (1<<pwm_high);
            //turn on High C
            TCCR0A |= (1<<pwm_low);
            //turn on low a
            break;

          case 3: // B a
            Commutation_Erase(2);
            TCCR1A |= (1<<pwm_high);
            //turn on High B
            TCCR0A |= (1<<pwm_low);
            //turn on low a
            break;

          case 4: // B c
            Commutation_Erase(2);
            TCCR1A |= (1<<pwm_high);
            //turn on High B
            TCCR2A |= (1<<pwm_low);
            //turn on low c
            break;

          case 5: // A c
            Commutation_Erase(2);
            TCCR0A |= (1<<pwm_high);
            //turn on High A
            TCCR2A |= (1<<pwm_low);
            //turn on low c
            break;

          default:
          break;
        }
      }
      else if (mode == 2){
        i *= (6.2831) / (4096/poles);

        int16_t angle = 255*sin(i+0*1.0472);
          OCR0A = fmax(angle,0);
          OCR0B = -fmin(angle,0);

                angle = 255*sin(i+1*1.0472);
          OCR1A = fmax(angle,0);
          OCR1B = -fmin(angle,0);

                angle = 255*sin(i+2*1.0472);
          OCR2A = fmax(angle,0);
          OCR2B = -fmin(angle,0);


      }
    }
    
    // Creates a electromagnetic force to set a magnet directly above
    // Phase A's coil. While held, as5600 can read the position and use
    // as an offset.
    void Commutation_Startup(){
      OCR0A = 200;
      TCCR0A |= (1<<pwm_high);
      OCR1B = 255;
      TCCR1A |= (1<<pwm_low);
      OCR2B = 255;
      TCCR2A |= (1<<pwm_low);
    }
    // Cleans out the timers pins and their triggers
    // a = 0 ; clears out pins, closes all high sides and opens all low sides
    // a = 1 ; closes all high sides, opens all low sides
    // a = 2 ; clears all pins
    void Commutation_Erase(uint8_t a = 0){
      if (a == 0 || a == 2){
      TCCR0A &= 0x0F;
      TCCR1A &= 0x0F;
      TCCR2A &= 0x0F;
      }
      if (a==0||a==1){
      OCR0A = OCR1A = OCR2A = 0;
      OCR0B = OCR1B = OCR2B = 255;
      }
    }

};
#endif
