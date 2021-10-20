/******************************************************************************/

/** @file servo.c
*
* @brief This module provides control functions for the servo motor.
*/

// Includes
#include <stdint.h>
#include "driverlib.h"
#include "Board.h"
#include "servo.h"
#include "defines.h"

// Local variables
static Timer_A_outputPWMParam param = {0};

/*!
* @brief Initializes TimerA1 to be used for PWM output for the servo motor.
*
* @par
* PWM output should be on TA1.1 = P1.5.
*/
void servo_init(void)
{
    // Configure PWM - TimerA1 runs in Up mode
    param.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
    param.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_8;
    param.timerPeriod = SERVO_TIMER_PERIOD;
    param.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_2;
    param.compareOutputMode = TIMER_A_OUTPUTMODE_RESET_SET;
    param.dutyCycle = SERVO_MAX_DUTY;

    // Set PWM output pin.
    GPIO_setAsPeripheralModuleFunctionOutputPin(
        SERVO_PORT,
        SERVO_PIN,
        SERVO_PIN_FUNCTION
        );

    Timer_A_outputPWM(TIMER_A1_BASE, &param);
    //Timer_A_enableInterrupt(TIMER_A1_BASE);
}

/*!
* @brief Writes the servo to its minimum position.
*/
void servo_write_min(void)
{
    param.dutyCycle = SERVO_MIN_DUTY;
    Timer_A_outputPWM(TIMER_A1_BASE, &param);
}

/*!
* @brief Writes the servo to its maximum position.
*/
void servo_write_max(void)
{
    param.dutyCycle = SERVO_MAX_DUTY;
    Timer_A_outputPWM(TIMER_A1_BASE, &param);
}

/*!
* @brief TIMER1_A3 interrupt vector ISR
*
* @par
* Should trigger when timer counts to 0.
*/
#pragma vector=TIMER1_A1_VECTOR
__interrupt void timer1_a1_isr(void)
{
    // Clear interrupt flag
    Timer_A_clearTimerInterrupt(TIMER_A0_BASE);
}
