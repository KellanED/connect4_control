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
#include "uart.h"

static turn_t   current_turn    = TBD;
static uint8_t  RobotColumn     = 0;
static uint8_t  HumanColumn     = 0;

void main (void)
{

    // Stop watchdog timer
    WDT_A_hold(WDT_A_BASE);

    // Initialize MCLK = 16MHz, SMCLK = 2MHz, ACLK = 32.768kHz
    CS_initClockSignal(CS_FLLREF, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initFLLSettle(CS_MCLK_DESIRED_FREQUENCY_IN_KHZ, CS_MCLK_FLLREF_RATIO);
    CS_initClockSignal(CS_SMCLK, CS_DCOCLKDIV_SELECT, CS_CLOCK_DIVIDER_8);

    // Initialize stepper driver
    stepper_init();

    // Initialize servo
    servo_init();

    // Initialize UART, baudrate = 115200
    uart_init();

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PMM_unlockLPM5();

    // Enable global interrupts
    __bis_SR_register(GIE);

    // Enable when everything is hooked up
#if 0
    stepper_go_home();
#endif

    // Set S1 as input. for testing, remove later
    GPIO_setAsInputPinWithPullUpResistor(
        GPIO_PORT_S1,
        GPIO_PIN_S1
        );

    // Wait for start game instruction from UART
    current_turn = uart_receive_start(); // @ = ROBOT, G = HUMAN

    while (1)
    {
        if (ROBOT == current_turn)
        {
            // Wait for column instruction from UART
            RobotColumn = uart_receive_column(); // p,q,r,s,t,u,v

            // Move stepper motor
            // Activate servo
            // Poll photo-interrupters for correct column
            // Respond to chip errors if necessary
            // Deactivate servo
            // Send stepper home

            // Wait for game status instruction from UART
            current_turn = uart_receive_status(current_turn); // H = next turn, O = game over
        }

        else if (HUMAN == current_turn)
        {
            // Poll photo-interrupters for chip detection
            while (GPIO_getInputPinValue(GPIO_PORT_S1, GPIO_PIN_S1));
            HumanColumn = 3;

            // Send column instruction through UART
            uart_send_column(HumanColumn); // h,i,j,k,l,m,n

            // Wait for game status instruction from UART
            current_turn = uart_receive_status(current_turn); // H = next turn, O = game over
        }

        else if (GAME_OVER == current_turn)
        {
            // For now, end the game by getting stuck here
            for (;;);
        }

        else
        {
            // Should never get here, means that current_turn == TBD
            for (;;);
        }
    }
}

/*** end of file ***/

