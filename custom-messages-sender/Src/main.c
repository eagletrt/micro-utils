#include "custom_sender.h"

int scrwidth = 0, scrheight = 0;
enum interfaces_t chosen_intf = primary_intf;
int n_shown_msgs              = 20;

static_assert(N_CAN_DEVICES == (sizeof(can_devices) / GEN_STR_LEN));

int main(int argc, char const *argv[]) {
    /* 
        primary_lv_current_battery_converted_t converted = { .lv_current = 10.0f };
        primary_lv_current_battery_t raw;
        primary_lv_current_battery_conversion_to_raw_struct(&raw, &converted);
        uint8_t msg[8];
        primary_lv_current_battery_pack(msg, &raw, PRIMARY_LV_CURRENT_BATTERY_BYTE_SIZE);
        primary_lv_current_battery_unpack(&raw, msg, PRIMARY_LV_CURRENT_BATTERY_BYTE_SIZE);
        primary_lv_current_battery_raw_to_conversion_struct(&converted, &raw);
        printf("%f\r\n", converted.lv_current);
        assert(converted.lv_current == 10.0f);
     */
    initscr();
    
    if (!init_canlib_metadata()) {
        fatal_error("Failed to initialize canlib metadata");
    }
    for (size_t icandev = 0; icandev < N_CAN_DEVICES; icandev++) {
        int retval = open_can_socket(can_devices[icandev]);
        if (retval < 0) {
            fatal_error("Failed to open one of the can devices");
        }
        can_sockets[icandev] = retval;
    }
    keypad(stdscr, TRUE);
    curs_set(0);
    // nodelay(stdscr, 1);
    // cbreak();
    set_escdelay(0);
    noecho();
    start_color();

    getmaxyx(stdscr, scrheight, scrwidth);
    if (scrheight < MIN_HEIGHT || scrwidth < MIN_WIDTH) {
        fatal_error("Terminal too small, could not render properly the interface");
    }
    int rows_occupied = 13;
    n_shown_msgs      = scrheight - rows_occupied;

    init_pair(MAIN_THEME, COLOR_WHITE, COLOR_BLACK);
    init_pair(TITLE_THEME, COLOR_BLACK, COLOR_GREEN);
    init_pair(FOCUSED_ELEM_THEME, COLOR_BLACK, COLOR_YELLOW);

    int current_focus             = 0;
    int searching_buffer_idx      = 0;
    char searching_buffer[BUFSIZ] = {0};

    bool quit = false;
    while (!quit) {
        render_menus[ctab](current_focus, (ctab != search_menu) ? NULL : (void *)searching_buffer);
        int user_input = getch();

        switch (ctab) {
            case fill_fields_menu:
            case main_menu: {
                switch (user_input) {
                    case 'q': {
                        quit = true;
                        break;
                    }
                    case 'e': {
                        if (ctab == fill_fields_menu) {
                            retrieve_input_prompt(current_focus);
                        }
                        break;
                    }
                    case 'o':
                        if (ctab == main_menu) {
                            current_focus = 0;
                            chosen_intf   = secondary_intf;
                        }
                        break;
                    case 'p':
                        if (ctab == main_menu) {
                            current_focus = 0;
                            chosen_intf   = primary_intf;
                        }
                        break;
                    case 'i':
                        if (ctab == main_menu) {
                            current_focus = 0;
                            chosen_intf   = inverter_intf;
                        }
                        break;
                    case ESCAPE_KEY:
                    case 'm':
                        ctab = main_menu;
                        clear();
                        refresh();
                        break;
                    case KEY_UP:
                        current_focus = current_focus_dec(current_focus);
                        break;
                    case KEY_DOWN:
                        current_focus = current_focus_inc(current_focus);
                        break;
                    case 's': {
                        current_focus = 0;
                        ctab          = search_menu;
                        break;
                    }
                    case '\n':
                        current_focus = action(current_focus);
                        break;
                    default:
                        break;
                }
                break;
            }
            case search_menu: {
                switch (user_input) {
                    case ESCAPE_KEY: {
                        memset(searching_buffer, 0, BUFSIZ);
                        searching_buffer_idx = 0;
                        ctab                 = main_menu;
                        clear();
                        refresh();
                        break;
                    }
                    case KEY_BACKSPACE: {
                        searching_buffer_idx--;
                        searching_buffer_idx                   = clamp(searching_buffer_idx, 0, BUFSIZ - 1);
                        searching_buffer[searching_buffer_idx] = 0;
                        break;
                    }
                    case KEY_UP:
                        current_focus = current_focus_dec(current_focus);
                        break;
                    case KEY_DOWN:
                        current_focus = current_focus_inc(current_focus);
                        break;
                    case KEY_ENTER:
                    case '\n':
                        current_focus = action(current_focus);
                        break;
                    default: {
                        current_focus                          = 0;
                        searching_buffer[searching_buffer_idx] = user_input;
                        searching_buffer_idx++;
                        searching_buffer_idx = clamp(searching_buffer_idx, 0, BUFSIZ - 1);
                        break;
                    }
                }
            }
            default: {
                break;
            }
        }
    }
    endwin();
    return 0;
}
