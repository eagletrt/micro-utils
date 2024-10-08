#include "custom_sender.h"

struct sockaddr_can address                  = {0};
int can_sockets[N_CAN_DEVICES]               = {0};
char can_devices[N_CAN_DEVICES][GEN_STR_LEN] = CAN_DEVICES;

struct pollfd poll_fds[N_CAN_DEVICES+1];
struct can_frame candump_data;
can_message_log_t msg_log[primary_MESSAGE_COUNT + secondary_MESSAGE_COUNT + inverters_MESSAGE_COUNT + bms_MESSAGE_COUNT];
struct can_frame can_selected_msg;
int msg_log_count;

device_t rxdev;

int idx_offset;

int open_can_socket(const char *device) {
    struct ifreq ifr;
    int can_socket = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (can_socket < 0) {
        return -1;
    }
    strcpy(ifr.ifr_name, device);
    ioctl(can_socket, SIOCGIFINDEX, &ifr);
    address.can_family                    = AF_CAN;
    address.can_ifindex                   = ifr.ifr_ifindex;
    const struct sockaddr *sockaddr_param = (struct sockaddr *)&(address);
    if (bind(can_socket, sockaddr_param, sizeof(address)) < 0) {
        return -2;
    }
    return can_socket;
}

bool close_socket(int can_socket) {
    if (close(can_socket) < 0) {
        fatal_error("Error closing can socket");
        return false;
    }
    return true;
}

int can_send(can_message_t *msg, int can_socket) {
    if (msg->size < 0 || msg->size > 8)
        return -1;
    struct can_frame frame;
    frame.can_id  = msg->id;
    frame.can_dlc = msg->size;
    memcpy(frame.data, msg->data, msg->size * sizeof(char));
    return write(can_socket, &frame, sizeof(frame));
}

int _scrwidth                                  = 0;
int _scrheight                                 = 0;
enum application_tabs_t ctab                   = main_menu;
int chosen_msg_idx                             = -1;
enum interfaces_t chosen_interface             = -1;
char current_fields[MAX_N_FIELDS][GEN_STR_LEN] = {0};
int current_n_fields                           = -1;
bool _is_focused                               = false;

extern int n_shown_msgs;
extern enum interfaces_t chosen_intf;

can_msg_metadata_t metadata_msgs[primary_MESSAGE_COUNT + secondary_MESSAGE_COUNT + inverters_MESSAGE_COUNT + bms_MESSAGE_COUNT];

void fatal_error(const char *error_msg) {
    endwin();
    fprintf(stderr, "[ERROR]: %s\n", error_msg);
    exit(1);
}

int get_intf_tot_msg() {
    switch (chosen_intf) {
        case primary_intf:
            return primary_MESSAGE_COUNT;
        case secondary_intf:
            return secondary_MESSAGE_COUNT;
        case inverter_intf:
            return inverters_MESSAGE_COUNT;
        case bms_intf:
            return bms_MESSAGE_COUNT;
        default:
            return 0;
    }
}

int get_intf_base_idx() {
    int base = 0U;
    switch (chosen_intf) {
        case bms_intf:
            base += inverters_MESSAGE_COUNT;
            ; // Missing break is intentional
        case inverter_intf:
            base += secondary_MESSAGE_COUNT;
            ; // Missing break is intentional
        case secondary_intf:
            base += primary_MESSAGE_COUNT;
            ; // Missing break is intentional
        default:
            return base;
    }
}

int get_idx_from_intf(int bidx, enum interfaces_t intf) {
    int base = bidx;
    switch (intf) {
        case bms_intf:
            base += inverters_MESSAGE_COUNT;
            ; // Missing break is intentional
        case inverter_intf:
            base += secondary_MESSAGE_COUNT;
            ; // Missing break is intentional
        case secondary_intf:
            base += primary_MESSAGE_COUNT;
            ; // Missing break is intentional
        default:
            return base;
    }
}

