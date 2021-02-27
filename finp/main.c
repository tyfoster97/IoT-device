/*************************************************************************
* main.c
*
* This is template code for the SER486 final exam.  Students may use this
* as a starting point their final project.
*/
#include "config.h"
#include "delay.h"
#include "dhcp.h"
#include "led.h"
#include "log.h"
#include "rtc.h"
#include "spi.h"
#include "uart.h"
#include "vpd.h"
#include "temp.h"
#include "socket.h"
#include "alarm.h"
#include "wdt.h"
#include "tempfsm.h"
#include "eeprom.h"
#include "ntp.h"
#include "w51.h"
#include "signature.h"

#define HTTP_PORT 8080 /* TCP port for HTTP */
#define SERVER_SOCKET 0

#define PARSE_CMD 1
#define PARSE_ENDP 2
#define PARSE_PARAM 3
#define PROCESS_REQ 4
#define SEND_REP 5
#define ERR_REP 9
#define WAITING 0

#define GET 'g'
#define DELETE 'd'
#define PUT 'p'
#define CONFIG 'c'
#define DEVICE 'd'
#define LOG 'l'
#define RESET 'r'
#define TC_HI 't'
#define TW_HI 'w'
#define TC_LO 'l'
#define TW_LO 'o'

int current_temperature = 75;

static char state = WAITING;
static char cmd;
static char endp;
static char param;
static unsigned char ch = ' ';

/********
 * is_delimeter(char c)
 *
 * Determines if a character is a delimeter
 *
 * params:
 *      c - character to compare
 *
 * return:
 *      1 if characters match, otherwise 0
 *
 * changes:
 *      none
 */
unsigned char is_delimeter(char c) {
    return c=='/';
}

