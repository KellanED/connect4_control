/******************************************************************************/

/** @file uart.c
 *
 * @brief This module provides functions to send and receive UART instructions.
 */

// Includes
#include <stdint.h>
#include "driverlib.h"
#include "Board.h"
#include "uart.h"

// Local variables
static uint8_t RxData;
static uint8_t TxData;

/*!
 * @brief Initializes eUSCIA0 with 115200 baudrate. *** FOR ACTUAL ROBOT USE UART A1 NEED TO CHANGE
 *
 * @par
 * Current implementation: Rx = P1.5, Tx = P1.4
 * Final implementation: Rx = P2.5, Tx = P2.6
 */
void
uart_init(void)
{
    // Configure and enable UART
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

    // Configure UART pins
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
}   /* uart_init() */


/*!
 * @brief Wait until a start game instruction is received.
 * @return The starting turn, ROBOT or HUMAN.
 */
turnType
uart_receive_start(void)
{
    turnType turn = TBD;

    do
    {
        RxData = EUSCI_A_UART_receiveData(EUSCI_A0_BASE);
        if (RxData == 0x40) // @
        {
            turn = ROBOT;
        }
        else if (RxData == 0x47) // G
        {
            turn = HUMAN;
        }
    } while(turn == TBD);

    return turn;
}   /* uart_receive_start() */

/*!
 * @brief Wait until column data is received.
 * @return The column 0-6.
 */
uint8_t
uart_receive_column(void)
{
    do
    {
        RxData = EUSCI_A_UART_receiveData(EUSCI_A0_BASE);
    } while((RxData & 0x38) != 0x30);

    return RxData & 0x07; // p,q,r,s,t,u,v
}   /* uart_receive_column(); */

/*!
 * @brief Wait until a game status is received.
 * @param[in] current_turn The current turn in the game.
 * @return The next turn.
 */
turnType
uart_receive_status(turnType current_turn)
{
    turnType next_turn = TBD;
    do
    {
        RxData = EUSCI_A_UART_receiveData(EUSCI_A0_BASE);
    } while((RxData & 0x38) != 0x08);

    if (RxData == 0x48) // H
    {
        if (current_turn == ROBOT)
        {
            next_turn = HUMAN;
        }
        else
        {
            next_turn = ROBOT;
        }
    }
    else if (RxData == 0x4F) // O
    {
        next_turn = GAME_OVER;
    }

    return next_turn;
}   /* uart_receive_status() */

/*!
 * @brief Send column data.
 * @param[in] The column to send. 0-6
 */
void
uart_send_column(uint8_t column)
{
    TxData = 0x68 | column; // h,i,j,k,l,m,n
    EUSCI_A_UART_transmitData(EUSCI_A0_BASE, TxData);
}   /* uart_send_column() */
