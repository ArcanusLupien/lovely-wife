#ifndef Definitions_H
#define Definitions_H
// Settings

#define phase 3 // either 1-phase or 3-phase

#define Display_I2C_Address 7  //  7
#define Motor_I2C_Address 8    //  8
#define AS5600_I2C_Address 54   // 54

// Motor Class Parameters

#define pole_count 10 // individual magnets
#define stator_count 9 // coils

#define throttle_per_bit 0.39
    // 5 V -> 100%
    // 100% / 255 bit = 0.39 %/bit

#define volt_per_bit 14.95
    // divider: 6800 + 3300 = 10100
    // 3300 / 10100 = 0.326
    // 5 V -> 15.3 V
    // 15.3 V / 1023 bit = 14.95 mV/bit

#define amp_per_bit 39.3
    // 5 V -> 30 A
    // 30 A / 1023 bit = 29.3 mA/bit

// TestPlatform Class Parameters

// Helpful Shortcuts

#define pwm_high 7
#define pwm_low 5

#define Motor_mode_Blind 0
#define Motor_mode_Trap 1
#define Motor_mode_Sine 2

// Controller Pins

// Test Platform Pins

// Motor Pins

#define PWM_Pin_A_high 6  //PD6 // OC0A
#define PWM_Pin_B_high 9  //PB1 // OC1A
#define PWM_Pin_C_high 11 //PB3 // OC2A

#define PWM_Pin_A_low 5  //PD5 // OC0B
#define PWM_Pin_B_low 10 //PB2 // OC1B
#define PWM_Pin_C_low 3  //PD3 // OC2B


#define ADC_Pin_1 A0 // PC0 // ADC0  // throttle
#define ADC_Pin_2 A1 // PC1 // ADC1  // voltage
#define ADC_Pin_3 A2 // PC2 // ADC2  // current

#define SDA A4 // PC4 // SDA
#define SCL A5 //PC5 // SCL

#endif