int init_canlib_metadata(void) {
    for (size_t iidx = 0; iidx < primary_MESSAGE_COUNT; iidx++) {
        uint16_t id                  = primary_id_from_index(iidx);
        if (id == 0xFF) {
            fatal_error("Could not retrieve primary message id from index");
        }
        int imsg                     = get_idx_from_intf(iidx, primary_intf);
        int n_field                  = primary_n_fields_from_id(id);
        metadata_msgs[imsg].n_fields = n_field;
        metadata_msgs[imsg].msg_name = (char *)malloc(GEN_STR_LEN * sizeof(char));
        if (!primary_message_name_from_id(id, metadata_msgs[imsg].msg_name)) {
            fprintf(stderr, "Could not retrieve primary message name from id %d\n", (int)id);
            return 0;
        }
        metadata_msgs[imsg].fields_type = (int *)malloc(n_field * sizeof(int));
        if (n_field > 0) {
            if (primary_fields_types_from_id(id, metadata_msgs[imsg].fields_type, n_field) == 0) {
                fprintf(stderr, "Could not retrieve primary fields types from id %d\n", (int)id);
                return 0;
            }
            metadata_msgs[imsg].fields_name = (char **)malloc(n_field * sizeof(char *));
            for (size_t ifield = 0; ifield < n_field; ifield++) {
                metadata_msgs[imsg].fields_name[ifield] = (char *)malloc(GEN_STR_LEN * sizeof(char));
            }
            if (primary_fields_string_from_id(id, (char **)metadata_msgs[imsg].fields_name, n_field, GEN_STR_LEN) ==
                1) {
                fprintf(stderr, "Could not retrieve primary string types from id %d\n", (int)id);
                return 0;
            }
        }
    }
    for (size_t iidx = 0; iidx < secondary_MESSAGE_COUNT; iidx++) {
        uint16_t id = secondary_id_from_index(iidx);
        if (id == 0xFF) {
            fatal_error("Could not retrieve secondary message id from index");
        }
        int imsg                     = get_idx_from_intf(iidx, secondary_intf);
        int n_field                  = secondary_n_fields_from_id(id);
        metadata_msgs[imsg].n_fields = n_field;
        metadata_msgs[imsg].msg_name = (char *)malloc(GEN_STR_LEN * sizeof(char));
        if (!secondary_message_name_from_id(id, metadata_msgs[imsg].msg_name)) {
            fprintf(stderr, "Could not retrieve secondary message name from id %d\n", (int)id);
            return 0;
        }
        metadata_msgs[imsg].fields_type = (int *)malloc(n_field * sizeof(int));
        if (n_field > 0) {
            if (secondary_fields_types_from_id(id, metadata_msgs[imsg].fields_type, n_field) == 0) {
                fprintf(stderr, "Could not retrieve secondary fields types from id %d\n", (int)id);
                return 0;
            }
            metadata_msgs[imsg].fields_name = (char **)malloc(n_field * sizeof(char *));
            for (size_t ifield = 0; ifield < n_field; ifield++) {
                metadata_msgs[imsg].fields_name[ifield] = (char *)malloc(GEN_STR_LEN * sizeof(char));
            }
            if (secondary_fields_string_from_id(id, (char **)metadata_msgs[imsg].fields_name, n_field, GEN_STR_LEN) ==
                1) {
                fprintf(stderr, "Could not retrieve secondary string types from id %d\n", (int)id);
                return 0;
            }
        }
    }
    for (size_t iidx = 0; iidx < inverters_MESSAGE_COUNT; iidx++) {
        uint16_t id = inverters_id_from_index(iidx);
        if (id == 0xFF) {
            fatal_error("Could not retrieve inverter message id from index");
        }
        int imsg                     = get_idx_from_intf(iidx, inverter_intf);
        int n_field                  = inverters_n_fields_from_id(id);
        metadata_msgs[imsg].n_fields = n_field;
        metadata_msgs[imsg].msg_name = (char *)malloc(GEN_STR_LEN * sizeof(char));
        if (!inverters_message_name_from_id(id, metadata_msgs[imsg].msg_name)) {
            fprintf(stderr, "Could not retrieve inverters message name from id %d\n", (int)id);
            return 0;
        }
        metadata_msgs[imsg].fields_type = (int *)malloc(n_field * sizeof(int));
        if (n_field > 0) {
            if (inverters_fields_types_from_id(id, metadata_msgs[imsg].fields_type, n_field) == 0) {
                fprintf(stderr, "Could not retrieve inverters fields types from id %d\n", (int)id);
                return 0;
            }
            metadata_msgs[imsg].fields_name = (char **)malloc(n_field * sizeof(char *));
            for (size_t ifield = 0; ifield < n_field; ifield++) {
                metadata_msgs[imsg].fields_name[ifield] = (char *)malloc(GEN_STR_LEN * sizeof(char));
            }
            if (inverters_fields_string_from_id(id, (char **)metadata_msgs[imsg].fields_name, n_field, GEN_STR_LEN) ==
                1) {
                fprintf(stderr, "Could not retrieve inverters string types from id %d\n", (int)id);
                return 0;
            }
        }
    }
    for (size_t iidx = 0; iidx < bms_MESSAGE_COUNT; iidx++) {
        uint16_t id = bms_id_from_index(iidx);
        if (id == 0xFF) {
            fatal_error("Could not retrieve bms message id from index");
        }
        int imsg                     = get_idx_from_intf(iidx, bms_intf);
        int n_field                  = bms_n_fields_from_id(id);
        metadata_msgs[imsg].n_fields = n_field;
        metadata_msgs[imsg].msg_name = (char *)malloc(GEN_STR_LEN * sizeof(char));
        if (!bms_message_name_from_id(id, metadata_msgs[imsg].msg_name)) {
            fprintf(stderr, "Could not retrieve bms message name from id %d\n", (int)id);
            return 0;
        }
        metadata_msgs[imsg].fields_type = (int *)malloc(n_field * sizeof(int));
        if (n_field > 0) {
            if (bms_fields_types_from_id(id, metadata_msgs[imsg].fields_type, n_field) == 0) {
                fprintf(stderr, "Could not retrieve bms fields types from id %d\n", (int)id);
                return 0;
            }
            metadata_msgs[imsg].fields_name = (char **)malloc(n_field * sizeof(char *));
            for (size_t ifield = 0; ifield < n_field; ifield++) {
                metadata_msgs[imsg].fields_name[ifield] = (char *)malloc(GEN_STR_LEN * sizeof(char));
            }
            if (bms_fields_string_from_id(id, (char **)metadata_msgs[imsg].fields_name, n_field, GEN_STR_LEN) ==
                1) {
                fprintf(stderr, "Could not retrieve bms string types from id %d\n", (int)id);
                return 0;
            }
        }
    }
    return 1;
}

