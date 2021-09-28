/** @file defines.h
*
* @brief Contains system level defines.
*/

#ifndef DEFINES_H
#define DEFINES_H

#define CS_MCLK_DESIRED_FREQUENCY_IN_KHZ 16000   // Frequency for MCLK in kHz
#define CS_MCLK_FLLREF_RATIO             488     // MCLK/FLLRef ratio 16k/32786

#define TIMER_PERIOD                     62499   // 62500/250000 = 0.25, 4Hz
#define TIMER_DUTY_CYCLE                 31249   // 50% duty cycle
#define TIMER_PWM_PORT                   GPIO_PORT_P1
#define TIMER_PWM_PIN                    GPIO_PIN1
#define TIMER_PWM_PIN_FUNCTION           GPIO_SECONDARY_MODULE_FUNCTION

#define DIR_PORT                         GPIO_PORT_P1
#define DIR_PIN                          GPIO_PIN0
#define DIR_PORT_OUT                     P1OUT

#define BUMP_PORT                        P2
#define BUMP_PIN                         7

#define UART_RX_PORT                     P1
#define UART_RX_PIN                      5
#define UART_TX_PORT                     P1
#define UART_TX_PIN                      4

#define PHOTO_PORT                       P2
#define PHOTO_PIN_1                      0
#define PHOTO_PIN_2                      1
#define PHOTO_PIN_3                      2
#define PHOTO_PIN_4                      3
#define PHOTO_PIN_5                      4
#define PHOTO_PIN_6                      5
#define PHOTO_PIN_7                      6

#endif /* DEFINES_H */
