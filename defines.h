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
#define TIMER_PWM_PIN                    GPIO_PIN5
#define TIMER_PWM_PIN_FUNCTION           GPIO_SECONDARY_MODULE_FUNCTION

#define DIR_PORT                         GPIO_PORT_P1
#define DIR_PIN                          GPIO_PIN0
#define DIR_PORT_OUT                     P1OUT

#endif /* DEFINES_H */
