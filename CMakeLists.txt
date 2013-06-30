cmake_minimum_required(VERSION 2.8)

project(d2_hack)


find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})


set(D2HackSources
  main.cpp
)


add_executable(d2_hack ${D2HackSources})
