/******************************************************************************/

/** @file servo.h
*
* @brief This module provides control functions for the servo motor.
*/

#ifndef SERVO_H
#define SERVO_H

void servo_init(void);

void servo_write_min(void);

void servo_write_max(void);

__interrupt void timer1_a1_isr(void);

#endif /* SERVO_H */

/*** end of file ***/
