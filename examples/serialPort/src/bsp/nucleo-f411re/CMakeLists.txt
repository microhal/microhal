cmake_minimum_required(VERSION 3.12)

add_library(microhal-configuration INTERFACE)
target_include_directories(microhal-configuration INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(serialPort_example PRIVATE ${CMAKE_CURRENT_LIST_DIR}/bsp_nucleo-f411re.cpp)
