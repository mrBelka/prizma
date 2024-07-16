cmake_minimum_required(VERSION 3.15)

# -static - compile in static mode
# -Wno-psabi - отключение предупреждений при компиляции nlohmann-json
set(CMAKE_CXX_FLAGS "-static -Wno-psabi")

set(PROJECT_NAME prizma)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)

# libs
include_directories("extlibs/asio/include")
include_directories("extlibs")

include_directories("utils/logger")
include_directories("io")
include_directories("protocol")
include_directories("devices")

add_executable(${PROJECT_NAME} main.cpp
                                utils/logger/Logger.cpp
                                io/Serial.cpp
                                protocol/ModbusRTUMaster.cpp
                                devices/UniversalModbusRTUClient.hpp)