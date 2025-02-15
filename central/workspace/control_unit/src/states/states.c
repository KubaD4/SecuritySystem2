#include "../../include/states.h"

volatile int password_correct = 0;
volatile int opened_safe = 0;   // Door opened
volatile int opened_critical = 0;   // Other sensor opened
volatile int timer = 30;  // 30 second initial value
State_t current_state = DISARMED;
State_t next_state = DISARMED;

extern volatile int flag = 0;
extern volatile int go_in_maintenance = 0;
extern volatile int go_in_armed = 0;
extern volatile int menu_selection=0;
extern volatile int back_to_menu = 0;
extern volatile int last_selection = 0;
extern volatile int menu_done = 0;
extern volatile int light = 0;

