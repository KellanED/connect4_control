/** @file defines.h
*
* @brief Contains system level defines.
*/

#ifndef DEFINES_H
#define DEFINES_H

#define CS_MCLK_DESIRED_FREQUENCY_IN_KHZ 16000   // Frequency for MCLK in kHz
#define CS_MCLK_FLLREF_RATIO             488     // MCLK/FLLRef ratio 16k/32786

// Stepper PWM Output using Timer0_A3
#define TIMER_PERIOD                     62499   // 62500/250000 = 0.25, 4Hz
#define TIMER_DUTY_CYCLE                 31249   // 50% duty cycle
#define TIMER_PWM_PORT                   GPIO_PORT_P1
#define TIMER_PWM_PIN                    GPIO_PIN1
#define TIMER_PWM_PIN_FUNCTION           GPIO_SECONDARY_MODULE_FUNCTION

#define DIR_PORT                         GPIO_PORT_P1
#define DIR_PIN                          GPIO_PIN0
#define DIR_PORT_OUT                     P1OUT

// Servo PWM Output using Timer1_A3
#define SERVO_TIMER_PERIOD               4999    // 5000/250000 = 0.02, 50Hz
#define SERVO_MIN_DUTY                   124      // 225/250000 = 900us
#define SERVO_MAX_DUTY                   574     // 525/250000 = 2100us
#define SERVO_PORT                       GPIO_PORT_P1
#define SERVO_PIN                        GPIO_PIN4
#define SERVO_PIN_FUNCTION               GPIO_SECONDARY_MODULE_FUNCTION

#define BUMP_PORT                        GPIO_PORT_P3
#define BUMP_PIN                         GPIO_PIN2

#define UART_RX_PORT                     P2      // UCA1
#define UART_RX_PIN                      5
#define UART_TX_PORT                     P2
#define UART_TX_PIN                      6

#define PHOTO1_PORT                      P2
#define PHOTO1_PIN                       0
#define PHOTO2_PORT                      P2
#define PHOTO2_PIN                       1
#define PHOTO3_PORT                      P2
#define PHOTO3_PIN                       2
#define PHOTO4_PORT                      P2
#define PHOTO4_PIN                       4
#define PHOTO5_PORT                      P2
#define PHOTO5_PIN                       7
#define PHOTO6_PORT                      P1
#define PHOTO6_PIN                       2
#define PHOTO7_PORT                      P1
#define PHOTO7_PIN                       3

#endif /* DEFINES_H */
