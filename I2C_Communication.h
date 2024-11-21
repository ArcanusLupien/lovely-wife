#ifndef I2C_Communication_Methods_H
#define I2C_Communicaiton_Methods_H
//Three possible codes for data going to motor uC
//  1: Request for Display1 data
//  2: Request for Display2 data
//  3: Throttle

/* Process of I2C

Master sends out 3 bytes
  0     : Code for transmission
  1, 2  : Blank on master_request, contains uint16_t space for data

Slave reads 1st byte and interprets
  0   : requests Display1
  1   : requests Display2
  10  : providing throttle

Slave responds
  0   : provides Display1
  1   : provides Display2
  10  : N/A
*/

//Request #0
struct I2C_Display1{
  // Data - 7 bytes
  uint8_t ID = 0;
  uint16_t current;
  uint16_t voltage;
  uint16_t rpm;
};

//Request #1
struct I2C_Display2{
  // Data - 3 bytes
  uint8_t ID = 1;
  uint8_t force;
  uint8_t power;
};

//Request #10
struct I2C_Throttle{
  // Data - 2 bytes
  uint8_t ID = 10;
  uint8_t throttle;
};
#endif
