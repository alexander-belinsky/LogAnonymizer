cmake_minimum_required(VERSION 3.27)
project(LogAnonimizer)

set(CMAKE_CXX_STANDARD 17)

add_executable(LogAnonimizer main.cpp libs/sqlite/sqlite3.c)
