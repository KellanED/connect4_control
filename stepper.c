/******************************************************************************/

/** @file stepper.c
*
* @brief This module provides control functions for the stepper motor.
*/

// Includes
#include <stdint.h>
#include "driverlib.h"
#include "Board.h"
#include "stepper.h"
#include "defines.h"

// Local variables
static uint16_t count = 0;
static Timer_A_outputPWMParam param = {0};

/*!
* @brief Initializes TimerA1 to be used for PWM output for the stepper motor.
*
* @par
* PWM output should be on TA1.1 = P1.5.
*/
void stepper_init(void)
{
    // Configure PWM - TimerA0 runs in Up mode
    param.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
    param.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_8;
    param.timerPeriod = TIMER_PERIOD;
    param.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1;
    param.compareOutputMode = TIMER_A_OUTPUTMODE_SET_RESET;
    param.dutyCycle = TIMER_DUTY_CYCLE;

    // Set PWM output pin.
    GPIO_setAsPeripheralModuleFunctionOutputPin(
        TIMER_PWM_PORT,
        TIMER_PWM_PIN,
        TIMER_PWM_PIN_FUNCTION
        );

    // Set direction output pin.
    GPIO_setAsOutputPin(
        DIR_PORT,
        DIR_PIN
        );

    // Set bump input pin
    GPIO_setAsInputPinWithPullUpResistor(
        BUMP_PORT,
        BUMP_PIN
        );
}

/*!
* @brief Send a number of steps to the stepper motor.
* @param[in] num The number of steps to be run.
* @param[in] dir The direction the motor should spin. 1 or 0
* @par
* This function can only be run after stepper_init() is run.
*/
void stepper_send_steps(uint16_t num, uint8_t dir)
{
    // Change direction according to parameter
    if (dir)
    {
        GPIO_setOutputHighOnPin(DIR_PORT, DIR_PIN);
    }
    else
    {
        GPIO_setOutputLowOnPin(DIR_PORT, DIR_PIN);
    }

    // Change count to number of steps and start PWM output
    count = num;
    Timer_A_outputPWM(TIMER_A0_BASE, &param);
    Timer_A_enableInterrupt(TIMER_A0_BASE);

    // Wait until all steps have been run before executing other code
    while (count != 0);
}

/*!
* @brief Moves the stepper motor back to its home position as determined
* by a limit switch.
* @par
* This function can only be run after stepper_init() is run.
*/
void stepper_go_home(void)
{
    while (GPIO_getInputPinValue(BUMP_PORT, BUMP_PIN))
    {
        stepper_send_steps(1, 0);
    }
}

/*!
* @brief TIMER0_A3 interrupt vector ISR
*
* @par
* Should trigger when timer counts to 0.
*/
#pragma vector=TIMER0_A1_VECTOR
__interrupt void timer0_a1_isr(void)
{
    // Decrement count and stop PWM output if no more steps left
    count--;
    if (count <= 0)
    {
        Timer_A_stop(TIMER_A0_BASE);
    }

    // Clear interrupt flag
    Timer_A_clearTimerInterrupt(TIMER_A0_BASE);
}
