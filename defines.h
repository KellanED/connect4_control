/** @file defines.h
*
* @brief Contains system level defines.
*/

#ifndef DEFINES_H
#define DEFINES_H

#define CS_MCLK_DESIRED_FREQUENCY_IN_KHZ 16000   // Frequency for MCLK in kHz
#define CS_MCLK_FLLREF_RATIO             488     // MCLK/FLLRef ratio 16k/32786

// Stepper PWM Output using Timer0_A3
#define TIMER_PERIOD                     249   // 250/250000 = 0.001, 1kHz
#define TIMER_DUTY_CYCLE                 124   // 50% duty cycle
#define TIMER_PWM_PORT                   GPIO_PORT_P1
#define TIMER_PWM_PIN                    GPIO_PIN1
#define TIMER_PWM_PIN_FUNCTION           GPIO_SECONDARY_MODULE_FUNCTION
#define DIR_PORT                         GPIO_PORT_P1
#define DIR_PIN                          GPIO_PIN0
#define DIR_PORT_OUT                     P1OUT
#define NENABLE_PORT                     GPIO_PORT_P3
#define NENABLE_PIN                      GPIO_PIN1

// Servo PWM Output using Timer1_A3
#define SERVO_TIMER_PERIOD               4999    // 5000/250000 = 0.02, 50Hz
#define SERVO_MIN_DUTY                   124     // 125/250000 = 500us
#define SERVO_MAX_DUTY                   574     // 575/250000 = 2300us
#define SERVO_PORT                       GPIO_PORT_P1
#define SERVO_PIN                        GPIO_PIN4
#define SERVO_PIN_FUNCTION               GPIO_SECONDARY_MODULE_FUNCTION

// Bump sensor for stepper homing
#define BUMP_PORT                        GPIO_PORT_P3
#define BUMP_PIN                         GPIO_PIN2

// UART1
#define UART_RX_PORT                     GPIO_PORT_P2
#define UART_RX_PIN                      GPIO_PIN5
#define UART_RX_FUNCTION                 GPIO_PRIMARY_MODULE_FUNCTION
#define UART_TX_PORT                     GPIO_PORT_P2
#define UART_TX_PIN                      GPIO_PIN6
#define UART_TX_FUNCTION                 GPIO_PRIMARY_MODULE_FUNCTION

// Photointerrupters
#define PHOTO1_PORT                      P2
#define PHOTO1                           BIT0
#define PHOTO2_PORT                      P2
#define PHOTO2                           BIT1
#define PHOTO3_PORT                      P2
#define PHOTO3                           BIT2
#define PHOTO4_PORT                      P2
#define PHOTO4                           BIT4
#define PHOTO5_PORT                      P2
#define PHOTO5                           BIT7
#define PHOTO6_PORT                      P1
#define PHOTO6                           BIT2
#define PHOTO7_PORT                      P1
#define PHOTO7                           BIT3

#define PHOTO_IN                        ((P1IN & (PHOTO7 | PHOTO6)) << 3) | ((P2IN & PHOTO5) >> 3) | ((P2IN & PHOTO4) >> 1) | (P2IN & (PHOTO3 | PHOTO2 | PHOTO1))

#endif /* DEFINES_H */

/*** end of file ***/

