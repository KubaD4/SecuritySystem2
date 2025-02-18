#include "../central/workspace/control_unit/include/states.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

// Function to reset system variables
void reset_system_variables() {
    /******************************
     *      System Variables      *
     ******************************/
    timer = 30;              // 30-second initial value
    password_correct = 0;
    opened_safe = 0;         // Door opened
    opened_critical = 0;     // Other sensor opened
    environment = 0;

    /******************************
     *      Menu Variables        *
     ******************************/
    menu_selection = 0;
    back_to_menu = 0;
    last_selection = 0;
    menu_done = 0;

    /******************************
     *      Mode Transition Flags *
     ******************************/
    go_in_maintenance = 0;
    go_in_armed = 0;
    flag = 0;
}

// Function to print test results in a pretty format
void print_test_result(const char* test_name, int passed) {
    printf("Test: %-30s [%s]\n", test_name, passed ? "PASSED" : "FAILED");
}

// Test cases for GRACE state
void test_grace_state() {
    printf("=== Testing GRACE State ===\n");

    // Test no transition
    reset_system_variables();
    back_to_menu = 1;
    next_state = evaluate_grace();
    print_test_result("No Transition", next_state == GRACE);

    // Test environment alarm trigger
    reset_system_variables();
    environment = 1;
    next_state = evaluate_grace();
    print_test_result("Environment Alarm Trigger", next_state == TRIGGERED);

    // Test timer expired
    reset_system_variables();
    timer = 0;
    next_state = evaluate_grace();
    print_test_result("Timer Expired", next_state == TRIGGERED);

    // Test password correct
    reset_system_variables();
    password_correct = 1;
    next_state = evaluate_grace();
    print_test_result("Password Correct", next_state == DISARMED);

    printf("\n");
}

// Test cases for ARMED state
void test_armed_state() {
    printf("=== Testing ARMED State ===\n");

    // Test no transition
    reset_system_variables();
    next_state = evaluate_armed();
    print_test_result("No Transition", next_state == ARMED);

    // Test password correct
    reset_system_variables();
    password_correct = 1;
    next_state = evaluate_armed();
    print_test_result("Password Correct", next_state == DISARMED);

    // Test open safe
    reset_system_variables();
    opened_safe = 1;
    next_state = evaluate_armed();
    print_test_result("Open Safe", next_state == GRACE);

    // Test open critical
    reset_system_variables();
    opened_critical = 1;
    next_state = evaluate_armed();
    print_test_result("Open Critical", next_state == TRIGGERED);

    // Test environment
    reset_system_variables();
    environment = 1;
    next_state = evaluate_armed();
    print_test_result("Environment Trigger", next_state == TRIGGERED);

    printf("\n");
}

// Test cases for DISARMED state
void test_disarmed_state() {
    printf("=== Testing DISARMED State ===\n");

    // Test no transition
    reset_system_variables();
    next_state = evaluate_disarmed();
    print_test_result("No Transition", next_state == DISARMED);

    // Test environment
    reset_system_variables();
    environment = 1;
    next_state = evaluate_disarmed();
    print_test_result("Environment Trigger", next_state == TRIGGERED);

    // Test go in maintenance
    reset_system_variables();
    password_correct = 1;
    go_in_maintenance = 1;
    next_state = evaluate_disarmed();
    print_test_result("Go in Maintenance", next_state == MAINTENANCE);

    // Test go in armed
    reset_system_variables();
    password_correct = 1;
    go_in_armed = 1;
    next_state = evaluate_disarmed();
    print_test_result("Go in Armed", next_state == DELAY);

    // Test password change
    reset_system_variables();
    password_correct = 1;
    go_in_change_password = 1;
    next_state = evaluate_disarmed();
    print_test_result("Password Change", next_state == CHANGE_PASSWORD);

    printf("\n");
}

// Test cases for TRIGGERED state
void test_triggered_state() {
    printf("=== Testing TRIGGERED State ===\n");

    // Test no transition
    reset_system_variables();
    next_state = evaluate_triggered();
    print_test_result("No Transition", next_state == TRIGGERED);

    // Test password correct
    reset_system_variables();
    password_correct = 1;
    next_state = evaluate_triggered();
    print_test_result("Password Correct", next_state == DISARMED);

    printf("\n");
}

// Test cases for DELAY state
void test_delay_state() {
    printf("=== Testing DELAY State ===\n");

    // Test no transition
    reset_system_variables();
    next_state = evaluate_delay();
    print_test_result("No Transition", next_state == DELAY);

    // Test password correct
    reset_system_variables();
    password_correct = 1;
    next_state = evaluate_delay();
    print_test_result("Password Correct", next_state == DISARMED);

    // Test environment
    reset_system_variables();
    environment = 1;
    next_state = evaluate_delay();
    print_test_result("Environment Trigger", next_state == TRIGGERED);

    // Test timer
    reset_system_variables();
    timer = 0;
    next_state = evaluate_delay();
    print_test_result("Timer Expired", next_state == ARMED);

    printf("\n");
}

// Test cases for MAINTENANCE state
void test_maintenance_state() {
    printf("=== Testing MAINTENANCE State ===\n");

    // Test no transition
    reset_system_variables();
    next_state = evaluate_maintenance();
    print_test_result("No Transition", next_state == MAINTENANCE);

    // Test back to menu
    reset_system_variables();
    back_to_menu = 1;
    next_state = evaluate_maintenance();
    print_test_result("Back to Menu", next_state == DISARMED);

    printf("\n");
}

// Test cases for CHANGE_PASSWORD state
void test_change_password_state() {
    printf("=== Testing CHANGE_PASSWORD State ===\n");

    // Test no transition
    reset_system_variables();
    next_state = evaluate_change_password();
    print_test_result("No Transition", next_state == CHANGE_PASSWORD);

    // Test set new password
    reset_system_variables();
    strcpy(keyBuffer, "1234");
    next_state = evaluate_change_password();
    print_test_result("Set New Password", next_state == DISARMED);

    printf("\n");
}

int main() {
    // Run all test cases
    test_grace_state();
    test_armed_state();
    test_disarmed_state();
    test_triggered_state();
    test_delay_state();
    test_maintenance_state();
    test_change_password_state();

    return 0;
}