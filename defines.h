/** @file defines.h
*
* @brief Contains system level defines.
*/

#ifndef DEFINES_H
#define DEFINES_H

#define CS_MCLK_DESIRED_FREQUENCY_IN_KHZ 16000   // Frequency for MCLK in kHz
#define CS_MCLK_FLLREF_RATIO             488     // MCLK/FLLRef ratio 16k/32786

#define TIMER_A_PERIOD                   62499   // 62500/250000 = 0.25, 4Hz
#define DUTY_CYCLE                       31249   // 50% duty cycle

#endif /* DEFINES_H */
