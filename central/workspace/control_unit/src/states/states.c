#include "../../include/states.h"

/******************************
 *      State Variables       *
 ******************************/
State_t current_state = DISARMED;
State_t next_state = DISARMED;

/******************************
 *      System Variables      *
 ******************************/
volatile int timer = 30;  // 30-second initial value
volatile int password_correct = 0;
volatile int opened_safe = 0;      // Door opened
volatile int opened_critical = 0;  // Other sensor opened
volatile int light = 0;

/******************************
 *      Menu Variables        *
 ******************************/
volatile int menu_selection = 0;
volatile int back_to_menu = 0;
volatile int last_selection = 0;
volatile int menu_done = 0;

/******************************
 *      Mode Transition Flags *
 ******************************/
volatile int go_in_maintenance = 0;
volatile int go_in_armed = 0;
volatile int flag = 0;

