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

// includes
#include "driverlib.h"
#include "Board.h"

// defines
#define CS_MCLK_DESIRED_FREQUENCY_IN_KHZ    16000   // target frequency for MCLK in kHz
#define CS_MCLK_FLLREF_RATIO                488     // MCLK/FLLRef ratio, 16000000/32768
#define TIMER_A_PERIOD                      62499   // Timer period = 62500/250000 = 0.25, 4Hz
#define DUTY_CYCLE                          31249   // 50% duty cycle

void main (void)
{
    // Stop watchdog timer
    WDT_A_hold(WDT_A_BASE);

    // Set FLL Reference as REFO - 32.769kHz
    CS_initClockSignal(
        CS_FLLREF,
        CS_REFOCLK_SELECT,
        CS_CLOCK_DIVIDER_1
        );

    // Set Ratio/Desired MCLK Frequency, initialize DCO and MCLK to 16MHz
    CS_initFLLSettle(
        CS_MCLK_DESIRED_FREQUENCY_IN_KHZ,
        CS_MCLK_FLLREF_RATIO
        );

    // Set SMCLK to 16MHz/8 = 2MHz
    CS_initClockSignal(
        CS_SMCLK,
        CS_DCOCLKDIV_SELECT,
        CS_CLOCK_DIVIDER_8
        );

    // Generate PWM - TimerA1 runs in Up mode
    Timer_A_outputPWMParam param = {0};
    param.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;              // SMCLK = 2MHz
    param.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_8;   // Divide to 250kHz
    param.timerPeriod = TIMER_A_PERIOD;
    param.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1;
    param.compareOutputMode = TIMER_A_OUTPUTMODE_SET_RESET;
    param.dutyCycle = DUTY_CYCLE;
    Timer_A_outputPWM(TIMER_A1_BASE, &param);

    // Enable TimerA1 interrupts
    Timer_A_enableInterrupt(TIMER_A1_BASE);

    // TA1.1 = P1.5 as PWM output, *********** NEED TO VERIFY IF THIS WORKS *************
    GPIO_setAsPeripheralModuleFunctionOutputPin(
        GPIO_PORT_P1,
        GPIO_PIN5,
        GPIO_SECONDARY_MODULE_FUNCTION
        );

    //Set LED1 as an output pin.
    GPIO_setAsOutputPin(
        GPIO_PORT_LED1,
        GPIO_PIN_LED1
        );

    /*
     * Disable the GPIO power-on default high-impedance mode to activate
     * previously configured port settings
     */
    PMM_unlockLPM5();

    // Enable interrupts
    __bis_SR_register(GIE);

    while(1) {
        // TAIFG interrupt flag is set when timer counts from TAxCCR0 to zero
        // Output on TA1.1 = P1.5
        __no_operation();
    }

}

// TIMER1_A3 interrupt vector ISR, should trigger when timer counts to 0
#pragma vector=TIMER1_A1_VECTOR
__interrupt void TIMER1_A1_ISR(void) {
    __no_operation();

    //Toggle LED1
    GPIO_toggleOutputOnPin(
        GPIO_PORT_LED1,
        GPIO_PIN_LED1
        );

    Timer_A_clearTimerInterrupt(TIMER_A1_BASE);
}