void fill_data(can_message_t *msg, char *final_command, const char *device) {
    snprintf(final_command, BUFSIZ, "cansend %s %03X#", device, msg->id);
    for (size_t ic = 0; ic < msg->size; ic++) {
        int clen = strlen(final_command);
        snprintf(final_command + clen, BUFSIZ - clen, "%02X", msg->data[ic]);
    }
}

void send_message(int current_focus) {
    int msgid         = -1;
    can_message_t msg = {0};
    char to_serialize[BUFSIZ];
    char final_command[BUFSIZ];
    int can_send_retval          = 0;
    int current_to_serialize_idx = 0;
    if (current_n_fields > 0) {
        int field_len = strlen(current_fields[0]);
        memcpy(to_serialize + current_to_serialize_idx, current_fields[0], field_len);
        current_to_serialize_idx += field_len;
    }
    for (size_t ifield = 1; ifield < current_n_fields; ifield++) {
        int field_len = strlen(current_fields[ifield]);
        memcpy(to_serialize + current_to_serialize_idx, ",", 1);
        current_to_serialize_idx++;
        memcpy(to_serialize + current_to_serialize_idx, current_fields[ifield], field_len);
        current_to_serialize_idx += field_len;
    }
    switch (chosen_intf) {
        case primary_intf: {
            msgid         = primary_id_from_index(chosen_msg_idx);
            msg.id        = msgid;
            size_t msg_sz = 0;
            primary_serialize_from_string(msgid, to_serialize, msg.data, &msg_sz);
            msg.size = (uint8_t)msg_sz;
            fill_data(&msg, (char *)final_command, (char *)can_devices[0]);
            can_send_retval = can_send(&msg, can_sockets[0]);
            break;
        }
        case secondary_intf: {
            msgid         = secondary_id_from_index(chosen_msg_idx - get_intf_base_idx());
            msg.id        = msgid;
            size_t msg_sz = 0;
            secondary_serialize_from_string(msgid, to_serialize, msg.data, &msg_sz);
            msg.size = (uint8_t)msg_sz;
            fill_data(&msg, (char *)final_command, (char *)can_devices[1]);
            can_send_retval = can_send(&msg, can_sockets[1]);
            break;
        }
        case inverter_intf: {
            msgid         = inverters_id_from_index(chosen_msg_idx - get_intf_base_idx());
            msg.id        = msgid;
            size_t msg_sz = 0;
            inverters_serialize_from_string(msgid, to_serialize, msg.data, &msg_sz);
            msg.size = (uint8_t)msg_sz;
            fill_data(&msg, (char *)final_command, (char *)can_devices[0]);
            can_send_retval = can_send(&msg, can_sockets[0]);
            break;
        }
        case bms_intf: {
            msgid         = bms_id_from_index(chosen_msg_idx);
            msg.id        = msgid;
            size_t msg_sz = 0;
            bms_serialize_from_string(msgid, to_serialize, msg.data, &msg_sz);
            msg.size = (uint8_t)msg_sz;
            fill_data(&msg, (char *)final_command, (char *)can_devices[0]);
            can_send_retval = can_send(&msg, can_sockets[0]);
            break;
        }
        default:
            // TODO: Handle invalid networks
            break;
    }
    WINDOW *prompt_win =
        newwin(SUBWIN_HEIGHT, SUBWIN_WIDTH, (scrheight - SUBWIN_HEIGHT) / 2, (scrwidth - SUBWIN_WIDTH) / 2);
    if (prompt_win == NULL) {
        assert(0 && "Could not create a input prompt window");
    }
    wclear(prompt_win);
    box(prompt_win, 0, 0);
    int current_row = 1;
    if (can_send_retval < 0) {
        WRITE_CENTERED(prompt_win, current_row, "[ERROR]: Could not send message", -1);
    } else {
        WRITE_CENTERED(prompt_win, current_row, "Message was sent successfully", -1);
    }
    current_row++;
    WRITE_CENTERED(prompt_win, current_row, "Manual cansend command [COULD NOT WORK]", -1);
    current_row++;
    WRITE_CENTERED(prompt_win, current_row, final_command, -1);
    current_row++;
    wrefresh(prompt_win);
    getch();
    delwin(prompt_win);
}

