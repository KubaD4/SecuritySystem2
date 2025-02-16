cmake_minimum_required(VERSION 3.26)
project(test C)

set(CMAKE_C_STANDARD 11)

add_definitions(-DTEST_BUILD)

add_executable(
        test
        test.c
        ../central/workspace/control_unit/src/states/states.c
        ../central/workspace/control_unit/src/states/armed.c
        ../central/workspace/control_unit/src/states/delay.c
        ../central/workspace/control_unit/src/states/disarmed.c
        ../central/workspace/control_unit/src/states/grace.c
        ../central/workspace/control_unit/src/states/maintenance.c
        ../central/workspace/control_unit/src/states/triggered.c
        ../central/workspace/control_unit/src/states/change_password.c
)

