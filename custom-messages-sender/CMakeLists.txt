cmake_minimum_required(VERSION 3.16.3)
project(custom_message_sender)

set(CMAKE_C_FLAGS "-g -Og -std=gnu23")

add_executable(${PROJECT_NAME}
    Src/main.c
    Src/custom_sender.c
    can/src/primary/can-primary-api.c
    can/src/primary/primary-utils.c
    can/src/inverters/can-inverters-api.c
    can/src/inverters/inverters-utils.c
    can/src/bms/can-bms-api.c
    can/src/bms/bms-utils.c
    libeagletrt-sw/src/eagletrt-api.c
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/can/include
    ${CMAKE_CURRENT_SOURCE_DIR}/can/include/bms
    ${CMAKE_CURRENT_SOURCE_DIR}/can/include/inverters
    ${CMAKE_CURRENT_SOURCE_DIR}/can/include/primary
    ${CMAKE_CURRENT_SOURCE_DIR}/libeagletrt-sw/include
)
target_link_libraries(${PROJECT_NAME}
    ncurses
    m
)
message(STATUS "Using C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "C Compiler Version: ${CMAKE_C_COMPILER_VERSION}")
