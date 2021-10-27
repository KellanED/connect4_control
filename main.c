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

typedef enum
{
    ROBOT,
    HUMAN,
    TBD,
    GAME_OVER
} turnType;
turnType TURN = TBD;

uint8_t RxData;
uint8_t TxData;
uint8_t RobotColumn;
uint8_t HumanColumn;

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

    // Initialize UART, baudrate = 115200 *** FOR ACTUAL ROBOT USE UART A1 NEED TO CHANGE
    EUSCI_A_UART_initParam UARTparam = {0};
    UARTparam.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;
    UARTparam.clockPrescalar = 17;
    UARTparam.firstModReg = 0;
    UARTparam.secondModReg = 74;
    UARTparam.parity = EUSCI_A_UART_NO_PARITY;
    UARTparam.msborLsbFirst = EUSCI_A_UART_LSB_FIRST;
    UARTparam.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT;
    UARTparam.uartMode = EUSCI_A_UART_MODE;
    UARTparam.overSampling = 0;
    EUSCI_A_UART_init(EUSCI_A0_BASE, &UARTparam);
    EUSCI_A_UART_enable(EUSCI_A0_BASE);

    //Configure UART pins
    GPIO_setAsPeripheralModuleFunctionOutputPin(
        GPIO_PORT_UCA0TXD,
        GPIO_PIN_UCA0TXD,
        GPIO_FUNCTION_UCA0TXD
    );
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_UCA0RXD,
        GPIO_PIN_UCA0RXD,
        GPIO_FUNCTION_UCA0RXD
    );

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PMM_unlockLPM5();

    // Enable global interrupts
    __bis_SR_register(GIE);

    // Uncomment when everything is hooked up
    //stepper_go_home();

    // Set S1 as input. for testing, remove later
    GPIO_setAsInputPinWithPullUpResistor(
        GPIO_PORT_S1,
        GPIO_PIN_S1
        );

    // Wait for start game instruction from UART
    do
    {
        RxData = EUSCI_A_UART_receiveData(EUSCI_A0_BASE);
        if (RxData == 0x40) // @
        {
            TURN = ROBOT;
        }
        else if (RxData == 0x47) // G
        {
            TURN = HUMAN;
        }
    } while(TURN == TBD);

    while(1)
    {
        if (TURN == ROBOT)
        {
            // Wait for column instruction from UART
            do
            {
                RxData = EUSCI_A_UART_receiveData(EUSCI_A0_BASE);
            } while((RxData & 0x38) != 0x30);

            RobotColumn = RxData & 0x07; // p,q,r,s,t,u,v
            // Move stepper motor
            // Activate servo
            // Poll photo-interrupters for correct column
            // Respond to chip errors if necessary
            // Deactivate servo
            // Send stepper home

            // Wait for game status instruction from UART
            do
            {
                RxData = EUSCI_A_UART_receiveData(EUSCI_A0_BASE);
            } while((RxData & 0x38) != 0x08);

            if (RxData == 0x48) // H
            {
                TURN = HUMAN;
            }
            else if (RxData == 0x4F) // O
            {
                TURN = GAME_OVER;
            }
        }

        else if (TURN == HUMAN)
        {
            // Poll photo-interrupters for chip detection
            while (GPIO_getInputPinValue(GPIO_PORT_S1, GPIO_PIN_S1));
            HumanColumn = 3;
            TxData = 0x68 | HumanColumn; // h,i,j,k,l,m,n

            // Send column instruction through UART
            EUSCI_A_UART_transmitData(EUSCI_A0_BASE, TxData);

            // Wait for game status instruction from UART
            do
            {
                RxData = EUSCI_A_UART_receiveData(EUSCI_A0_BASE);
            } while((RxData & 0x38) != 0x08);

            if (RxData == 0x48) // H
            {
                TURN = ROBOT;
            }
            else if (RxData == 0x4F) // O
            {
                TURN = GAME_OVER;
            }
        }

        else if (TURN == GAME_OVER)
        {
            while(1);
        }
    }
}
