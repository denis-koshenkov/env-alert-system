cmake_minimum_required(VERSION 4.0.3)

find_package(Zephyr)
project(env-alert-system)

target_sources(app PRIVATE src/main.c)
