/********************************************************
 * tempfsm.c
 *
 * SER486 Project 3
 * Fall 2020
 * Written By: Ty Foster (student)
 *
 * this file defines the behavior of the temperature
 * finite statemachine
 *
 * functions are:
 *      tempfsm_alert(int, char*) - sends an event to the
 *          log and changes the blink message of the led.
 *
 *      tempfsm_init(void) - initializes the temp FSM
 *
 *      tempfsm_reset(void) - resets the FSM to NORM1 state
 *
 *      tempfsm_update(int, int, int, int, int) - updates
 *          the FSM based on current state, temp, and the
 *          warning and critical temperature thresholds.
 *
 */

#include "alarm.h"
#include "led.h"
#include "log.h"
#include "tempfsm.h"

/* define states */
#define NORM1 0
#define NORM2 1
#define NORM3 2
#define WARNLO1 3
#define WARNLO2 4
#define CRITLO 5
#define WARNHI1 6
#define WARNHI2 7
#define CRITHI 8

static unsigned char state;

/**********************************
 * tempfsm_alert(int event, char* msg)
 *
 * initializes the temperature finite
 *  state machine
 *
 * arguments:
 *  event - the event code to register
 *      in the logs
 *  msg - the string to blink on the
 *      LED
 *
 * returns:
 *  nothing
 *
 * changes:
 *  nothing
 */
void tempfsm_alert(int event, char* msg) {
    alarm_send(event);
    log_add_record(event);
    led_set_blink(msg);
}

/**********************************
 * tempfsm_init(void)
 *
 * initializes the temperature finite
 *  state machine
 *
 * arguments:
 *  nothing
 *
 * returns:
 *  nothing
 *
 * changes:
 *  nothing
 */
void tempfsm_init(void) {
    /* INITIALIZE TEMP IN MAIN */
    /* set initial state */
    state = NORM1;
}

/**********************************
 * tempfsm_reset()
 *
 * this function resets the finite
 *  state machine for the temperature
 *  probe to NORM1
 *
 * arguments:
 *  nothing
 *
 * returns:
 *  nothing
 *
 * changes:
 *  State of tempFSM to NORM1
 */
void tempfsm_reset(void) {
    /* reset state to NORM1 */
    state = NORM1;
}

/**********************************
 * tempfsm_update(int tcurrent, int tcrit_h, int twarn_h,
 *                  int tcrit_l, int twarn_l)
 *
 * updates the state of the finite
 *  state machine based on predefinied states
 *
 * arguments:
 *  tcurrent - the current temperature of
 *      the device temperature sensor
 *  tcrit_h - the high critical temperature
 *  twarn_h - the high warning temp
 *  tcrit_l - the low critical temp
 *  twarn_l - the low warning temp
 *
 * returns:
 *  nothing
 *
 * changes:
 *  Blinking speed of LED to indicate
 *  if the temperature is in a safe
 *  range
 */
void tempfsm_update(int tcurrent, int tcrit_h, int twarn_h, int tcrit_l, int twarn_l) {
    /* determine state */
    switch(state) {
        case NORM1:
            if (tcurrent>=twarn_h) {
                tempfsm_alert(EVENT_HI_WARN, "-");
                state = WARNHI1;
            } else if (tcurrent<=twarn_l) {
                tempfsm_alert(EVENT_LO_WARN, "-");
                state = WARNLO1;
            }
            break;
        case NORM2:
            if (tcurrent>=twarn_h) {
                tempfsm_alert(EVENT_HI_WARN, "-");
                state = WARNHI2;
            } else if (tcurrent<=twarn_l) {
                led_set_blink("-");
                state = WARNLO1;
            }
            break;
        case NORM3:
            if (tcurrent>=twarn_h) {
                led_set_blink("-");
                state = WARNHI1;
            } else if (tcurrent<=twarn_l) {
                tempfsm_alert(EVENT_LO_WARN, "-");
                state = WARNLO1;
            }
            break;
        case WARNLO1:
            if (tcurrent>twarn_l){
                led_set_blink(" ");
                state = NORM2;
            } else if (tcurrent<=tcrit_l){
                tempfsm_alert(EVENT_LO_ALARM, ".");
                state = CRITLO;
            }
            break;
        case WARNLO2:
            if (tcurrent>twarn_l) {
                led_set_blink(" ");
                state = NORM2;
            } else if (tcurrent<=tcrit_l){
                led_set_blink(".");
                state = CRITLO;
            }
            break;
        case CRITLO:
            if (tcurrent>tcrit_l) {
                led_set_blink("-");
                state = WARNLO2;
            }
            break;
        case WARNHI1:
            if (tcurrent<twarn_h) {
                led_set_blink(" ");
                state = NORM3;
            } else if (tcurrent>=tcrit_h) {
                tempfsm_alert(EVENT_HI_ALARM, ".");
                state = CRITHI;
            }
            break;
        case WARNHI2:
            if (tcurrent<twarn_h) {
                led_set_blink(" ");
                state = NORM3;
            } else if (tcurrent>=tcrit_h) {
                led_set_blink(".");
                state = CRITHI;
            }
            break;
        case CRITHI:
            if (tcurrent<tcrit_h) {
                led_set_blink("-");
                state = WARNHI2;
            }
            break;
        default:
            /* if not in a valid state reset the fsm */
            tempfsm_reset();
            break;
    }
}