int action(int current_focus) {
    switch (ctab) {
        case search_menu:
            // chosen_msg_idx = current_msg_idx + get_intf_base_idx(); // chosen_msg_idx already set
            chosen_intf      = chosen_intf;
            current_n_fields = metadata_msgs[chosen_msg_idx].n_fields;
            ctab             = fill_fields_menu;
            for (size_t ifield = 0; ifield < MAX_N_FIELDS; ifield++) {
                memcpy(current_fields[ifield], "0", 2);
            }
            return 0;
        case main_menu:
            chosen_msg_idx   = current_focus + get_intf_base_idx();
            chosen_interface = chosen_intf;
            current_n_fields = metadata_msgs[chosen_msg_idx].n_fields;
            ctab             = fill_fields_menu;
            for (size_t ifield = 0; ifield < MAX_N_FIELDS; ifield++) {
                memcpy(current_fields[ifield], "0", 2);
            }
            return 0;
        case fill_fields_menu:
            if (current_focus < current_n_fields) {
                retrieve_input_prompt(current_focus);
            } else {
                send_message(current_focus);
                chosen_msg_idx   = -1;
                if (chosen_msg_idx == inverter_intf) {
                    current_focus = 0;
                }
                chosen_interface = primary_intf;
                current_n_fields = -1;
                ctab             = main_menu;
                return current_focus;
            }
            break;
        case can_dump: { 
                ctab = can_msg;
                return current_focus;
            }
        default:
            // TODO: Handle invalid networks
            return current_focus;
    }
}

