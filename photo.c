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
}

/*!
* @brief Waits until a chip is detected with photo-interrupters.
* @return The column that the chip was detected in. 0-6
*/
uint8_t
photo_wait (void)
{
    uint8_t sensors      = PHOTO_IN;
    uint8_t sensors_prev = PHOTO_IN;
    uint8_t sensors_diff = sensors ^ sensors_prev;
    uint8_t position     = 0;

    // Wait until one of the sensors detects a chip
    while (!(sensors_diff))
    {
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