int main(void)
{
    /* Initialize the hardware devices
	 * uart
	 * led
	 * vpd
	 * config
     * log
     * rtc
     * spi
     * temp sensor
	 * W51 Ethernet controller
     * tempfsm
     */
    uart_init();
    led_init();
    vpd_init();
    config_init();
    log_init();
    rtc_init();
    spi_init();
    temp_init();
    W5x_init();
    tempfsm_init();

    /* sign the assignment
    * Asurite is the first part of your asu email (before the @asu.edu
    */
    signature_set("Ty", "Foster", "tafoste4");

    /* configure the W51xx ethernet controller prior to DHCP */
    unsigned char blank_addr[] = {0, 0, 0, 0};
    W5x_config(vpd.mac_address, blank_addr, blank_addr, blank_addr);

    /* loop until a dhcp address has been gotten */
    while (!dhcp_start(vpd.mac_address, 60000UL, 4000UL))
    {
    }
    uart_writestr("local ip: ");
    uart_writeip(dhcp_getLocalIp());

    /* configure the MAC, TCP, subnet and gateway addresses for the Ethernet controller*/
    W5x_config(vpd.mac_address, dhcp_getLocalIp(), dhcp_getGatewayIp(), dhcp_getSubnetMask());

    /* add a log record for EVENT_TIMESET prior to synchronizing with network time */
    log_add_record(EVENT_TIMESET);

    /* synchronize with network time */
    ntp_sync_network_time(5);

    /* add a log record for EVENT_NEWTIME now that time has been synchronized */
    log_add_record(EVENT_NEWTIME);

    /* start the watchdog timer */
    wdt_init();

    /* log the EVENT STARTUP and send and ALARM to the Master Controller */
    log_add_record(EVENT_STARTUP);
    alarm_send(EVENT_STARTUP);

    /* request start of test if 'T' key pressed - You may run up to 3 tests per
     * day.  Results will be e-mailed to you at the address asurite@asu.edu
     */
    check_for_test_start();

    /* start the first temperature reading and wait 5 seconds before reading again
    * this long delay ensures that the temperature spike during startup does not
    * trigger any false alarms.
    */
    temp_start();
    /* set delay and wait */
    delay_set(0, 5000);

    while (1)
    {
        /* reset  the watchdog timer every loop */
        wdt_reset();
        /* update the LED blink state */
        led_update();
        /* if the temperature sensor delay is done, update the current temperature
        * from the temperature sensor, update the temperature sensor finite state
        * machine (which provides hysteresis) and send any temperature sensor
        * alarms (from FSM update).
        */
        if (delay_isdone(0) && temp_is_data_ready())
        {
            /* read the temperature sensor */
            int tcurr = temp_get();
            /* update the temperature fsm and send any alarms associated with it */
            tempfsm_update(tcurr, config.hi_alarm, config.hi_warn, config.lo_alarm, config.lo_warn);
            /* restart the temperature sensor delay to trigger in 1 second */
            temp_start();
            delay_set(0, 1000);
        }
        /*if the server socket is closed */
        else if(socket_is_closed(SERVER_SOCKET))
        {
            /* if socket is closed, open it in passive (listen) mode */
            socket_open(SERVER_SOCKET, HTTP_PORT);
            socket_listen(SERVER_SOCKET);

        }
        /* if there is input to process */
        else if(socket_received_line(SERVER_SOCKET))
        {
            switch(state) {
                case WAITING:
                    /* change state */
                    state = PARSE_CMD;
                    break;
                case PARSE_CMD:
                    /* check if request type valid */
                    if (socket_recv_compare(SERVER_SOCKET, "DELETE")){
                        /* update cmd */
                        cmd = DELETE;
                        /* update state */
                        state = PARSE_ENDP;
                    }else if (socket_recv_compare(SERVER_SOCKET, "GET")){
                        /* update cmd */
                        cmd = GET;
                        /* update state */
                        state = PARSE_ENDP;
                    }else if (socket_recv_compare(SERVER_SOCKET, "PUT")){
                        /* update cmd */
                        cmd = PUT;
                        /* update state */
                        state = PARSE_ENDP;
                    }else{
                        state = ERR_REP;
                    }
                    break;
                case PARSE_ENDP:
                    /* advance until '/' */
                    while(!is_delimeter(ch)) socket_recv(SERVER_SOCKET, &ch, 1);
                    /* continue based on command */
                    switch(cmd) {
                        case DELETE:
                            /* if endp is device log */
                            if (socket_recv_compare(SERVER_SOCKET, "device/log ")) {
                                /* set flag */
                                endp = LOG;
                                /* set state */
                                state = PROCESS_REQ;
                            } else {
                                state = ERR_REP;
                            }
                            break;
                        case GET:
                            /* if endp is device */
                            if (socket_recv_compare(SERVER_SOCKET, "device ")) {
                                /* set flag */
                                endp = DEVICE;
                                /* set state */
                                state = PROCESS_REQ;
                            } else {
                                state = ERR_REP;
                            }
                            break;
                        case PUT:
                            /* if endp is device */
                            if (socket_recv_compare(SERVER_SOCKET, "device?")) {
                                /* setflag */
                                endp = DEVICE;
                                /* set state */
                                state = PARSE_PARAM;
                            }
                            /* else if endp is config */
                            else if (socket_recv_compare(SERVER_SOCKET, "device/config?")) {
                                /* set flag */
                                endp = CONFIG;
                                /* set state */
                                state = PARSE_PARAM;
                            } else {
                                state = ERR_REP;
                            }
                            break;
                        default:
                            state = ERR_REP;
                            break;
                    }
                    break;
                case PARSE_PARAM:
                    switch(endp) {
                        /* put in device */
                        case DEVICE:
                            if (socket_recv_compare(SERVER_SOCKET, "reset=\"true\"")) {
                                param = RESET;
                                state = PROCESS_REQ;
                            }
                            else {
                                state = ERR_REP;
                            }
                            break;
                        /* put in config */
                        case CONFIG:
                            if (socket_recv_compare(SERVER_SOCKET, "tcrit_hi=")) {
                                param = TC_HI;
                                state = PROCESS_REQ;
                            }
                            else if (socket_recv_compare(SERVER_SOCKET, "twarn_hi=")) {
                                param = TW_HI;
                                state = PROCESS_REQ;
                            }
                            else if (socket_recv_compare(SERVER_SOCKET, "tcrit_lo=")) {
                                param = TC_LO;
                                state = PROCESS_REQ;
                            }
                            else if (socket_recv_compare(SERVER_SOCKET, "twarn_lo=")) {
                                param = TW_LO;
                                state = PROCESS_REQ;
                            }
                            else {
                                state = ERR_REP;
                            }
                            break;
                        default:
                            state = ERR_REP;
                            break;
                    }
                    break;
                case PROCESS_REQ:
                    /* if GET && device */
                    if (cmd==GET && endp==DEVICE) {
                        state = SEND_REP;
                    }
                    /* else if PUT && device */
                    else if (cmd==PUT && endp==DEVICE) {
                        switch(param) {
                            case RESET:
                                wdt_force_restart();
                                break;
                            default:
                                state = ERR_REP;
                                break;
                        }
                    }
                    /* else if PUT && config */
                    else if (cmd == PUT && endp==CONFIG) {
                        int *t = 0;
                        switch(param) {
                            case TC_HI:
                                /* if int is command */
                                if(socket_recv_int(SERVER_SOCKET, t) && *t > config.hi_warn) {
                                    config.hi_alarm = *t;
                                    state = WAITING;
                                } else {
                                    state = ERR_REP;
                                }
                                break;
                            case TW_HI:
                                /* if int is command */
                                if(socket_recv_int(SERVER_SOCKET, t) && *t < config.hi_alarm && *t > config.lo_warn) {
                                    config.hi_warn = *t;
                                    state = WAITING;
                                } else {
                                    state = ERR_REP;
                                }
                                break;
                            case TC_LO:
                                /* if int is command */
                                if(socket_recv_int(SERVER_SOCKET, t) && *t < config.lo_warn) {
                                    config.lo_alarm = *t;
                                    state = WAITING;
                                } else {
                                    state = ERR_REP;
                                }
                                break;
                            case TW_LO:
                                /* if int is command */
                                if(socket_recv_int(SERVER_SOCKET, t) && *t < config.hi_warn && *t > config.lo_alarm) {
                                    config.lo_warn = *t;
                                    state = WAITING;
                                } else {
                                    state = ERR_REP;
                                }
                                break;
                            default:
                                state = ERR_REP;
                                break;
                        }
                    }
                    /* else if DELETE && log */
                    else if (cmd==DELETE && endp==LOG) {
                        log_clear();
                        state = WAITING;
                    }
                    /* respond */
                    if (state==WAITING) {
                        socket_writestr(SERVER_SOCKET, "OK\r\n");
                        socket_flush_line(SERVER_SOCKET);
                        socket_close(SERVER_SOCKET);
                    }
                    break;
                case SEND_REP:
                    /* OBJ START */
                    socket_writestr(SERVER_SOCKET, "{");
                    /* vpd */
                    socket_writequotedstring(SERVER_SOCKET, "vpd");
                    /* VPD START */
                    socket_writestr(SERVER_SOCKET, ":{");
                    /* model */
                    socket_writequotedstring(SERVER_SOCKET, "model");
                    socket_writestr(SERVER_SOCKET, ":");
                    socket_writequotedstring(SERVER_SOCKET, vpd.model);
                    socket_writestr(SERVER_SOCKET, ",");
                    /* manufacturer */
                    socket_writequotedstring(SERVER_SOCKET, "manufacturer");
                    socket_writestr(SERVER_SOCKET, ":");
                    socket_writequotedstring(SERVER_SOCKET, vpd.manufacturer);
                    socket_writestr(SERVER_SOCKET, ",");
                    /* serial number */
                    socket_writequotedstring(SERVER_SOCKET, "serial_number");
                    socket_writestr(SERVER_SOCKET, ":");
                    socket_writequotedstring(SERVER_SOCKET, vpd.serial_number);
                    socket_writestr(SERVER_SOCKET, ",");
                    /* manufacture date */
                    socket_writequotedstring(SERVER_SOCKET, "manufacture_date");
                    socket_writestr(SERVER_SOCKET, ":");
                    socket_writedate(SERVER_SOCKET, vpd.manufacture_date);
                    socket_writestr(SERVER_SOCKET, ",");
                    /* mac address */
                    socket_writequotedstring(SERVER_SOCKET, "mac_address");
                    socket_writestr(SERVER_SOCKET, ":");
                    socket_write_macaddress(SERVER_SOCKET, vpd.mac_address);
                    socket_writestr(SERVER_SOCKET, ",");
                    /* country code */
                    socket_writequotedstring(SERVER_SOCKET, "countr_code");
                    socket_writestr(SERVER_SOCKET, ":");
                    socket_writequotedstring(SERVER_SOCKET, vpd.country_of_origin);
                    /* VPD END */
                    socket_writestr(SERVER_SOCKET, "},");
                    /* tc_hi */
                    socket_writequotedstring(SERVER_SOCKET, "tcrit_hi");
                    socket_writestr(SERVER_SOCKET, ":");
                    socket_writedec32(SERVER_SOCKET, config.hi_alarm);
                    socket_writestr(SERVER_SOCKET, ",");
                    /* tw_hi */
                    socket_writequotedstring(SERVER_SOCKET, "twarn_hi");
                    socket_writestr(SERVER_SOCKET, ":");
                    socket_writedec32(SERVER_SOCKET, config.hi_warn);
                    socket_writestr(SERVER_SOCKET, ",");
                    /* tc_lo */
                    socket_writequotedstring(SERVER_SOCKET, "tcrit_lo");
                    socket_writestr(SERVER_SOCKET, ":");
                    socket_writedec32(SERVER_SOCKET, config.lo_alarm);
                    socket_writestr(SERVER_SOCKET, ",");
                    /* tw_lo */
                    socket_writequotedstring(SERVER_SOCKET, "twarn_lo");
                    socket_writestr(SERVER_SOCKET, ":");
                    socket_writedec32(SERVER_SOCKET, config.lo_warn);
                    socket_writestr(SERVER_SOCKET, ",");
                    /* temperature */
                    socket_writequotedstring(SERVER_SOCKET, "temperature");
                    socket_writestr(SERVER_SOCKET, ":");
                    socket_writedec32(SERVER_SOCKET, temp_get());
                    socket_writestr(SERVER_SOCKET, ",");
                    /* log */
                    socket_writequotedstring(SERVER_SOCKET, "log");
                    /* LOG START */
                    socket_writestr(SERVER_SOCKET, ":[");
                    /* for each entry */
                    for(unsigned long i=0; i < log_get_num_entries(); i++) {
                        unsigned long *tstamp = 0;
                        unsigned char *event = 0;
                        log_get_record(i, tstamp, event);
                        /* ENTRY START */
                        socket_writestr(SERVER_SOCKET, "{");
                        /* timestamp */
                        socket_writequotedstring(SERVER_SOCKET, "timestamp");
                        socket_writestr(SERVER_SOCKET, ":");
                        socket_writedate(SERVER_SOCKET, *tstamp);
                        socket_writestr(SERVER_SOCKET, ",");
                        /* event */
                        socket_writequotedstring(SERVER_SOCKET, "event");
                        socket_writestr(SERVER_SOCKET, ":");
                        socket_writedec32(SERVER_SOCKET, *event);
                        /* ENTRY END */
                        socket_writestr(SERVER_SOCKET, "}");
                        if (i+1 != log_get_num_entries()) socket_writestr(SERVER_SOCKET, ",");
                    }
                    /* END LOG */
                    socket_writestr(SERVER_SOCKET, "]");
                    /* END OBJ */
                    socket_writestr(SERVER_SOCKET, "}");
                    /* crlf */
                    socket_writestr(SERVER_SOCKET, "\r\n");
                    state = WAITING;
                    socket_close(SERVER_SOCKET);
                    break;
                case ERR_REP:
                    /* flush socket */
                    socket_flush_line(SERVER_SOCKET);
                    /* notify user */
                    socket_writestr(SERVER_SOCKET, "INVALID\r\n");
                    /* change state */
                    state = WAITING;
                    socket_close(SERVER_SOCKET);
                    break;
                default:
                    state = WAITING;
                    socket_close(SERVER_SOCKET);
                    break;
            }
        }
        /* otherwise... */
        else
        {
            /* update any pending log write backs */
            log_update();
            /* update any pending config write backs */
            config_update();
        }
    }
    return 0;
}
