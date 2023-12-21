#include "custom_sender.h"

struct sockaddr_can address = {0};
int can_sockets[N_CAN_DEVICES] = {0};
char can_devices[N_CAN_DEVICES][GEN_STR_LEN] = CAN_DEVICES;

int open_can_socket(const char *device) {
  struct ifreq ifr;
  int can_socket = socket(PF_CAN, SOCK_RAW, CAN_RAW);
  if (can_socket < 0) {
    return -1;
  }
  strcpy(ifr.ifr_name, device);
  ioctl(can_socket, SIOCGIFINDEX, &ifr);
  address.can_family = AF_CAN;
  address.can_ifindex = ifr.ifr_ifindex;
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
  frame.can_id = msg->id;
  frame.can_dlc = msg->size;
  memcpy(frame.data, msg->data, msg->size * sizeof(char));
  return write(can_socket, &frame, sizeof(frame));
}

int _scrwidth = 0;
int _scrheight = 0;
enum application_tabs_t ctab = search_menu;
int chosen_msg_idx = -1;
enum interfaces_t chosen_interface = -1;
char current_fields[MAX_N_FIELDS][GEN_STR_LEN] = {0};
int current_n_fields = -1;
bool _is_focused = false;

extern int n_shown_msgs;
extern enum interfaces_t chosen_intf;

can_msg_metadata_t metadata_msgs[primary_MESSAGE_COUNT +
                                 secondary_MESSAGE_COUNT +
                                 inverters_MESSAGE_COUNT];

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
  }
  return 0;
}

int get_intf_base_idx() {
  switch (chosen_intf) {
  case primary_intf:
    return 0;
  case secondary_intf:
    return primary_MESSAGE_COUNT;
  case inverter_intf:
    return primary_MESSAGE_COUNT + secondary_MESSAGE_COUNT;
  }
  return 0;
}

int get_idx_from_intf(int bidx, enum interfaces_t intf) {
  switch (intf) {
  case primary_intf:
    return bidx;
  case secondary_intf:
    return primary_MESSAGE_COUNT + bidx;
  case inverter_intf:
    return primary_MESSAGE_COUNT + secondary_MESSAGE_COUNT + bidx;
  }
}

