#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdbool.h>
#include <poll.h>
#include "canlib_device.h"

#define GEN_STR_LEN   (128)
#define GEN_LABEL_LEN (256)
#define MAX_N_FIELDS  (1024)
#define SEPARATOR_LEN (120)
#define SUBWIN_WIDTH  (80)
#define SUBWIN_HEIGHT (7)

#define WRITE_CENTERED(screen, y, string, focus_id)                       \
    getmaxyx(screen, _scrheight, _scrwidth);                              \
    _is_focused = (focus_id >= 0) && (focus_id == current_focus);         \
    if (_is_focused) {                                                    \
        wattrset(screen, COLOR_PAIR(FOCUSED_ELEM_THEME));                 \
    }                                                                     \
    mvwprintw(screen, y, (_scrwidth - strlen(string)) / 2, "%s", string); \
    if (_is_focused) {                                                    \
        wattrset(screen, COLOR_PAIR(MAIN_THEME));                         \
    }

#define PRINT_INDICATIONS(screen)                                                             \
    const char indications_row_1[] = "Use up and down arrows to browse around";               \
    const char indications_row_2[] = "P to use primary network, I to use inverter network";   \
    const char indications_row_3[] = "O to use secondary network, U to use bms newtork";      \
    const char indications_row_4[] = "S to search for a message, E to edit the current field when a message is chosen";  \
    const char indications_row_5[] = "M to go to menu, H to select past sent a past message"; \
    const char indications_row_6[] = "D to go to can dump menu";                              \
    const char indications_row_7[] = "ENTER to select one message to send, Q to quit";        \
    current_row += 2;                                                                         \
    WRITE_CENTERED(screen, current_row, indications_row_1, -1);                               \
    current_row++;                                                                            \
    WRITE_CENTERED(screen, current_row, indications_row_2, -1);                               \
    current_row++;                                                                            \
    WRITE_CENTERED(screen, current_row, indications_row_3, -1);                               \
    current_row++;                                                                            \
    WRITE_CENTERED(screen, current_row, indications_row_4, -1);                               \
    current_row++;                                                                            \
    WRITE_CENTERED(screen, current_row, indications_row_5, -1);                               \
    current_row++;                                                                            \
    WRITE_CENTERED(screen, current_row, indications_row_6, -1);                               \
    current_row++;                                                                            \
    WRITE_CENTERED(screen, current_row, indications_row_7, -1);                               \
    current_row += 3

#include "custom_sender_config.h"
#include "inverters/inverters_network.h"
#include "inverters/inverters_utils.h"
#include "primary/primary_network.h"
#include "primary/primary_utils.h"
#include "secondary/secondary_network.h"
#include "secondary/secondary_utils.h"
#include "bms/bms_network.h"
#include "bms/bms_utils.h"

#include <ncurses.h>

typedef struct {
    uint16_t id;
    uint8_t size;
    uint8_t data[8];
} can_message_t;

typedef struct {
    char *msg_name;
    size_t n_fields;
    int *fields_type;
    char **fields_name;
} can_msg_metadata_t;

typedef struct {
    bool rec;
    struct can_frame data;
} can_message_log_t;

extern int scrwidth, scrheight;
extern enum application_tabs_t ctab;
extern int (*render_menus[n_application_tabs])(int, void *);
extern int can_sockets[N_CAN_DEVICES];
extern struct pollfd poll_fds[N_CAN_DEVICES+1];
extern char can_devices[N_CAN_DEVICES][GEN_STR_LEN];

extern struct can_frame candump_data;
extern struct can_frame can_selected_msg;

extern device_t rxdev;

extern can_message_log_t msg_log[primary_MESSAGE_COUNT + secondary_MESSAGE_COUNT + inverters_MESSAGE_COUNT + bms_MESSAGE_COUNT];
extern int msg_log_count;

extern int idx_offset;

int open_can_socket(const char *device);

int init_canlib_metadata(void);
int retrieve_input_prompt(int current_focus);
int action(int current_focus);
int current_focus_dec(int current_focus);
int current_focus_inc(int current_focus);

void fatal_error(const char *error_msg);

int render_can_dump_msg(int current_focus, void* data);
int render_can_msg_data_fields(int current_focus);
int get_can_msg_index_from_id(int id);

#endif  // GRAPHICS_H
