#ifndef CUSTOM_SENDER_CONFIG_H
#define CUSTOM_SENDER_CONFIG_H

#include <assert.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <math.h>
#include <net/if.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

#define N_CAN_DEVICES 2
/****
 * Choose one of the two following CAN_DEVICES configurations
*/
#define CAN_DEVICES { "can0", "can0" }
    //{ "vcan0", "vcan1" }

#define MAIN_THEME         1
#define TITLE_THEME        2
#define FOCUSED_ELEM_THEME 3

#define MIN_WIDTH  (70u)
#define MIN_HEIGHT (25u)

#define ESCAPE_KEY (27)

#define clamp(x, a, b) (((x) < (a)) ? (a) : (((x) > (b)) ? (b) : (x)))

enum application_tabs_t { main_menu, fill_fields_menu, search_menu, can_dump, can_msg, search_menu_rec, n_application_tabs};


enum interfaces_t { primary_intf, secondary_intf, inverter_intf, bms_intf, n_intf };

#endif  // CUSTOM_SENDER_CONFIG_H
