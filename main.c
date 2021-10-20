/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/

// Includes
#include "driverlib.h"
#include "Board.h"
#include "defines.h"
#include "stepper.h"
#include "servo.h"

/******************************************************************************/

void main (void)
{

    // Stop watchdog timer
    WDT_A_hold(WDT_A_BASE);

    // Initialize MCLK as 16MHz and SMCLK as 2MHz
    CS_initClockSignal(CS_FLLREF, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initFLLSettle(CS_MCLK_DESIRED_FREQUENCY_IN_KHZ, CS_MCLK_FLLREF_RATIO);
    CS_initClockSignal(CS_SMCLK, CS_DCOCLKDIV_SELECT, CS_CLOCK_DIVIDER_8);

    // Initialize stepper driver
    stepper_init();

    // Initialize servo
    servo_init();

    // Set P1.5 as an output pin. - for verification, remove later
    GPIO_setAsOutputPin(
        GPIO_PORT_P1,
        GPIO_PIN5
        );

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PMM_unlockLPM5();

    // Enable global interrupts
    __bis_SR_register(GIE);

    // Set S1 as input. for verification, remove later
    GPIO_setAsInputPinWithPullUpResistor(
            GPIO_PORT_S1,
            GPIO_PIN_S1
            );

    // Set S2 as input. for verification, remove later
    GPIO_setAsInputPinWithPullUpResistor(
            GPIO_PORT_S2,
            GPIO_PIN_S2
            );

    while(1)
    {
        // testing servo
        if (!GPIO_getInputPinValue(GPIO_PORT_S1, GPIO_PIN_S1))
        {
            servo_write_min();
        }
        if (!GPIO_getInputPinValue(GPIO_PORT_S2, GPIO_PIN_S2))
        {
            servo_write_max();
        }
    }

}
