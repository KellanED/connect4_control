/******************************************************************************/

/** @file uart.c
 *
 * @brief This module provides functions to send and receive UART instructions.
 *
 * Instruction Set:
 * 1 byte instructions
 * 2 bit header, 3 bit opcode, 3 bit operand
 * 01 000 000   start game      robot first     @
 * 01 000 111   start game      human first     G
 * 01 001 000   game status     not finished    H
 * 01 001 111   game status     game over       O
 * 01 101 ABC   human column    abc = bin col#  h,i,j,k,l,m,n
 * 01 110 abc   robot column    abc = bin col#  p,q,r,s,t,u,v
 *
 * TODO:
 * 01 111 000   Error           robot chip
 * 01 111 001   Error           human chip
 */

// Includes
#include <stdint.h>
#include "driverlib.h"
#include "Board.h"
#include "uart.h"

// Local variables
static uint8_t RxData = 0;
static uint8_t TxData = 0;

/*!
 * @brief Initializes eUSCIA0 with 115200 baudrate.
 * TODO: FOR ACTUAL ROBOT USE UART A1 NEED TO CHANGE
 *
 * @par
 * Current implementation: Rx = P1.5, Tx = P1.4
 * Final implementation: Rx = P2.5, Tx = P2.6
 */
void
uart_init (void)
{
    // Configure and enable UART
    EUSCI_A_UART_initParam UARTparam = {0};
    UARTparam.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;
    UARTparam.clockPrescalar    = 17;
    UARTparam.firstModReg       = 0;
    UARTparam.secondModReg      = 74;
    UARTparam.parity            = EUSCI_A_UART_NO_PARITY;
    UARTparam.msborLsbFirst     = EUSCI_A_UART_LSB_FIRST;
    UARTparam.numberofStopBits  = EUSCI_A_UART_ONE_STOP_BIT;
    UARTparam.uartMode          = EUSCI_A_UART_MODE;
    UARTparam.overSampling      = 0;
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
turn_t
uart_receive_start (void)
{
    turn_t initial_turn = TBD;

    // Stay in this loop until the appropriate instruction is received.
    do
    {
        RxData = EUSCI_A_UART_receiveData(EUSCI_A0_BASE);
        if (0x40 == RxData) // @
        {
            initial_turn = ROBOT;
        }
        else if (0x47 == RxData) // G
        {
            initial_turn = HUMAN;
        }
    }
    while (TBD == initial_turn);

    return initial_turn;
}   /* uart_receive_start() */

/*!
 * @brief Wait until column data is received.
 * @return The column 0-6.
 */
uint8_t
uart_receive_column (void)
{
    // Stay in this loop until the appropriate instruction is received.
    do
    {
        RxData = EUSCI_A_UART_receiveData(EUSCI_A0_BASE);
    }
    while (0x30 != (RxData & 0x38));

    return RxData & 0x07; // p,q,r,s,t,u,v
}   /* uart_receive_column(); */

/*!
 * @brief Wait until a game status is received.
 * @param[in] current_turn The current turn in the game.
 * @return The next turn.
 */
turn_t
uart_receive_status (turn_t current_turn)
{
    turn_t next_turn = TBD;

    // Stay in this loop until the proper instruction is received.
    do
    {
        RxData = EUSCI_A_UART_receiveData(EUSCI_A0_BASE);
    }
    while (0x08 != (RxData & 0x38));

    // Determine next turn if the game is not over
    if (0x48 == RxData) // H
    {
        if (ROBOT == current_turn)
        {
            next_turn = HUMAN;
        }
        else
        {
            next_turn = ROBOT;
        }
    }
    else if (0x4F == RxData) // O
    {
        next_turn = GAME_OVER;
    }

    return next_turn;
}   /* uart_receive_status() */

/*!
 * @brief Encode and send column data.
 * @param[in] The column to send. 0-6
 */
void
uart_send_column (uint8_t column)
{
    TxData = 0x68 | column; // h,i,j,k,l,m,n
    EUSCI_A_UART_transmitData(EUSCI_A0_BASE, TxData);
}   /* uart_send_column() */

/*** end of file ***/
