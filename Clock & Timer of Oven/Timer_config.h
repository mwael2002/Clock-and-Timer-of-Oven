/*
 * Timer_config.h
 *
 *  Created on: Jul 12, 2022
 *      Author: mwael
 */

#ifndef TIMER_CONFIG_H_
#define TIMER_CONFIG_H_

/************************************************************************************************/

#define TIMER0_MODE                                      TIMER0_NORMAL_MODE
#define TIMER0_PRESCALER                                 TIMER0_DIVIDE_BY_8
#define TIMER0_OVERFLOW_TIME_MILLISECONDS                500
#define TIMER0_CTC_MODE_OUTPUT                           TIMER0_CTC_TOGGLE
#define TIMER0_FAST_PWM_MODE_OUTPUT                      TIMER0_FAST_PWM_SET_CMP
#define TIMER0_PHASECORRECT_PWM_MODE_OUTPUT              TIMER0_PHASECORRECT_PWM_SET_CMP_COUNTUP

/************************************************************************************************/

#define TIMER1_MODE                                      TIMER1_NORMAL_MODE
#define TIMER1_PRESCALER                                 TIMER1_DIVIDE_BY_64
#define TIMER1_OVERFLOW_TIME_MILLISECONDS                1000

#define TIMER1_USE_CMPB                                  1

#define TIMER1_CMPA_CTC_MODE_OUTPUT                      TIMER1_CMPA_CTC_TOGGLE
#define TIMER1_CMPA_FAST_PWM_MODE_OUTPUT                 TIMER1_CMPA_FAST_PWM_SET_CMP
#define TIMER1_CMPA_PHASECORRECT_PWM_MODE_OUTPUT         TIMER1_CMPA_PHASECORRECT_PWM_CLEAR_CMP_COUNTUP

#define TIMER1_CMPB_CTC_MODE_OUTPUT                      TIMER1_CMPB_CTC_TOGGLE
#define TIMER1_CMPB_FAST_PWM_MODE_OUTPUT                 TIMER1_CMPB_FAST_PWM_CLEAR_CMP
#define TIMER1_CMPB_PHASECORRECT_PWM_MODE_OUTPUT         TIMER1_CMPB_PHASECORRECT_PWM_CLEAR_CMP_COUNTUP

/*************************************************************************************************/

#define TIMER2_MODE                                      TIMER2_NORMAL_MODE
#define TIMER2_PRESCALER                                 TIMER2_DIVIDE_BY_1024
#define TIMER2_OVERFLOW_TIME_MILLISECONDS                1000
#define TIMER2_CTC_MODE_OUTPUT                           TIMER2_CTC_TOGGLE
#define TIMER2_FAST_PWM_MODE_OUTPUT                      TIMER2_FAST_PWM_CLEAR_CMP
#define TIMER2_PHASECORRECT_PWM_MODE_OUTPUT              TIMER2_PHASECORRECT_PWM_CLEAR_CMP_COUNTUP


#endif /* TIMER_CONFIG_H_ */
