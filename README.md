# lovely-wife
ESC Controller Software. Includes Master Controller, ESC and computer interface
Erik Eldridge

power:
  12-15 V
  30 A


Purpose:
To control a motor and measure propeller effeciency

motor control:
  This is done by using the 6 pwm signals created by AVR timers
  each timer value is set based on either a sine wave or a trap wave
  motor position is read from a AS5600 module attached to the motor

measurements:
  AVR ADC measures supply voltage, modulated current and throttle
  Voltage is obtained by a voltage divider
  current is measured by an op amp output
  throttle is a direct measure from a poteinometer

Communication
  there is a TestPlatform uC (display / flight controller) over the Motor uC
  Motor uC is master over AS5600 and load cell
  all calculations are run inside the Motor uC for measuremets
  Data is transmitted to the display/flight controller
    specific packets are requested in a master write
    the order is fulfilled and sent back
    Throttle is given to the Motor uC 


