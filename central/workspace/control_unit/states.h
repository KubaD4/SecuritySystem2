#ifndef STATEMACHINEIOT_STATES_H
#define STATEMACHINEIOT_STATES_H

// volatile variables
extern volatile int state_code;
extern volatile int password_correct;
extern volatile int opened_safe;
extern volatile int opened_critical;
extern volatile int grace_timer;


//States constants
#define ALARM_STATE_ARMED       1
#define ALARM_STATE_DISARMED    2
#define ALARM_STATE_GRACE       3
#define ALARM_STATE_TRIGGERED   4

//Armed constants
#define ARMED_FULL          1
#define ARMED_VOLUMETRIC    2
#define ARMED_PERIMETRAL    3


/* States:
ARMED: System is active and monitoring for intrusions
DISARMED: System is inactive
GRACE: A transition period (entering/exiting)
TRIGGERED: Alarm has been activated */


//Disarmed constants


//Grace constants

//Triggered constants

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

#endif //STATEMACHINEIOT_STATES_H
