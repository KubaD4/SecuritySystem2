#ifndef STATES_H
#define STATES_H

// volatile variables

//Generic constants
extern volatile int state_code;
extern volatile int password_correct;

extern volatile int grace_timer;

//Disarmed constants
extern volatile int menu_selection;
extern volatile int go_in_maintenance;
extern volatile int go_in_armed;
extern volatile int flag;
 volatile int back_to_menu;


//Triggered constants
extern volatile int opened_safe;
extern volatile int opened_critical;


//States constants
#define ALARM_STATE_ARMED       1
#define ALARM_STATE_DISARMED    2
#define ALARM_STATE_GRACE       3
#define ALARM_STATE_TRIGGERED   4


/* States:
ARMED: System is active and monitoring for intrusions
DISARMED: System is inactive
GRACE: A transition period (entering/exiting)
TRIGGERED: Alarm has been activated */

//Armed interface
void prepare_armed();
void handle_armed();
void finish_armed();

//Disarmed interface
void prepare_disarmed();
void handle_disarmed();
void finish_disarmed();

//Grace interface
void prepare_grace();
void handle_grace();
void finish_grace();

//Triggered interface
void prepare_triggered();
void handle_triggered();
void finish_triggered();

//Maintenance interface
void prepare_maintenance();
void handle_maintenance();
void finish_maintenance();

typedef enum {
    DISARMED,
    ARMED,
    TRIGGERED,
    GRACE,
    MAINTENANCE,
    NUM_STATES
}State_t;

extern State_t current_state;

#endif //STATEMACHINEIOT_STATES_H