int current_focus_dec(int current_focus) {
    switch (ctab) {
        case search_menu:
        case main_menu:
            return clamp(current_focus - 1, 0, get_intf_tot_msg() - 1);
        case fill_fields_menu:
            return clamp(current_focus - 1, 0, metadata_msgs[chosen_msg_idx].n_fields);
        case can_dump:
            return clamp(current_focus - 1, 0, msg_log_count-1);
        case can_msg:
            return clamp(current_focus - 1, 0, metadata_msgs[primary_index_from_id(can_selected_msg.can_id)].n_fields-1);
        default:
            return current_focus;
    }
}

int current_focus_inc(int current_focus) {
    switch (ctab) {
        case search_menu:
        case main_menu:
            return clamp(current_focus + 1, 0, get_intf_tot_msg() - 1);
        case fill_fields_menu:
            return clamp(current_focus + 1, 0, metadata_msgs[chosen_msg_idx].n_fields);
        case can_dump:
            return clamp(current_focus + 1, 0, msg_log_count-1);
        case can_msg:
            return clamp(current_focus + 1, 0, metadata_msgs[primary_index_from_id(can_selected_msg.can_id)].n_fields-1);
        default:
            return current_focus;
    }
}

int retrieve_input_prompt(int current_focus) {
    char input_prompt[GEN_STR_LEN] = {0};
    int cind = 0, input_enter = 0;
    WINDOW *prompt_win =
        newwin(SUBWIN_HEIGHT, SUBWIN_WIDTH, (scrheight - SUBWIN_HEIGHT) / 2, (scrwidth - SUBWIN_WIDTH) / 2);
    if (prompt_win == NULL) {
        assert(0 && "Could not create a input prompt window");
    }
    curs_set(1);

    while (cind < GEN_STR_LEN && !input_enter) {
        wclear(prompt_win);
        box(prompt_win, 0, 0);
        int current_row = 1;
        WRITE_CENTERED(prompt_win, current_row, "Enter the value for that field", -1);
        current_row++;
        WRITE_CENTERED(prompt_win, current_row, input_prompt, -1);
        wrefresh(prompt_win);
        int inchar = getch();
        switch (inchar) {
            case ESCAPE_KEY: {
                input_enter = true;
                break;
            }
            case KEY_ENTER:
            case '\n':
                input_enter = true;
                break;
            case KEY_BACKSPACE:
            case 127:
            case '\b':
                cind               = clamp(cind - 1, 0, cind);
                input_prompt[cind] = 0;
                break;
            default:
                input_prompt[cind] = inchar;
                cind++;
                break;
        }
    }
    curs_set(0);
    memcpy(current_fields[current_focus], input_prompt, GEN_STR_LEN);
    delwin(prompt_win);
    return 0U;
}

int render_main_menu(int current_focus, void *data) {
    bool is_main_menu = (data == NULL);

    wclear(stdscr);
    wattrset(stdscr, COLOR_PAIR(MAIN_THEME));
    int current_row = 0;
    box(stdscr, 0, 0);

    const char title[] = "CUSTOM MESSAGE SENDER MENU";
    wattrset(stdscr, COLOR_PAIR(TITLE_THEME));
    current_row++;
    WRITE_CENTERED(stdscr, current_row, title, -1);
    wattrset(stdscr, COLOR_PAIR(MAIN_THEME));
    if (is_main_menu) {
        PRINT_INDICATIONS(stdscr);
    } else {
        char *searching_string = (char *)data;
        current_row += 2;
        const char searching_tip_1[] = "You are searching for the following string";
        WRITE_CENTERED(stdscr, current_row, searching_tip_1, -1);
        current_row++;
        WRITE_CENTERED(stdscr, current_row, searching_string, -1);
        current_row += 2;
    }

    int intf_tot_msg      = get_intf_tot_msg();
    int intf_base_idx     = get_intf_base_idx();
    size_t actual_written = 0;

    for (size_t iidx = 0; actual_written < (n_shown_msgs) && (iidx + current_focus) < intf_tot_msg; iidx++) {
        int showed_msg = current_focus + iidx;
        char *msg_name = metadata_msgs[intf_base_idx + showed_msg].msg_name;
        if (is_main_menu) {
            WRITE_CENTERED(stdscr, current_row + iidx, msg_name, showed_msg);
            if (_is_focused) {
                chosen_msg_idx = intf_base_idx + showed_msg;
            }
            actual_written++;
        } else {
            char *searching_string = (char *)data;
            if (!(strstr(msg_name, searching_string) == NULL)) {
                WRITE_CENTERED(stdscr, current_row + actual_written, msg_name, actual_written);
                if (_is_focused) {
                    chosen_msg_idx = intf_base_idx + showed_msg;
                }
                actual_written++;
            }
        }
    }
    refresh();
    return 0U;
}

