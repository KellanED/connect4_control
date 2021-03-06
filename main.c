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
#include "photo.h"

// 1 or 0 of these should be uncommented, all commented for production robot
//#define bump_testing
//#define photo_testing
//#define uart_testing
//#define servo_testing
//#define stepper_testing // only test after testing bump switch
//#define mechanical_testing

static const uint16_t num_columns       = 7;
static const uint16_t steps_to_board    = 319;  // 319 steps = 45mm, 1000 steps = 141mm
static const uint16_t column_steps      = 248;  // 248 steps = 35mm

static turn_t   current_turn    = TBD;
static uint8_t  robot_column    = 0;
static uint8_t  human_column    = 0;

void main (void)
{

    // Stop watchdog timer
    WDT_A_hold(WDT_A_BASE);

    // Initialize MCLK = 16MHz, SMCLK = 2MHz, ACLK = 32.768kHz
    CS_initClockSignal(CS_FLLREF, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initFLLSettle(CS_MCLK_DESIRED_FREQUENCY_IN_KHZ, CS_MCLK_FLLREF_RATIO);
    CS_initClockSignal(CS_SMCLK, CS_DCOCLKDIV_SELECT, CS_CLOCK_DIVIDER_8);
    CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    // Initialize stepper driver
    stepper_init();

    // Initialize servo
    servo_init();

    // Initialize UART, baudrate = 115200
    uart_init();

    // Initialize photo-interrupters
    photo_init();

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PMM_unlockLPM5();

    // Enable global interrupts
    __bis_SR_register(GIE);

#if defined(uart_testing) || defined(servo_testing) || defined(bump_testing) || defined(stepper_testing) || defined(photo_testing)
    // Set S1 as input
    GPIO_setAsInputPinWithPullUpResistor(
        GPIO_PORT_S1,
        GPIO_PIN_S1
        );
    // Set S2 as input
    GPIO_setAsInputPinWithPullUpResistor(
        GPIO_PORT_S2,
        GPIO_PIN_S2
        );
#else
    // Retract chip dispenser
    servo_write_max();

    // Send stepper to 0 position
    stepper_enable();
    stepper_go_home();
    stepper_disable();

    // Wait for start game instruction from UART
    current_turn = uart_receive_start(); // @ = ROBOT, G = HUMAN
#endif

    while (1)
    {

#if !defined(uart_testing) && !defined(servo_testing) && !defined(bump_testing) && !defined(stepper_testing) && !defined(photo_testing) && !defined(mechanical_testing)
        if (ROBOT == current_turn)
        {
            // Wait for column instruction from UART
            robot_column = uart_receive_column(); // p,q,r,s,t,u,v

            // Move stepper motor to appropriate column, some weird math bc we 0 is farthest away
            uint16_t robot_column_steps = steps_to_board + column_steps * (num_columns - robot_column - 1);
            stepper_enable();
            stepper_send_steps(robot_column_steps, 1);
            stepper_disable();

            // Extend chip dispenser
            servo_write_min();

            // Poll photo-interrupters for correct column
            uint8_t detected_column;
            uint8_t error_sent = 0;
            do
            {
                detected_column = photo_wait(1);
                // Check for error and send if one hasn't been sent yet
                if ((detected_column != robot_column) && !error_sent)
                {
                    // Check for chip jam error
                    if (detected_column == 7) // 7 means it timed out
                    {
                        uart_send_error(1);
                    }
                    // or wrong column error
                    else
                    {
                        uart_send_error(0);
                    }
                    error_sent = 1;
                }
            }
            while (detected_column != robot_column);

            // Send no error
            uart_send_no_error(); // W

            // Retract chip dispenser
            servo_write_max();

            // Send stepper home
            stepper_enable();
            stepper_go_home();
            stepper_disable();

            // Wait for game status instruction from UART
            current_turn = uart_receive_status(current_turn); // H = next turn, O = game over
        }

        else if (HUMAN == current_turn)
        {
            // Poll photo-interrupters for chip detection
            human_column = photo_wait(0);

            // Send column instruction through UART
            uart_send_column(human_column); // h,i,j,k,l,m,n

            // Wait for game status instruction from UART
            current_turn = uart_receive_status(current_turn); // H = next turn, O = game over
        }

        else if (GAME_OVER == current_turn)
        {
            // Wait for start game instruction from UART
                current_turn = uart_receive_start(); // @ = ROBOT, G = HUMAN
        }

        else
        {
            // Should never get here, means that current_turn == TBD
            for (;;);
        }
#endif

#if defined(uart_testing)
        if (!GPIO_getInputPinValue(GPIO_PORT_S1, GPIO_PIN_S1))
        {
            // Should send h,i,j,k,l,m,n upon successive presses
            uart_send_column(human_column++);
        }
        if (!GPIO_getInputPinValue(GPIO_PORT_S2, GPIO_PIN_S2))
        {
            // Receive p,q,r,s,t,u,v
            robot_column = uart_receive_column();
        }
#endif

#if defined(servo_testing)
        // move to max value on one button, min value on other button
        if (!GPIO_getInputPinValue(GPIO_PORT_S1, GPIO_PIN_S1))
        {
            servo_write_min();
        }
        if (!GPIO_getInputPinValue(GPIO_PORT_S2, GPIO_PIN_S2))
        {
            servo_write_max();
        }
#endif

#if defined(bump_testing)
        // should increment human_column upon successive presses of bump switch
        if (!GPIO_getInputPinValue(BUMP_PORT, BUMP_PIN))
        {
            GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
            human_column++;
        }
#endif

#if defined(stepper_testing)
        // move 10 steps on one button, go home on other button
        if (!GPIO_getInputPinValue(GPIO_PORT_S1, GPIO_PIN_S1))
        {
            // 1000 steps = 141mm
            // 248 steps = 35mm
            stepper_enable();
            stepper_send_steps(248, 1);
            stepper_disable();
        }
        if (!GPIO_getInputPinValue(GPIO_PORT_S2, GPIO_PIN_S2))
        {
            stepper_enable();
            stepper_go_home();
            stepper_disable();
        }
#endif

#if defined(photo_testing)
        human_column = photo_wait();
#endif

#if defined(mechanical_testing)
        // Wait for column instruction from UART
        robot_column = uart_receive_column(); // p,q,r,s,t,u,v

        // Move stepper motor to appropriate column, some weird math bc we 0 is farthest away
        uint16_t robot_column_steps = steps_to_board + column_steps * (num_columns - robot_column - 1);
        stepper_enable();
        stepper_send_steps(robot_column_steps, 1);
        stepper_disable();

        // Extend chip dispenser
        servo_write_min();

        // Poll photo-interrupters for correct column
        uint8_t detected_column;
        uint8_t error_sent = 0;
        do
        {
            detected_column = photo_wait(1);
            // Check for error and send if one hasn't been sent yet
            if ((detected_column != robot_column) && !error_sent)
            {
                // Check for chip jam error
                if (detected_column == 7) // 7 means it timed out
                {
                    uart_send_error(1); // y
                }
                // or wrong column error
                else
                {
                    uart_send_error(0); // x
                }
                error_sent = 1;
            }
        }
        while (detected_column != robot_column);

        // Send no error
        uart_send_no_error(); // W

        // Retract chip dispenser
        servo_write_max();

        // Send stepper home
        stepper_enable();
        stepper_go_home();
        stepper_disable();
#endif

    }
}

/*** end of file ***/

