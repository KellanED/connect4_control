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
} turn_t;

typedef enum
{
    ROBOT_CHIP_ERROR,
    HUMAN_CHIP_ERROR
} error_t;

void uart_init(void);

turn_t uart_receive_start(void);

uint8_t uart_receive_column(void);

turn_t uart_receive_status(turn_t current_turn);

void uart_send_column(uint8_t column);

void uart_send_error(error_t error);

#endif /* UART_H_ */

/*** end of file ***/
