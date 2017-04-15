cmake_minimum_required(VERSION 3.6)
project(smartHouse)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost COMPONENTS system thread REQUIRED)
include_directories(server GPIO)
link_libraries(boost_system boost_thread pthread)
set(SOURCE_FILES main.cpp server/smartHouseApi.hpp GPIO/PinCtrl.cpp GPIO/PinCtrl.hpp log.cpp log.hpp)
add_executable(smartHouse ${SOURCE_FILES})
