#include "../../include/states.h"

/******************************
 *      State Variables       *
 ******************************/
State_t current_state = DISARMED;
State_t next_state = DISARMED;
#ifndef PIN_LENGTH
#define PIN_LENGTH 4
#endif

/******************************
 *      System Variables      *
 ******************************/
volatile int timer = 30;  // 30-second initial value
volatile int password_correct = 0;
volatile int opened_safe = 0;      // Sensor opened
volatile int opened_critical = 0;  // Temperature high or shield removed
volatile int environment = 0;
volatile char globalPassword[PIN_LENGTH + 1] = "0000";
volatile char keyBuffer[PIN_LENGTH + 1] = {0};
/******************************
 *      Menu Variables        *
 ******************************/
volatile int menu_selection = 0;
volatile int back_to_menu = 0;
volatile int last_selection = 0;
volatile int menu_done = 0;
volatile int temp=20;
volatile int prevtemp=20;

/******************************
 *      Mode Transition Flags *
 ******************************/
volatile int go_in_maintenance = 0;
volatile int go_in_armed = 0;
volatile int go_in_change_password = 0;
volatile int change_password = 0;