int render_fill_fields_menu(int current_focus, void *data) {
    wclear(stdscr);
    wattrset(stdscr, COLOR_PAIR(MAIN_THEME));
    box(stdscr, 0, 0);
    int current_row = 0;

    const char title[] = "MESSAGE FILL FIELDS MENU";
    wattrset(stdscr, COLOR_PAIR(TITLE_THEME));
    current_row++;
    WRITE_CENTERED(stdscr, current_row, title, -1);
    wattrset(stdscr, COLOR_PAIR(MAIN_THEME));
    PRINT_INDICATIONS(stdscr);

    char chosen_msg_label[GEN_LABEL_LEN]; 
    snprintf(chosen_msg_label, GEN_LABEL_LEN, "Chosen msg name: %s", metadata_msgs[chosen_msg_idx].msg_name); 
    wattrset(stdscr, COLOR_PAIR(TITLE_THEME)); 
    WRITE_CENTERED(stdscr, current_row, chosen_msg_label, -1);
    wattrset(stdscr, COLOR_PAIR(MAIN_THEME));
    current_row += 2;

    const char first_table_row[] = "Available fields";
    WRITE_CENTERED(stdscr, current_row, first_table_row, -1);
    current_row++;
    mvwhline(stdscr, current_row, (scrwidth - SEPARATOR_LEN) / 2, '~', SEPARATOR_LEN);
    current_row++;
    int n_fields = metadata_msgs[chosen_msg_idx].n_fields;
    for (size_t ifield = current_focus; ifield < n_fields && ifield < (10 + current_focus); ifield++) {
        char *field_name                = metadata_msgs[chosen_msg_idx].fields_name[ifield];
        int field_type                  = metadata_msgs[chosen_msg_idx].fields_type[ifield];
        char field_label[GEN_LABEL_LEN] = {0};
        snprintf(field_label, GEN_LABEL_LEN, "%s = %s", field_name, current_fields[ifield]);
        WRITE_CENTERED(stdscr, current_row, field_label, ifield);
        current_row++;
    }
    const char label_send_message[] = "Send this message";
    WRITE_CENTERED(stdscr, current_row, label_send_message, n_fields);
    current_row++;
    return 0;
}

int get_can_msg_index_from_id(int id) {
    int idx = -1;
    switch (chosen_intf) {
        case primary_intf:
            idx = primary_index_from_id(id);
            break;
        case secondary_intf:
            idx = secondary_index_from_id(id);
            break;
        case bms_intf:
            idx = bms_index_from_id(id);
            break;
        case inverter_intf:
            idx = inverters_index_from_id(id);
            break;
        default:
            break;
    }

    if(idx<0) {
        return -1;
    }

    return idx + get_intf_base_idx();
}


int render_can_msg_center(WINDOW* win, int row, struct can_frame* msg, int current_focus) {
    int msg_idx = get_can_msg_index_from_id(msg->can_id);

    if(msg_idx>=0) {
        char str[200];
        sprintf(str, "[0x%03x] %s", msg->can_id, metadata_msgs[msg_idx].msg_name);
        WRITE_CENTERED(win, row, "                                          ", -1);
        WRITE_CENTERED(win, row, str, row-12);

        if(row-12 == current_focus)
            can_selected_msg = *msg;
        return 0;
    }

    return -1;
}

