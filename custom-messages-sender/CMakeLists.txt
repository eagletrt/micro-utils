cmake_minimum_required(VERSION 3.16.3)
project(custom_message_sender)

add_executable(${PROJECT_NAME}
    Src/main.c
    Src/custom_sender.c
    can/lib/primary/primary_network.c
    can/lib/primary/primary_utils.c
    can/lib/secondary/secondary_network.c
    can/lib/secondary/secondary_utils.c
    can/lib/inverters/inverters_network.c
    can/lib/inverters/inverters_utils.c
    can/lib/bms/bms_network.c
    can/lib/bms/bms_utils.c
)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/can/lib
) 
target_link_libraries(${PROJECT_NAME}
    ncurses
    m
)
