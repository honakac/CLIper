cmake_minimum_required(VERSION 3.0)

project(cliper)

add_executable(cliper main.cpp
                      cliper.cpp
                      database.cpp)

install(TARGETS cliper RUNTIME DESTINATION bin)
