#include "states.h"

volatile int state_code = ALARM_STATE_DISARMED;  // Start disarmed
volatile int password_correct = 0;
volatile int opened_safe = 0;   // Door opened
volatile int opened_critical = 0;   // Other sensor opened
volatile int grace_timer = 30;  // 30 second initial value
