#include "custom_sender.h"

int scrwidth = 0, scrheight = 0;
enum interfaces_t chosen_intf = primary_intf;
int n_shown_msgs              = 20;

static_assert(N_CAN_DEVICES == (sizeof(can_devices) / GEN_STR_LEN));

int main(int argc, char const *argv[]) {
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
    curs_set(0);
    // nodelay(stdscr, 1);
    // cbreak();
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

    int current_focus = 0;

    bool quit = false;
    while (!quit) {
        render_menus[ctab](current_focus);
        char user_input = getch();

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
                if (ctab == search_menu) {
                    current_focus = 0;
                    chosen_intf   = secondary_intf;
                }
                break;
            case 'p':
                if (ctab == search_menu) {
                    current_focus = 0;
                    chosen_intf   = primary_intf;
                }
                break;
            case 'i':
                if (ctab == search_menu) {
                    current_focus = 0;
                    chosen_intf   = inverter_intf;
                }
                break;
            case 27:
            case 'm':
                ctab = search_menu;
                clear();
                break;
            case 'w':
                current_focus = current_focus_dec(current_focus);
                break;
            case 's':
                current_focus = current_focus_inc(current_focus);
                break;
            case CTRL('s'):

                break;
            case '\n':
                current_focus = action(current_focus);
                break;
            default:
                break;
        }
    }
    endwin();
    return 0;
}
