/******************************************************************************/

/** @file stepper.c
*
* @brief This module provides control functions for the stepper motor.
*/

// Includes
#include "driverlib.h"
#include "Board.h"
#include "stepper.h"
#include "defines.h"

/*!
* @brief Initializes TimerA1 to be used for PWM output for the stepper motor.
*
* @par
* PWM output should be on TA1.1 = P1.5.
*/
void stepper_init(void)
{
    // Generate PWM - TimerA1 runs in Up mode
    Timer_A_outputPWMParam param = {0};
    param.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
    param.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_8;
    param.timerPeriod = TIMER_A_PERIOD;
    param.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1;
    param.compareOutputMode = TIMER_A_OUTPUTMODE_SET_RESET;
    param.dutyCycle = DUTY_CYCLE;
    Timer_A_outputPWM(TIMER_A1_BASE, &param);

    // TA1.1 = P1.5 as PWM output, *********** NEED TO VERIFY IF THIS WORKS *************
    GPIO_setAsPeripheralModuleFunctionOutputPin(
        GPIO_PORT_P1,
        GPIO_PIN5,
        GPIO_SECONDARY_MODULE_FUNCTION
        );

    // Enable TimerA1 interrupts
    Timer_A_enableInterrupt(TIMER_A1_BASE);
}

/*!
* @brief Send a number of steps to the stepper motor.
* @param[in] num The number of steps to be run.
* @par
* This function can only be run after stepper_init() is run.
*/
void stepper_send_steps(uint16_t num);

/*!
* @brief Moves the stepper motor back to its home position as determined
* by a limit switch.
* @par
* This function can only be run after stepper_init() is run.
*/
void stepper_go_home(void);

/*!
* @brief TIMER1_A3 interrupt vector ISR
*
* @par
* Should trigger when timer counts to 0.
*/
#pragma vector=TIMER1_A1_VECTOR
__interrupt void timer1_a1_isr(void)
{
    __no_operation();

    //Toggle LED1
    GPIO_toggleOutputOnPin(
        GPIO_PORT_LED1,
        GPIO_PIN_LED1
        );

    Timer_A_clearTimerInterrupt(TIMER_A1_BASE);
}