int init_canlib_metadata(void) {
  for (size_t iidx = 0; iidx < primary_MESSAGE_COUNT; iidx++) {
    uint16_t id = primary_id_from_index(iidx);
    int imsg = get_idx_from_intf(iidx, primary_intf);
    int n_field = primary_n_fields_from_id(id);
    metadata_msgs[imsg].n_fields = n_field;
    metadata_msgs[imsg].msg_name = (char *)malloc(GEN_STR_LEN * sizeof(char));
    if (!primary_message_name_from_id(id, metadata_msgs[imsg].msg_name)) {
      fprintf(stderr, "Could not retrieve primary message name from id %d\n",
              (int)id);
      return 0;
    }
    metadata_msgs[imsg].fields_type = (int *)malloc(n_field * sizeof(int));
    if (n_field > 0) {
      if (primary_fields_types_from_id(id, metadata_msgs[imsg].fields_type,
                                       n_field) == 0) {
        fprintf(stderr, "Could not retrieve primary fields types from id %d\n",
                (int)id);
        return 0;
      }
      metadata_msgs[imsg].fields_name =
          (char **)malloc(n_field * sizeof(char *));
      for (size_t ifield = 0; ifield < n_field; ifield++) {
        metadata_msgs[imsg].fields_name[ifield] =
            (char *)malloc(GEN_STR_LEN * sizeof(char));
      }
      if (primary_fields_string_from_id(
              id, (char **)metadata_msgs[imsg].fields_name, n_field,
              GEN_STR_LEN) == 1) {
        fprintf(stderr, "Could not retrieve primary string types from id %d\n",
                (int)id);
        return 0;
      }
    }
  }
  for (size_t iidx = 0; iidx < secondary_MESSAGE_COUNT; iidx++) {
    uint16_t id = secondary_id_from_index(iidx);
    if (id == 0xFF) {
      fatal_error("Could not retrieve secondary message id from index");
    }
    int imsg = get_idx_from_intf(iidx, secondary_intf);
    int n_field = secondary_n_fields_from_id(id);
    metadata_msgs[imsg].n_fields = n_field;
    metadata_msgs[imsg].msg_name = (char *)malloc(GEN_STR_LEN * sizeof(char));
    if (!secondary_message_name_from_id(id, metadata_msgs[imsg].msg_name)) {
      fprintf(stderr, "Could not retrieve secondary message name from id %d\n",
              (int)id);
      return 0;
    }
    metadata_msgs[imsg].fields_type = (int *)malloc(n_field * sizeof(int));
    if (n_field > 0) {
      if (secondary_fields_types_from_id(id, metadata_msgs[imsg].fields_type,
                                         n_field) == 0) {
        fprintf(stderr,
                "Could not retrieve secondary fields types from id %d\n",
                (int)id);
        return 0;
      }
      metadata_msgs[imsg].fields_name =
          (char **)malloc(n_field * sizeof(char *));
      for (size_t ifield = 0; ifield < n_field; ifield++) {
        metadata_msgs[imsg].fields_name[ifield] =
            (char *)malloc(GEN_STR_LEN * sizeof(char));
      }
      if (secondary_fields_string_from_id(
              id, (char **)metadata_msgs[imsg].fields_name, n_field,
              GEN_STR_LEN) == 1) {
        fprintf(stderr,
                "Could not retrieve secondary string types from id %d\n",
                (int)id);
        return 0;
      }
    }
  }
  for (size_t iidx = 0; iidx < inverters_MESSAGE_COUNT; iidx++) {
    uint16_t id = inverters_id_from_index(iidx);
    if (id == 0xFF) {
      fatal_error("Could not retrieve inverter message id from index");
    }
    int imsg = get_idx_from_intf(iidx, inverter_intf);
    int n_field = inverters_n_fields_from_id(id);
    metadata_msgs[imsg].n_fields = n_field;
    metadata_msgs[imsg].msg_name = (char *)malloc(GEN_STR_LEN * sizeof(char));
    if (!inverters_message_name_from_id(id, metadata_msgs[imsg].msg_name)) {
      fprintf(stderr, "Could not retrieve inverters message name from id %d\n",
              (int)id);
      return 0;
    }
    metadata_msgs[imsg].fields_type = (int *)malloc(n_field * sizeof(int));
    if (n_field > 0) {
      if (inverters_fields_types_from_id(id, metadata_msgs[imsg].fields_type,
                                         n_field) == 0) {
        fprintf(stderr,
                "Could not retrieve inverters fields types from id %d\n",
                (int)id);
        return 0;
      }
      metadata_msgs[imsg].fields_name =
          (char **)malloc(n_field * sizeof(char *));
      for (size_t ifield = 0; ifield < n_field; ifield++) {
        metadata_msgs[imsg].fields_name[ifield] =
            (char *)malloc(GEN_STR_LEN * sizeof(char));
      }
      if (inverters_fields_string_from_id(
              id, (char **)metadata_msgs[imsg].fields_name, n_field,
              GEN_STR_LEN) == 1) {
        fprintf(stderr,
                "Could not retrieve inverters string types from id %d\n",
                (int)id);
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
  int msgid = -1;
  can_message_t msg = {0};
  char to_serialize[BUFSIZ];
  char final_command[BUFSIZ];
  int can_send_retval = 0;
  int current_to_serialize_idx = 0;
  for (size_t ifield = 0; ifield < current_n_fields; ifield++) {
    int field_len = strlen(current_fields[ifield]);
    memcpy(to_serialize + current_to_serialize_idx, " ", 1);
    current_to_serialize_idx++;
    memcpy(to_serialize + current_to_serialize_idx, current_fields[ifield],
           field_len);
    current_to_serialize_idx += field_len;
  }
  switch (chosen_intf) {
  case primary_intf: {
    msgid = primary_id_from_index(chosen_msg_idx);
    msg.id = msgid;
    size_t msg_sz = 0;
    primary_serialize_from_id(msgid, to_serialize, msg.data, &msg_sz);
    msg.size = (uint8_t)msg_sz;
    fill_data(&msg, (char *)final_command, (char *)can_devices[0]);
    can_send_retval = can_send(&msg, can_sockets[0]);
    break;
  }
  case secondary_intf: {
    msgid = secondary_id_from_index(chosen_msg_idx);
    msg.id = msgid;
    size_t msg_sz = 0;
    secondary_serialize_from_id(msgid, to_serialize, msg.data, &msg_sz);
    msg.size = (uint8_t)msg_sz;
    fill_data(&msg, (char *)final_command, (char *)can_devices[1]);
    can_send_retval = can_send(&msg, can_sockets[1]);
    break;
  }
  case inverter_intf: {
    msgid = inverters_id_from_index(chosen_msg_idx);
    msg.id = msgid;
    size_t msg_sz = 0;
    inverters_serialize_from_id(msgid, to_serialize, msg.data, &msg_sz);
    msg.size = (uint8_t)msg_sz;
    fill_data(&msg, (char *)final_command, (char *)can_devices[0]);
    can_send_retval = can_send(&msg, can_sockets[0]);
    break;
  }
  }
  WINDOW *prompt_win =
      newwin(SUBWIN_HEIGHT, SUBWIN_WIDTH, (scrheight - SUBWIN_HEIGHT) / 2,
             (scrwidth - SUBWIN_WIDTH) / 2);
  if (prompt_win == NULL) {
    assert(0 && "Could not create a input prompt window");
  }
  wclear(prompt_win);
  box(prompt_win, 0, 0);
  int current_row = 1;
  if (can_send_retval < 0) {
    WRITE_CENTERED(prompt_win, current_row, "[ERROR]: Could not send message",
                   -1);
  } else {
    WRITE_CENTERED(prompt_win, current_row, "Message was sent successfully",
                   -1);
  }
  current_row++;
  WRITE_CENTERED(prompt_win, current_row,
                 "Manual cansend command [COULD NOT WORK]", -1);
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
    chosen_msg_idx = current_focus + get_intf_base_idx();
    chosen_interface = chosen_intf;
    current_n_fields = metadata_msgs[chosen_msg_idx].n_fields;
    ctab = fill_fields_menu;
    for (size_t ifield = 0; ifield < MAX_N_FIELDS; ifield++) {
      memcpy(current_fields[ifield], "0", GEN_STR_LEN);
    }
    return 0;
  case fill_fields_menu:
    if (current_focus < current_n_fields) {
      retrieve_input_prompt(current_focus);
    } else {
      send_message(current_focus);
      chosen_msg_idx = -1;
      chosen_interface = primary_intf;
      current_n_fields = -1;
      ctab = search_menu;
      return 0;
    }
    return current_focus;
  }
}

int current_focus_dec(int current_focus) {
  switch (ctab) {
  case search_menu:
    return clamp(current_focus - 1, 0, get_intf_tot_msg() - 1);
  case fill_fields_menu:
    return clamp(current_focus - 1, 0, metadata_msgs[chosen_msg_idx].n_fields);
  }
}

int current_focus_inc(int current_focus) {
  switch (ctab) {
  case search_menu:
    return clamp(current_focus + 1, 0, get_intf_tot_msg() - 1);
  case fill_fields_menu:
    return clamp(current_focus + 1, 0, metadata_msgs[chosen_msg_idx].n_fields);
  }
}

int retrieve_input_prompt(int current_focus) {
  char input_prompt[GEN_STR_LEN] = {0};
  int cind = 0, input_enter = 0;
  WINDOW *prompt_win =
      newwin(SUBWIN_HEIGHT, SUBWIN_WIDTH, (scrheight - SUBWIN_HEIGHT) / 2,
             (scrwidth - SUBWIN_WIDTH) / 2);
  if (prompt_win == NULL) {
    assert(0 && "Could not create a input prompt window");
  }

  while (cind < GEN_STR_LEN && !input_enter) {
    wclear(prompt_win);
    box(prompt_win, 0, 0);
    int current_row = 1;
    WRITE_CENTERED(prompt_win, current_row, "Enter the value for that field",
                   -1);
    current_row++;
    WRITE_CENTERED(prompt_win, current_row, input_prompt, -1);
    wrefresh(prompt_win);
    int inchar = getch();
    switch (inchar) {
    case KEY_ENTER:
    case '\n':
      input_enter = true;
      break;
    case KEY_BACKSPACE:
    case 127:
    case '\b':
      cind = clamp(cind - 1, 0, cind);
      input_prompt[cind] = 0;
      break;
    default:
      input_prompt[cind] = inchar;
      cind++;
      break;
    }
  }
  memcpy(current_fields[current_focus], input_prompt, GEN_STR_LEN);
  delwin(prompt_win);
}

int render_search_menu(int current_focus) {
  wclear(stdscr);
  wattrset(stdscr, COLOR_PAIR(MAIN_THEME));
  int current_row = 0;
  box(stdscr, 0, 0);

  const char title[] = "CUSTOM MESSAGE SENDER MENU";
  wattrset(stdscr, COLOR_PAIR(TITLE_THEME));
  current_row++;
  WRITE_CENTERED(stdscr, current_row, title, -1);
  wattrset(stdscr, COLOR_PAIR(MAIN_THEME));
  PRINT_INDICATIONS(stdscr);

  int intf_tot_msg = get_intf_tot_msg();
  int intf_base_idx = get_intf_base_idx();

  for (size_t iidx = 0;
       iidx < (n_shown_msgs) && (iidx + current_focus) < intf_tot_msg; iidx++) {
    int showed_msg = current_focus + iidx;
    char *msg_name = metadata_msgs[intf_base_idx + showed_msg].msg_name;
    WRITE_CENTERED(stdscr, current_row + iidx, msg_name, showed_msg);
  }
}

int render_fill_fields_menu(int current_focus) {
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
  snprintf(chosen_msg_label, GEN_LABEL_LEN, "Chosen msg name: %s",
           metadata_msgs[chosen_msg_idx].msg_name);
  wattrset(stdscr, COLOR_PAIR(TITLE_THEME));
  WRITE_CENTERED(stdscr, current_row, chosen_msg_label, -1);
  wattrset(stdscr, COLOR_PAIR(MAIN_THEME));
  current_row += 2;

  const char first_table_row[] = "Available fields";
  WRITE_CENTERED(stdscr, current_row, first_table_row, -1);
  current_row++;
  mvwhline(stdscr, current_row, (scrwidth - SEPARATOR_LEN) / 2, '~',
           SEPARATOR_LEN);
  current_row++;
  int n_fields = metadata_msgs[chosen_msg_idx].n_fields;
  for (size_t ifield = current_focus;
       ifield < n_fields && ifield < (10 + current_focus); ifield++) {
    char *field_name = metadata_msgs[chosen_msg_idx].fields_name[ifield];
    int field_type = metadata_msgs[chosen_msg_idx].fields_type[ifield];
    char field_label[GEN_LABEL_LEN] = {0};
    snprintf(field_label, GEN_LABEL_LEN, "%s = %s", field_name,
             current_fields[ifield]);
    WRITE_CENTERED(stdscr, current_row, field_label, ifield);
    current_row++;
  }
  const char label_send_message[] = "Send this message";
  WRITE_CENTERED(stdscr, current_row, label_send_message, n_fields);
  current_row++;
  return 0;
}

int (*render_menus[n_application_tabs])(int) = {render_search_menu,
                                                render_fill_fields_menu};
