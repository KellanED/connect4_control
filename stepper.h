/******************************************************************************/

/** @file stepper.h
*
* @brief This module provides control functions for the stepper motor.
*/

#ifndef STEPPER_H
#define STEPPER_H

void stepper_init(void);

void stepper_send_steps(uint16_t num, uint8_t dir);

void stepper_go_home(void);

__interrupt void timer1_a1_isr(void);

#endif /* STEPPER_H */
