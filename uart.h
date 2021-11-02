/******************************************************************************/

/** @file uart.c
*
* @brief This module provides functions to send and receive UART instructions.
*/

#ifndef UART_H_
#define UART_H_

typedef enum
{
    ROBOT,
    HUMAN,
    TBD,
    GAME_OVER
} turnType;

void uart_init(void);

turnType uart_receive_start(void);

uint8_t uart_receive_column(void);

turnType uart_receive_status(turnType current_turn);

void uart_send_column(uint8_t column);

#endif /* UART_H_ */
