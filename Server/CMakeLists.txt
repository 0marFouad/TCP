cmake_minimum_required(VERSION 3.12)
project(Server)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS -pthread)
add_executable(Server main.cpp ServerStarter.cpp ServerStarter.h Utils.cpp Utils.h Server.cpp Server.h Packet.cpp Packet.h)