#ifndef STATES_H
#define STATES_H

/******************************
 *          Typedefs          *
 ******************************/

// State enumeration
typedef enum {
    DISARMED,
    ARMED,
    TRIGGERED,
    GRACE,
    MAINTENANCE,
    DELAY,
    CHANGE_PASSWORD,
    NUM_STATES // Number of states (automatically calculated)
} State_t;

// State machine structure
typedef struct {
    State_t state;
    void (*prepare_state)(void);
    void (*handle_state)(void);
    State_t (*evaluate_state)(void);
    void (*finish_state)(void);
} StateMachine_t;


/******************************
 *       Global Variables     *
 ******************************/
#ifndef PIN_LENGTH
#define PIN_LENGTH 4
#endif

// State variables
extern State_t current_state;
extern State_t next_state;

// Generic constants
extern volatile int state_code;
extern volatile int password_correct;
extern volatile int environment;
extern volatile int timer;
extern volatile char globalPassword[PIN_LENGTH + 1];
extern volatile char keyBuffer[PIN_LENGTH + 1];

// Disarmed constants
extern volatile int menu_selection;
extern volatile int go_in_maintenance;
extern volatile int go_in_armed;
extern volatile int go_in_change_password;
extern volatile int flag;
extern volatile int back_to_menu;
extern volatile int last_selection;
extern volatile int menu_done;
extern volatile int temp;
extern volatile int prevtemp;

// Triggered constants
extern volatile int opened_safe;
extern volatile int opened_critical;


/******************************
 *    Function Declarations   *
 ******************************/

/* States:
 * ARMED: System is active and monitoring for intrusions
 * DISARMED: System is inactive
 * GRACE: A transition period (entering/exiting)
 * TRIGGERED: Alarm has been activated
 */

// Armed interface
void prepare_armed(void);
void handle_armed(void);
State_t evaluate_armed(void);
void finish_armed(void);

// Disarmed interface
void prepare_disarmed(void);
void handle_disarmed(void);
State_t evaluate_disarmed(void);
void finish_disarmed(void);

// Grace interface
void prepare_grace(void);
void handle_grace(void);
State_t evaluate_grace(void);
void finish_grace(void);

// Triggered interface
void prepare_triggered(void);
void handle_triggered(void);
State_t evaluate_triggered(void);
void finish_triggered(void);

// Maintenance interface
void prepare_maintenance(void);
void handle_maintenance(void);
State_t evaluate_maintenance(void);
void finish_maintenance(void);

// Delay interface
void prepare_delay(void);
void handle_delay(void);
State_t evaluate_delay(void);
void finish_delay(void);

// Password Change interface (AGGIUNTA)
void prepare_change_password(void);
void handle_change_password(void);
State_t evaluate_change_password(void);
void finish_change_password(void);

#endif // STATES_H
