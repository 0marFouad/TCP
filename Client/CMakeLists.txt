cmake_minimum_required(VERSION 3.12)
project(Client)

set(CMAKE_CXX_STANDARD 14)

add_executable(Client main.cpp ClientStarter.cpp ClientStarter.h Utils.cpp Utils.h Packet.cpp Packet.h)