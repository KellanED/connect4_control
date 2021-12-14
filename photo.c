/** @file photo.c
*
* @brief This module provides control functions for reading the photo-interrupters.
*/

// Includes
#include <stdint.h>
#include "driverlib.h"
#include "Board.h"
#include "photo.h"
#include "defines.h"

// Local variables
static uint16_t                 timeout_cycles  = 2559; // (2559+1)/512 = 5 seconds
static Timer_A_initUpModeParam  param           = {0};

/*!
* @brief Initializes photo-interrupters to be used for chip detection.
*/
void
photo_init (void)
{
    // Set pins as input
    P2DIR &= ~(PHOTO1 | PHOTO2 | PHOTO3 | PHOTO4 | PHOTO5);
    P1DIR &= ~(PHOTO6 | PHOTO7);

    // Disable pull up or pull down resistor
    P2REN &= ~(PHOTO1 | PHOTO2 | PHOTO3 | PHOTO4 | PHOTO5);
    P1REN &= ~(PHOTO6 | PHOTO7);

    // Configure TimerA2 in up mode. Cycles at 512Hz. 5 seconds at 2559
    param.clockSource                               = TIMER_A_CLOCKSOURCE_ACLK;
    param.clockSourceDivider                        = TIMER_A_CLOCKSOURCE_DIVIDER_64;
    param.timerPeriod                               = 0xFFFF;
    param.timerInterruptEnable_TAIE                 = TIMER_A_TAIE_INTERRUPT_DISABLE;
    param.captureCompareInterruptEnable_CCR0_CCIE   = TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE;
    param.timerClear                                = TIMER_A_DO_CLEAR;
    param.startTimer                                = true;
    Timer_A_initUpMode(TIMER_A2_BASE, &param);
}

/*!
* @brief Waits until a chip is detected with photo-interrupters.
* @param[in] check_timeout Should the function timeout after 5 seconds? 1 = yes, 0 = no
* @return The column that the chip was detected in. 0-6, 7 if timed out
*/
uint8_t
photo_wait (uint8_t check_timeout)
{
    uint8_t sensors      = PHOTO_IN;
    uint8_t sensors_prev = PHOTO_IN;
    uint8_t sensors_diff = sensors ^ sensors_prev;
    uint8_t position     = 0;
    TA2R = 0x0000; // reset timer counter

    // Wait until one of the sensors detects a chip
    while (!(sensors_diff))
    {
        // Detect timeout
        if (check_timeout && (timeout_cycles <= Timer_A_getCounterValue(TIMER_A2_BASE)))
        {
            sensors_diff = 0x80;
            break;
        }
        sensors_prev = sensors;
        sensors = PHOTO_IN;
        sensors_diff = sensors ^ sensors_prev;
    }

    // Determine position of sensed chip
    while (sensors_diff)
    {
        sensors_diff = sensors_diff >> 1;
        position++;
    }

    return (position - 1);
}
