cmake_minimum_required(VERSION 3.13.1)

include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project(pager)

list(APPEND HEADERS
    "${PROJECT_BINARY_DIR}/include/generated"
    "${PROJECT_BINARY_DIR}/zephyr/include/generated"
    "${PROJECT_SOURCE_DIR}/include"
    )

list(APPEND SOURCES
    "${PROJECT_SOURCE_DIR}/src/main.c"
    "${PROJECT_SOURCE_DIR}/src/input.c"
    "${PROJECT_SOURCE_DIR}/src/output.c"
    "${PROJECT_SOURCE_DIR}/src/led.c"
    "${PROJECT_SOURCE_DIR}/src/motor.c"
    "${PROJECT_SOURCE_DIR}/src/buzzer.c"
    "${PROJECT_SOURCE_DIR}/src/battery.c"
    "${PROJECT_SOURCE_DIR}/src/thread.c"
    "${PROJECT_SOURCE_DIR}/src/state_machine.c"
    "${PROJECT_SOURCE_DIR}/src/bt_callbacks.c"
    "${PROJECT_SOURCE_DIR}/src/bt_node.c"
    "${PROJECT_SOURCE_DIR}/src/bt_mesh.c"
)

include_directories(${HEADERS})
target_sources(app PRIVATE ${SOURCES})