int render_can_dump(int current_focus, void *data) {
    wclear(stdscr);
    wattrset(stdscr, COLOR_PAIR(MAIN_THEME));
    box(stdscr, 0, 0);
    int current_row = 0;

    const char title[] = "CAN DUMP MENU";
    wattrset(stdscr, COLOR_PAIR(TITLE_THEME));
    current_row++;
    WRITE_CENTERED(stdscr, current_row, title, -1);
    wattrset(stdscr, COLOR_PAIR(MAIN_THEME));

    PRINT_INDICATIONS(stdscr);

    render_can_dump_msg(current_focus, data);

    refresh();

    return 0;
}

int render_can_dump_msg(int current_focus, void* data) {
    int current_row = 12;
    int msg_count=0;
    int msg_invalid = 0;
    for(size_t i=0; i<primary_MESSAGE_COUNT + secondary_MESSAGE_COUNT + inverters_MESSAGE_COUNT + bms_MESSAGE_COUNT; i++) {
        if(msg_log[i].rec) {
            if(msg_count>=current_focus) {
                render_can_msg_center(stdscr, current_row++, &msg_log[i].data, current_focus-msg_count);
            }
            msg_count++;
        }
    }

    return 0;
}

int render_can_msg_data_fields(int current_focus) {
    
    int msg_idx = get_can_msg_index_from_id(can_selected_msg.can_id);
    if(msg_idx<0)
        return -1;

    uint8_t buf[1024];
    uint8_t raw[1024];
    char msg_params[1024];
    device_set_address(&rxdev, raw, 1024, buf, 1024);

    switch (chosen_intf) {
        case primary_intf:
            primary_devices_deserialize_from_id(&rxdev, can_selected_msg.can_id, can_selected_msg.data);
            primary_to_string_from_id(can_selected_msg.can_id, rxdev.message, msg_params);
            break;
        case secondary_intf:
            secondary_devices_deserialize_from_id(&rxdev, can_selected_msg.can_id, can_selected_msg.data);
            secondary_to_string_from_id(can_selected_msg.can_id, rxdev.message, msg_params);
            break;
        case bms_intf:
            bms_devices_deserialize_from_id(&rxdev, can_selected_msg.can_id, can_selected_msg.data);
            bms_to_string_from_id(can_selected_msg.can_id, rxdev.message, msg_params);
            break;
        case inverter_intf:
            inverters_devices_deserialize_from_id(&rxdev, can_selected_msg.can_id, can_selected_msg.data);
            inverters_to_string_from_id(can_selected_msg.can_id, rxdev.message, msg_params);
            break;
        default:
            break;
    }

    char *token, *ptr, *tmpstr = msg_params;
    int msg_count = 0;
    int current_row = 12;
    for(size_t i=0; i<metadata_msgs[msg_idx].n_fields; i++, tmpstr=NULL) {
        token = strtok_r(tmpstr, ",", (char** restrict)&ptr);
        if(msg_count>=current_focus) {
            char msg_str[1024];
            sprintf(
                    msg_str, 
                    "%s: %s", 
                    metadata_msgs[msg_idx].fields_name[i]+strlen(metadata_msgs[msg_idx].msg_name)+1,
                    token
                );
            WRITE_CENTERED(stdscr, current_row, "                                          ", -1);
            WRITE_CENTERED(stdscr, current_row++, msg_str, msg_count);
        }
        msg_count++;

    }
}

int render_can_msg_data(int current_focus, void* data) {
    wclear(stdscr);
    wattrset(stdscr, COLOR_PAIR(MAIN_THEME));
    box(stdscr, 0, 0);
    int current_row = 0;

    int msg_idx = get_can_msg_index_from_id(can_selected_msg.can_id);

    wattrset(stdscr, COLOR_PAIR(TITLE_THEME));
    current_row++;
    WRITE_CENTERED(stdscr, current_row, metadata_msgs[msg_idx].msg_name, -1);
    wattrset(stdscr, COLOR_PAIR(MAIN_THEME));

    PRINT_INDICATIONS(stdscr);

    render_can_msg_data_fields(current_focus);

    refresh();

    return 0;
}

int (*render_menus[n_application_tabs])(int, void *) = {render_main_menu, render_fill_fields_menu, render_main_menu, render_can_dump, render_can_msg_data, render_can_dump};
